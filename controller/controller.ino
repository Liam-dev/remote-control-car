#include <Stepper.h>
#include <Ultrasonic.h>
#include <LiquidCrystal.h>

struct pair {
  int a;
  int b;
};

const int maxSpeed = 590;
const int stepsPerRevolution = 64; 
//const float quarterPi = 0.78539816339;
//const float sqrt2 = 1.41421356237;

//Stepper leftmotor(stepsPerRevolution, 2, 4, 3, 5);
//Stepper RightMotor(stepsPerRevolution, 6, 7, 8, 9);

unsigned long lastButtonPress;

LiquidCrystal lcd(8, 9, 10, 12, 11, 13);

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), buttonPressed, FALLING);

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Hello World!");
}

void loop() {
  // put your main code here, to run repeatedly:

  struct pair motorSpeeds = calculateMotorSpeeds(analogRead(A0), analogRead(A1));  
  lcd.clear();
  lcd.print(analogRead(A0));
  lcd.print(" ");
  lcd.print(analogRead(A1));
  lcd.setCursor(0, 1);
  lcd.print(motorSpeeds.a);
  lcd.print(" ");
  lcd.print(motorSpeeds.b);

  delay(500);
}



void buttonPressed(){
  lcd.clear();
  lcd.print("Button Pressed");
  Serial.println("Button Pressed");
}
