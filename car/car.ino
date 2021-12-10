#include <Stepper.h>
#include <Ultrasonic.h>
#include <NRFLite.h>

struct pair {
  int a;
  int b;
};

const int maxSpeed = 590;
const int stepsPerRevolution = 64;

Stepper leftMotor = Stepper(stepsPerRevolution, 5, 6, 7, 8);
Stepper rightMotor = Stepper(stepsPerRevolution, 1, 2, 3, 4);

void setup() {
  // put your setup code here, to run once:
  leftMotor.setSpeed(100);
  rightMotor.setSpeed(100);    
}

void loop() {
  // put your main code here, to run repeatedly:
  leftMotor.step(1);
  rightMotor.step(1);
}

void stepMotorAtSpeed(Stepper motor, int motorSpeed, int steps){
  motor.setSpeed(abs(motorSpeed));
  
  if (motorSpeed < 0){
    motor.step(-steps);
  }
  else if (motorSpeed > 0){
    motor.step(steps);
  }
}

struct pair calculateMotorSpeeds(float x, float y){
  struct pair speeds;
  
  Serial.println("Readings");
  Serial.println(x);
  Serial.println(y);
  Serial.println(" ");
  
  float turn = map(x, 0, 1023, maxSpeed, -maxSpeed) - 9;
  float forward = map(y, 0, 1023, maxSpeed, -maxSpeed) + 5;

  Serial.println("Values");
  Serial.println(turn);
  Serial.println(forward);
  Serial.println(" ");
  
  if (abs(forward) < 2){
    forward = 0;
  }
  if (abs(turn) < 2){
    turn = 0;
  }


  int maximum = maxSpeed;
  int total = forward + turn;
  int difference = forward - turn;

  if (forward >= 0){
    if (turn > 0){
      speeds.a = maximum;
      speeds.b = difference;
    }
        
    else if (turn < 0){
      speeds.a = total;
      speeds.b = maximum;
    }
    else{
      speeds.a = forward;
      speeds.b = forward;        
    }
  }     
  else{
    if (turn >= 0){
      speeds.a = total;
      speeds.b = -maximum;
    }
      
    else{
      speeds.a = -maximum;
      speeds.b = difference;
    }   
  }
      
  Serial.println("Speeds");
  Serial.println(speeds.a);
  Serial.println(speeds.b);
  Serial.println(" ");
  return speeds;
}

void checkSerial() {
  if (Serial.available() > 0){
    byte message = Serial.read();
  }
}
