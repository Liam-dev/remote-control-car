#include <LiquidCrystal.h>
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

const int joystickPinX = A0;
const int joystickPinY = A1;

const static uint8_t radioId = 1;    // This radio's id.
const static uint8_t receiverId = 0; // Id of the radio we will transmit to.
const static uint8_t CEPin = 9;
const static uint8_t CSNPin = 10;

NRFLite radio;
RadioData radioData;

LiquidCrystal lcd(4, 3, 5, 7, 6, 8);

unsigned long lastButtonPress;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  //pinMode(2, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(2), buttonPressed, FALLING);

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Hello World!");

  if (!radio.init(radioId, CEPin, CSNPin))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
    
  radioData.radioId = radioId;
}

void loop() {
  // put your main code here, to run repeatedly:
  struct Pair input = getJoystickInput();
  
  if (radio.send(receiverId, &radioData, sizeof(radioData))) // Note how '&' must be placed in front of the variable name.
  {
      Serial.println("...Success");
  }
  else
  {
      Serial.println("...Failed");
  }
  
  delay(1000);
}

struct Pair getJoystickInput(){
  struct Pair input;
  input.a = analogRead(joystickPinX);
  input.b = analogRead(joystickPinY);
  return input;
}

void buttonPressed(){
  lcd.clear();
  lcd.print("Button Pressed");
  Serial.println("Button Pressed");
}
