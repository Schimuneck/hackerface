/*
Hackerface team
*/
#define ML_Ctrl 4     //define direction control pin of B motor
#define ML_PWM 5   //define PWM control pin of B motor
#define MR_Ctrl 2    //define direction control pin of A motor
#define MR_PWM 9   //define PWM control pin of A motor
const int sensor_l = 6;//define the pin of left line tracking sensor
const int sensor_c = 7;//define the pin of middle line tracking sensor
const int sensor_r = 8;//define the pin of right line tracking sensor
int BLE_val;
int BLE_code = 1;
int ST_control = 1;
int ST_linefollow = 2;
int ST_maze = 3;
int ST_lcd = 4;
int BT_Serial_Iter = 7;
int BT_Serial_Vals[] = {0, 0, 0, 0, 0, 0, 0};
int l_val,c_val,r_val;//define these variables
int line_started = 0;
int line_back_count = 0;
 
void setup()
{
  Serial.begin(9600);
  pinMode(ML_Ctrl, OUTPUT);//set direction control pin of B motor to OUTPUT
  pinMode(ML_PWM, OUTPUT);//set PWM control pin of B motor to OUTPUT
  pinMode(MR_Ctrl, OUTPUT);//set direction control pin of A motor to OUTPUT
  pinMode(MR_PWM, OUTPUT);//Set PWM control pin of A motor to OUTPUT
  pinMode(sensor_l,INPUT);//set the pins of left line tracking sensor to INPUT
  pinMode(sensor_c,INPUT);//set the pins of middle line tracking sensor to INPUT
  pinMode(sensor_r,INPUT);//set the pins of right line tracking sensor to INPUT
}

void loop()
{
  // readBTInput();
  // if (BLE_code == ST_linefollow){
    tracking();
  // }
}

void readBTInput() {
  if(Serial.available()>0)
  {
    BLE_val = int(Serial.read());
    Serial.println(BLE_val);

    if(BT_Serial_Iter < 7) {
      BT_Serial_Vals[BT_Serial_Iter] = BLE_val;
      BT_Serial_Iter++;
      if(BT_Serial_Iter == 7) {
        // Serial.println("New values set");
        // Serial.println(String(BT_Serial_Vals[0]) + ", " + String(BT_Serial_Vals[1]) + ", " + String(BT_Serial_Vals[2]) + ", " + String(BT_Serial_Vals[3]) + ", " + String(BT_Serial_Vals[4]) + ", " + String(BT_Serial_Vals[5]) + ", " + String(BT_Serial_Vals[6]));

        // First value being 0 means the controls just changed, so we can ignore new values when that comes in.
        if (BT_Serial_Vals[0] == 0) {
          return;
        }

        if(BT_Serial_Vals[4] & 2){
          BLE_code = BLE_code + 1;
          if(BLE_code>ST_lcd){
            BLE_code = 1;            
          } else if (BLE_code == ST_linefollow){
            line_started = 0;
          }
        }

        if(BLE_code == ST_control){
          car_control();
        } 
      }
    }
   
    if(BLE_val == 255) {
      // If we receive a 255 value, we reset input tracking to receive new values from the BT Serial monitor
      // Serial.println("Setting new values");
      BT_Serial_Iter = 0;
    }
  }
}
//*********************************************************
void car_control(){
  if(BT_Serial_Vals[4] & 16) {
    if(BT_Serial_Vals[5] & 4) {
      Serial.println("Forward rigth");
      forward_right();
    } else if(BT_Serial_Vals[5] & 8){
      Serial.println("Forward left");
      forward_left();
    } else {
      Serial.println("Full forward");
      forward();
    }
  } else if(BT_Serial_Vals[4] & 32) {
    if(BT_Serial_Vals[5] & 4) {
      Serial.println("Backward rigth");
      backward_rigth();
    } else if(BT_Serial_Vals[5] & 8){
      Serial.println("Backward left");
      backward_left();
    } else {
      Serial.println("Full backward");
      backward();
    }
  } else if(BT_Serial_Vals[5] & 4) {
      Serial.println("Full rigth");
      right();
    }
  else if(BT_Serial_Vals[5] & 8) {
      Serial.println("Full left");
      left();
    }
  
  else {
    Serial.println("Stopped");
    stopped();
  }
}
// void backward()
// {
//   digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
//   analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
//   analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
// }
// void backward_rigth()
// {
//   digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
//   analogWrite(ML_PWM,36);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
//   analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
// }
// void backward_left()
// {
//   digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
//   analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
//   analogWrite(MR_PWM,36);//set PWM control speed of A motor to 200
// }
// void forward()
// {
//   digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
//   analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
//   analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
// }
// void forward_right()
// {
//   digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
//   analogWrite(ML_PWM,36);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
//   analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
// }
// void forward_left()
// {
//   digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
//   analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
//   analogWrite(MR_PWM,36);//set PWM control speed of A motor to 200
// }
// void left()
// {
//   digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
//   analogWrite(ML_PWM,200);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH 
//   analogWrite(MR_PWM,200);//set PWM control speed of A motor to 200
// }
// void right()
// {
//   digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
//   analogWrite(ML_PWM,200);//set PWM control speed of B motor to 200
//   digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
//   analogWrite(MR_PWM,200);//set PWM control speed of A motor to 200
// }
// void stopped()
// {
//   analogWrite(ML_PWM,0);//set PWM control speed of B motor to 0
//   analogWrite(MR_PWM,0);//set PWM control speed of A motor to 0
// }

void backward()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,100);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
  analogWrite(MR_PWM,100);//set PWM control speed of A motor to 200
}
void backward_rigth()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,85);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
  analogWrite(MR_PWM,170);//set PWM control speed of A motor to 200
}
void backward_left()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,170);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
  analogWrite(MR_PWM,85);//set PWM control speed of A motor to 200
}
void forward()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,170);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,170);//set PWM control speed of A motor to 200
}
void forward_right()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,85);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,170);//set PWM control speed of A motor to 200
}
void forward_left()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,170);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,85);//set PWM control speed of A motor to 200
}
void left()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,170);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH 
  analogWrite(MR_PWM,170);//set PWM control speed of A motor to 200
}
void right()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,170);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,170);//set PWM control speed of A motor to 200
}
void stopped()
{
  analogWrite(ML_PWM,0);//set PWM control speed of B motor to 0
  analogWrite(MR_PWM,0);//set PWM control speed of A motor to 0
}

void tracking()
{
  l_val = digitalRead(sensor_l);//read the value of left line tracking sensor
  c_val = digitalRead(sensor_c);//read the value of middle line tracking sensor
  r_val = digitalRead(sensor_r);//read the value of right line tracking sensor
  Serial.println(l_val);
  Serial.println(c_val);
  Serial.println(r_val);
  if(c_val == 1 || (r_val == 1 && l_val == 1))//if the state of middle one is 1, which means detecting black line
  {
    forward();//car goes forward
  }
  else if(r_val == 1)//if only left line tracking sensor detects black trace
    {
      forward_left();//car turns left
    }
  else if(l_val == 1)//if only right line tracking sensor detects black trace
    {
      forward_right();//car turns right
    }
  else// if left and right line tracking sensors detect black trace or they don’t read
    {
      backward();
      // line_back_count = line_back_count + 1;
      // if (line_back_count < 10){
      //   backward();
      // } 
      // else if (line_back_count < 15){    
      //   left();
      // } 
      // else if (line_back_count<25){    
      //   right();
      // }
      // else {
      //   line_back_count = 0;
                
      // }
    }
}
