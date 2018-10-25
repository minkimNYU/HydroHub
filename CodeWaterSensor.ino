#include "LiquidCrystal.h"
#include <Adafruit_GFX.h>    // Core graphics library

#include <Adafruit_ST7735.h> // Hardware-specific library

#include <SPI.h>





// For the breakout, you can use any 2 or 3 pins

// These pins will also work for the 1.8" TFT shield

#define TFT_CS     10

#define TFT_RST    9  // you can also connect this to the Arduino reset

                      // in which case, set this #define pin to -1!

#define TFT_DC     8



// Option 1 (recommended): must use the hardware SPI pins

// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be

// an output. This is much faster - also required if you want

// to use the microSD card (see the image drawing example)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);


// which pin to use for reading the sensor? can use any pin!
#define FLOWSENSORPIN 2

// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void setup() {
   Serial.begin(9600);
   Serial.print("Flow sensor test!");

   tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab

   uint16_t time = millis();
   tft.fillScreen(ST7735_BLACK);
   time = millis() - time;
   
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
   useInterrupt(true);
}

void testdrawtext(const char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void loop()                     // run over and over again
{ 
  //more info on water 
  Serial.print("Freq: "); Serial.println(flowrate);
  Serial.print("Pulses: "); Serial.println(pulses, DEC);
  

  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;

  //printing to serial after converting to mL
  Serial.print((1000*liters)); Serial.println(" MilliLiters");
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_RED);
  
  tft.setCursor(0,0);
  String myRead= String(liters*1000);
  tft.println("Ml Drunk:");
  tft.println(myRead);
  //testdrawtext(myRead.c_str(), ST7735_WHITE);
  
  delay(1000);
 
  delay(100);
}
