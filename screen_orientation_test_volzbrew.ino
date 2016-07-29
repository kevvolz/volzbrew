// UTFT_Textrotation_Demo (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the textrotation-functions.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <UTouch.h>

// Data wire is plugged into port 10 on the Arduino
#define ONE_WIRE_BUS 10

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// Uncomment the next line for Arduino 2009/Uno
//UTFT myGLCD(ITDB32S,19,18,17,16);   // Remember to change the model parameter to suit your display module!

// Uncomment the next line for Arduino Mega
//UTFT myGLCD(ITDB32S,38,39,40,41);   // Remember to change the model parameter to suit your display module!
UTFT myGLCD(ILI9341_16,38,39,40,41);


//INIT touchscreen
UTouch  myTouch( 6, 5, 4, 3, 2);


float tempf;
int x = 0;
int y = 0;
int count = 0;

void drawOutline()
{
      myGLCD.print("VOLZBREW", 0, 0);
    myGLCD.setColor(0, 0, 255);
    myGLCD.print("Temperature", 0, 16, 0);
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("VolzBrew", 319, 0, 90);
    myGLCD.setColor(255, 0, 0);
    myGLCD.print("VolzBrew", 319, 239, 180);
    myGLCD.setColor(255, 255, 0);
    myGLCD.print("Volzbrew", 0, 239, 270);

 //   myGLCD.setFont(SevenSegNumFont);
 //   myGLCD.setColor(0, 255, 0);
//    myGLCD.print("72", 90, 100, 45);
    myGLCD.setColor(0, 255, 255);
 //   myGLCD.print("72", 150, 50, 0);
}

void checkTemp()
{
   sensors.requestTemperatures(); // Send the command to get temperatures
   tempf = (sensors.getTempCByIndex(0)*1.8+32);
   myGLCD.printNumF(tempf, 2, 150, 50);
}

void setup()
{

//---------------------------------------------- Set PWM frequency for D6, D7 & D8 ---------------------------
 
TCCR4B = TCCR4B & B11111000 | B00000001;    // set timer 4 divisor to     1 for PWM frequency of 31372.55 Hz
//TCCR4B = TCCR4B & B11111000 | B00000010;    // set timer 4 divisor to     8 for PWM frequency of  3921.16 Hz
//TCCR4B = TCCR4B & B11111000 | B00000011;    // set timer 4 divisor to    64 for PWM frequency of   490.20 Hz
//TCCR4B = TCCR4B & B11111000 | B00000100;    // set timer 4 divisor to   256 for PWM frequency of   122.55 Hz
//TCCR4B = TCCR4B & B11111000 | B00000101;    // set timer 4 divisor to  1024 for PWM frequency of    30.64 Hz

//SETUP POWER AND GND FOR TEMP ON PINS 9-11

  pinMode(9, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(9, LOW);
  digitalWrite(11, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);


  //SCREEN INIT
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);

  //BUTTONS INIT
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  // Start up the temp library
  sensors.begin();
  drawOutline();
}

void loop()
{
      if (myTouch.dataAvailable())
    {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
    myGLCD.setColor(255, 127, 0);
    myGLCD.print("Touch", 140, 120, 0);
      
      
      while(myTouch.dataAvailable());
  //      myTouch.read();
        myGLCD.print("     ", 140, 120, 0);
        
        if (x < 120 & y < 80)
        {
            digitalWrite(8, LOW);
                myGLCD.print("OFF   ", 140, 120, 0);          
        }
      if (x > 120 & y < 80)
        {
            analogWrite(8, 27);
                myGLCD.print("LOW   ", 140, 120, 0);          
        }
      if (x < 120 & y > 80)
        {
            analogWrite(8, 85);
                myGLCD.print("MEDIUM", 140, 120, 0);          
        }
      if (x > 120 & y > 80)
        {
            digitalWrite(8, HIGH);
                myGLCD.print("HIGH   ", 140, 120, 0);          
        }
    }


    
     if (count == 10000)
     {
      checkTemp();
      count = 0;
     }
     else {
      count ++;
     }
//      delay(1000);
}


void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
