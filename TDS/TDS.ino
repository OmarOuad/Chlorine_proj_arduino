#include <EEPROM.h>
#include "GravityTDS.h"
#include <LiquidCrystal.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
#define ONE_WIRE_BUS 7
#define TdsSensorPin A1
 
OneWire oneWire(ONE_WIRE_BUS); 
GravityTDS gravityTds;
 
DallasTemperature sensors(&oneWire);
 
float tdsValue = 0;
 
void setup()
{
    Serial.begin(115200);
    lcd.begin(16,2);
    sensors.begin();
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(12.0);  //voltage
    gravityTds.setAdcRange(4096); 
    gravityTds.begin();  //initialization
}
 
void loop()
{
    sensors.requestTemperatures();
 
    gravityTds.setTemperature(sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    Serial.print("Temperature is: "); 
    Serial.print(sensors.getTempCByIndex(0));
    

    //imprimer les valeurs sur l'écran du TDS (total dissolved solids)
    lcd.setCursor(0, 0);
    lcd.print("TDS: ");
    lcd.print(tdsValue,0);
    lcd.print(" PPM");
 
    //imprimer les valeurs sur l'écran de la température
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print(" C");
    
    
    delay(1500);
    lcd.clear();
}