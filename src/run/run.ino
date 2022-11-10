/*
Hackerface team
*/
#define ML_Ctrl 4     //define direction control pin of B motor
#define ML_PWM 5   //define PWM control pin of B motor
#define MR_Ctrl 2    //define direction control pin of A motor
#define MR_PWM 9   //define PWM control pin of A motor
int BLE_val;
int BT_Serial_Iter = 7;
int BT_Serial_Vals[] = {0, 0, 0, 0, 0, 0, 0};
 
void setup()
{
  Serial.begin(9600);
  pinMode(ML_Ctrl, OUTPUT);//set direction control pin of B motor to OUTPUT
  pinMode(ML_PWM, OUTPUT);//set PWM control pin of B motor to OUTPUT
  pinMode(MR_Ctrl, OUTPUT);//set direction control pin of A motor to OUTPUT
  pinMode(MR_PWM, OUTPUT);//Set PWM control pin of A motor to OUTPUT
}

void loop()
{
  readBTInput();
}

void readBTInput() {
  if(Serial.available()>0)
  {
    BLE_val = int(Serial.read());
    // Serial.println(BLE_val);

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
    }
   
    if(BLE_val == 255) {
      // If we receive a 255 value, we reset input tracking to receive new values from the BT Serial monitor
      // Serial.println("Setting new values");
      BT_Serial_Iter = 0;
    }
  }
}
//*********************************************************

void backward()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
  analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
}
void backward_rigth()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,36);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
  analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
}
void backward_left()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH
  analogWrite(MR_PWM,36);//set PWM control speed of A motor to 200
}
void forward()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
}
void forward_right()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,36);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,255);//set PWM control speed of A motor to 200
}
void forward_left()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,255);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,36);//set PWM control speed of A motor to 200
}
void left()
{
  digitalWrite(ML_Ctrl,LOW);//set direction control pin of B motor to LOW
  analogWrite(ML_PWM,200);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,HIGH);//set direction control pin of A motor to HIGH 
  analogWrite(MR_PWM,200);//set PWM control speed of A motor to 200
}
void right()
{
  digitalWrite(ML_Ctrl,HIGH);//set direction control pin of B motor to HIGH 
  analogWrite(ML_PWM,200);//set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl,LOW);//set direction control pin of A motor to LOW
  analogWrite(MR_PWM,200);//set PWM control speed of A motor to 200
}
void stopped()
{
  analogWrite(ML_PWM,0);//set PWM control speed of B motor to 0
  analogWrite(MR_PWM,0);//set PWM control speed of A motor to 0
}