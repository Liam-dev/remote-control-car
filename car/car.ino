#include <Stepper.h>
#include <Ultrasonic.h>
#include <SPI.h>
#include <NRFLite.h>

struct Pair {
  int a;
  int b;
};

struct RadioData // Any packet up to 32 bytes can be sent.
{
    uint8_t radioId;
};

const int maxSpeed = 100;
const int stepsPerRevolution = 64;
const int temperaturePin = A0;

const static uint8_t radioId = 0;    // This radio's id.
//const static uint8_t receiverId = 0; // Id of the radio we will transmit to.
const static uint8_t CEPin = 9;
const static uint8_t CSNPin = 10;

NRFLite radio;
RadioData radioData;

Stepper leftMotor = Stepper(stepsPerRevolution, 6, 8, 7, A0);
Stepper rightMotor = Stepper(stepsPerRevolution, 2, 4, 3, 5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  
  if (!radio.init(radioId, CEPin, CSNPin))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
  
  
  leftMotor.setSpeed(400);
  rightMotor.setSpeed(400);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  leftMotor.step(-8);
  rightMotor.step(8);
  Serial.println(readTemperature());
}

int readTemperature(){
  return ((5 * analogRead(temperaturePin) / 1024) - 0.5) * 100;
}
  
void checkRadio() {
  while (radio.hasData())
  {
    radio.readData(&radioData); // Note how '&' must be placed in front of the variable name.

    String msg = "Radio ";
    msg += radioData.radioId;

    Serial.println(msg);
  }
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

struct Pair calculateMotorSpeeds(float x, float y){
  struct Pair speeds;
  
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
