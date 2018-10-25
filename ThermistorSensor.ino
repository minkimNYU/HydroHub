#include <math.h>
#include <Time.h>
#include <Adafruit_GFX.h>    // Core graphics library

#include <Adafruit_ST7735.h> // Hardware-specific library

#include <SPI.h>

#define TFT_CS     10

#define TFT_RST    9  // you can also connect this to the Arduino reset

                      // in which case, set this #define pin to -1!

#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

double Thermistor(int RawADC) 
{
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1))); 
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;          
 Temp = (Temp * 9.0)/ 5.0 + 32.0; 
 return Temp;
}
unsigned long Time;
bool run;
void setup() {
 Serial.begin(9600);
 tft.initR(INITR_144GREENTAB); 
 run = false;
 Serial.print("Time (s), ");
 Serial.println("Temperature (F)");
}

void testdrawtext(const char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void loop() 
{
            
  int val; 
  int times;
  int second;                 
  double temp;            
  val=analogRead(0);      
  temp=Thermistor(val); 
  times = millis()/1000;
  second= times/1000;  
  tft.fillScreen(ST7735_BLACK);
  String myRead= String(temp);
  testdrawtext(myRead.c_str(), ST7735_WHITE);
  Serial.print(times);
  Serial.print(", ");
  Serial.println(temp);  
  delay(5000);            
}

