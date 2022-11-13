#include <Servo.h>
#include "SR04.h"
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;
// variable to store the servo position
const uint_8 data[] =
{
0x20, 0x00, 0x3c, 0x08, 0x10, 0x3c, 0x00, 0x3e, 0x0a, 0x62, 0x40, 0x7d, 0x00, 0x3e, 0x0a, 0x22
};

int trigPin = 12;  // Trigger
int echoPin = 13;  // Echo
long duration, cm, inches;
long distance;

#define ML_Ctrl 4  //define direction control pin of B motor
#define ML_PWM 5   //define PWM control pin of B motor
#define MR_Ctrl 2  //define direction control pin of A motor
#define MR_PWM 9   //define PWM control pin of A motor

int detectionDistance = 10;

int straightOffset = -10;
int speed = 255;

int turnDelay = 350;

SR04 sr04 = SR04(echoPin, trigPin);

void setup() {
  Serial.begin(9600);  //Serial Port begin

  setupServo();
  setupEyes();
  setupMotors();
}

void loop() {
  // uturn();

  // delay(5000);
  if (canGoStraight(detectionDistance)) {
    forward();
    return;
  }

  backward();

  delay(75);

  stopped();

  // setupServo();


  delay(100);


  long rightDistance = checkDistanceRight();
  bool canTurnRightVal = canTurnRight(25);

  long leftDistance = checkDistanceLeft();
  bool canTurnLeftVal = canTurnLeft(25);


  resetEyes();

  delay(150);

  // Serial.print("can go right");
  // Serial.print(canTurnRightVal);
  // Serial.println();

  // Serial.print("can go left");
  // Serial.print(canTurnLeftVal);
  // Serial.println();



  if (canTurnLeftVal && canTurnRightVal) {
    if (rightDistance < leftDistance) {
      right();
    } else {
      left();
    }
  } else {
  if (canTurnRightVal) {
    right();
  } else if (canTurnLeftVal) {
    left();
  } else {
    uturn();
  }
  }





  delay(turnDelay);

  resetEyes();
  // delay(150);
  // delay(1000);
  //
  // myservo.detach();


  // delay(1000);
}


void setupServo() {
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
  myservo.write(90);
}

void setupEyes() {
  pinMode(trigPin, OUTPUT);  //Define inputs and outputs
  pinMode(echoPin, INPUT);
  pinMode(3, OUTPUT);

  resetEyes();
  delay(100);
}

void setupMotors() {
  pinMode(ML_Ctrl, OUTPUT);  //set direction control pin of B motor to OUTPUT
  pinMode(ML_PWM, OUTPUT);   //set PWM control pin of B motor to OUTPUT
  pinMode(MR_Ctrl, OUTPUT);  //set direction control pin of A motor to OUTPUT
  pinMode(MR_PWM, OUTPUT);   //Set PWM control pin of A motor to OUTPUT
}


void turnEyes(int position) {
  myservo.write(position + straightOffset);
  // tell servo to go to position in variable 'pos'
  delay(300);
  // waits 15ms for the servo to reach the position
}


void resetEyes() {
  myservo.write(90 + straightOffset);
}


long checkDistance() {
  return sr04.Distance();
}

bool checkIfColliding(int collideValue) {
  distance = sr04.Distance();

  Serial.println(distance);


  if (distance <= collideValue && distance >= 0) {
    Serial.println("Something is in the way");
    return true;
  } else {
    Serial.println("Nothing is in the way");
    return false;
  }
}


bool canTurnLeft(int collidingValueLeft) {
  Serial.println("Checking left");

  turnEyes(180);


  return !checkIfColliding(collidingValueLeft);
}

bool canTurnRight(int collidingValueRight) {
  Serial.println("Checking right");

  turnEyes(0);

  return !checkIfColliding(collidingValueRight);
}


long checkDistanceRight() {
  Serial.println("Checking right");

  turnEyes(0);

  return checkDistance();
}


long checkDistanceLeft() {
  Serial.println("Checking left");

  turnEyes(180);

  return checkDistance();
}

bool canGoStraight(int collValue) {
  Serial.println("Checking straight");

  // resetEyes();
  return !checkIfColliding(collValue);
}

// void forward() {
//   Serial.println("Forward");

//   digitalWrite(ML_Ctrl, LOW);  //set direction control pin of B motor to HIGH
//   analogWrite(ML_PWM, 78);  //set PWM control speed of B motor to 150
//   digitalWrite(MR_Ctrl, LOW);  //set direction control pin of A motor to
//   analogWrite(MR_PWM,   78);  //set PWM control speed of A motor to 150
// }

void forward() {
  digitalWrite(ML_Ctrl, LOW);  //set direction control pin of B motor to LOW
  analogWrite(ML_PWM, speed);  //set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl, LOW);  //set direction control pin of A motor to LOW
  analogWrite(MR_PWM, speed);  //set PWM control speed of A motor to 200
}

void right() {
  Serial.println("Turn right");

  digitalWrite(ML_Ctrl, LOW);   //set direction control pin of B motor to HIGH
  analogWrite(ML_PWM, speed);   //set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl, HIGH);  //set direction control pin of A motor to LOW
  analogWrite(MR_PWM, speed);   //set PWM control speed of A motor to 200
}


void left() {
  Serial.println("Turn left");

  digitalWrite(ML_Ctrl, HIGH);  //set direction control pin of B motor to HIGH
  analogWrite(ML_PWM, speed);   //set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl, LOW);   //set direction control pin of A motor to LOW
  analogWrite(MR_PWM, speed);   //set PWM control speed of A motor to 200
}

void uturn() {
  right();
  // right();
  // right();
  // right();
  delay(675);


  // delay(turnDelay);

  stopped();
  delay(150);
}


void backward() {
  digitalWrite(ML_Ctrl, HIGH);  //set direction control pin of B motor to HIGH
  analogWrite(ML_PWM, 255);     //set PWM control speed of B motor to 200
  digitalWrite(MR_Ctrl, HIGH);  //set direction control pin of A motor to HIGH
  analogWrite(MR_PWM, 255);     //set PWM control speed of A motor to 200
}

void stopped() {
  analogWrite(ML_PWM, 0);  //set PWM control speed of B motor to 0
  analogWrite(MR_PWM, 0);  //set PWM control speed of A motor to 0
}