#include "SPI.h" 
#include "NRFLite.h"
#include "LiquidCrystal.h"

const int buzzerPin = 2;

int distance, temperature;
int period = 500, time_now = 0;

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

bool tonePlaying;

NRFLite _radio;
struct Pair joystick;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.begin(9600);

  lcd.begin(16, 2);
  
  
  _radio.init(11, 9, 10); // Set radio to Id = 1, along with the CE and CSN pins
}

void loop()
{
  temperature = _data.a;
  distance = _data.b;

  obstacleDetection(distance)

   
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
        lcd.clear();
        lcd.print(distance);
    }
   delay(50);
   
}

// Reads the values from the joystick
struct Pair getJoystickInput(){
  struct Pair input;
  input.a = analogRead(joystickPinX);
  input.b = analogRead(joystickPinY);
  return input;
}

void obstacleDetection(int distance){
  if (distance < 20 && distance > 5)
  {
    if (sin(millis() * 10) > 0){
      tone(buzzerPin, distance*40);
    }
    else{
      noTone(buzzerPin);
    }
  }
  else if(distance < 5)
  {
   tone(buzzerPin, 2000);
  }
  else
  {
    noTone(buzzerPin);
  }
}
