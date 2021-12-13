#include <Stepper.h>
#include <Ultrasonic.h>
#include <SPI.h>
#include <NRFLite.h>

struct Pair {
  int a;
  int b;
};

// Constants
const int maxSpeed = 400;
const int stepsPerRevolution = 64;
const int temperaturePin = A1;

// Radio constants
const static uint8_t radioId = 10;    // This radio's id.
const static uint8_t receiverId = 11; // Id of the radio we will transmit to.
const static uint8_t CEPin = 9;
const static uint8_t CSNPin = 10;

// Variable declaration
NRFLite radio;
Pair radioData;
Pair sensorReadings;
unsigned long timeSinceLastReading;
Pair motorSpeeds;

// Stepper motor initialisation.
Stepper leftMotor = Stepper(stepsPerRevolution, 6, 8, 7, A0);
Stepper rightMotor = Stepper(stepsPerRevolution, 2, 4, 3, 5);

// Ultrasonic sensor initialisation.
Ultrasonic ultrasonicSensor(A2);

void setup() {
  // Set up pins and serial monitor.

  pinMode(temperaturePin, INPUT);

  Serial.begin(9600);


  if (!radio.init(radioId, CEPin, CSNPin))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
  Serial.println("Radio connected");

}

void loop() {
  checkRadio();

  if (millis() - timeSinceLastReading > 500) {
    timeSinceLastReading = millis();

    sensorReadings.a = readTemperature();
    sensorReadings.b = ultrasonicSensor.MeasureInCentimeters();
    
    Serial.println(sensorReadings.a);

    radio.send(receiverId, &sensorReadings, sizeof(sensorReadings));
  }
}

// Gets the temperature reading from the temperature sensor. Returns an integer value
// in degree Celsius.
int readTemperature() {
  return ((5 * ((double)analogRead(temperaturePin) / 1024)) - 0.5) * 100;
}

// Check the radio for new input
void checkRadio() {
  while (radio.hasData())
  {
    radio.readData(&radioData); // Note how '&' must be placed in front of the variable name.

    //Serial.print(radioData.a);
    //Serial.print(" ");
    //Serial.println(radioData.b);
  }

  Pair newMotorSpeeds = calculateMotorSpeeds(radioData);
  //Serial.print(-newMotorSpeeds.a);
  //Serial.print(" ");
  //Serial.println(newMotorSpeeds.b);



  /*
    if (abs(newMotorSpeeds.a - motorSpeeds.a) > 1){
    leftMotor.setSpeed(abs(newMotorSpeeds.a));
    }

    if (abs(newMotorSpeeds.b - motorSpeeds.b) > 1){
    rightMotor.setSpeed(abs(newMotorSpeeds.b));
    }
  */

  leftMotor.setSpeed(abs(newMotorSpeeds.a));
  rightMotor.setSpeed(abs(newMotorSpeeds.b));

  if (newMotorSpeeds.a < 0) {
    leftMotor.step(2);
  }
  else if (newMotorSpeeds.a > 0) {
    leftMotor.step(-2);
  }

  if (newMotorSpeeds.b < 0) {
    rightMotor.step(-2);
  }
  else if (newMotorSpeeds.b > 0) {
    rightMotor.step(2);
  }

  //motorSpeeds = newMotorSpeeds;
}

// Calculates the speeds of two motors from the joystick input values.
Pair calculateMotorSpeeds(struct Pair input) {
  Pair speeds;
  int x = input.b;
  int y = input.a;

  float turn = map(x, 0, 1023, -maxSpeed, maxSpeed) + 7;
  float forward = map(y, 0, 1023, -maxSpeed, maxSpeed) - 2;

  //Serial.print(turn);
  //Serial.print(" ");
  //Serial.println(forward);

  if (abs(forward) < 7) {
    forward = 0;
  }
  if (abs(turn) < 7) {
    turn = 0;
  }

  //Serial.print(turn);
  //Serial.print(" ");
  //Serial.println(forward);

  int maximum = maxSpeed;
  int total = forward + turn;
  int difference = forward - turn;

  if (forward >= 0) {
    if (turn > 0) {
      speeds.a = maximum;
      speeds.b = difference;
    }

    else if (turn < 0) {
      speeds.a = total;
      speeds.b = maximum;
    }
    else {
      speeds.a = forward;
      speeds.b = forward;
    }
  }
  else {
    if (turn >= 0) {
      speeds.a = total;
      speeds.b = -maximum;
    }

    else {
      speeds.a = -maximum;
      speeds.b = difference;
    }
  }

  return speeds;
}
