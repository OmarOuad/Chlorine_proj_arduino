#include <LiquidCrystal.h>
#include "GravityTDS.h"
#define TdsSensorPin A1
LiquidCrystal lcd(1,2,4,5,6,7);

GravityTDS gravityTds;
 
float temperature = 25,tdsValue = 0;
const int RELAY_PIN = A5; //controlling the pump through pin A5 on the arduino UNO card

void setup()
{
    //Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    lcd.begin(16,2);
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
}
 
void loop()
{
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    lcd.setCursor(0, 0);
    lcd.print("TDS Value:");
    lcd.setCursor(0, 1);
    lcd.print(tdsValue,0);
    lcd.print(" PPM");
    if(tdsValue<1500){
      digitalWrite(RELAY_PIN, HIGH);
    }
    if(tdsValue>1900){
      digitalWrite(RELAY_PIN, LOW);
    }
    delay(2000);
    lcd.clear();
}
