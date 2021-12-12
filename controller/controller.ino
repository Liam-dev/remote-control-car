#include "SPI.h"
#include "NRFLite.h"
#include "LiquidCrystal.h"

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

const static uint8_t radioId = 11;    // This radio's id.
const static uint8_t receiverId = 0; // Id of the radio we will transmit to.
const static uint8_t CEPin = 9;
const static uint8_t CSNPin = 10;

Pair _data;

LiquidCrystal lcd(4, 3, 5, 7, 6, 8);

unsigned long lastButtonPress;

NRFLite _radio;
struct Pair joystick;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Hello World!");
  
  _radio.init(11, 9, 10); // Set radio to Id = 1, along with the CE and CSN pins
}

void loop()
{
   //_data++;
   struct Pair joystick = getJoystickInput();
   //Serial.println(joystick);
   
   
   //Serial.print(joystick.a);
   //Serial.print(" ");
   //Serial.println(joystick.b);
   _radio.send(10, &joystick, sizeof(joystick)); // Send data to the radio with Id = 0
   while(_radio.hasData())
    {
        //Serial.println("success");
        _radio.readData(&_data);
        Serial.println(_data.a);
    }
   delay(50);
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
