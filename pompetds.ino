
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>    //include arduinos i2c library
#include <sequencer2.h> //imports a 2 function sequencer 
#include <Ezo_i2c_util.h> //brings in common print statements
#include <EEPROM.h>
#include "GravityTDS.h"
#include <LiquidCrystal.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

Ezo_board PMP_UP = Ezo_board(8, "PMP");                   //create a pump circuit object, whose address is ç and name is "PMP_UP". This pump dispenses pH up solution.

#define ONE_WIRE_BUS 7
#define TdsSensorPin A1
 
OneWire oneWire(ONE_WIRE_BUS); 
GravityTDS gravityTds;
 
DallasTemperature sensors(&oneWire);
 
float tdsValue = 0;
float volume = 5000;

void main(); 

Sequencer2 Seq(&step1, 1000, &step2, 0); //calls the steps in sequence with time in between them

void setup() {
  Wire.begin();                           //start the I2C
  Serial.begin(9600);                     //start the serial communication to the computer
  Seq.reset();                            //initialize the sequencer
  Serial.begin(115200);
  lcd.begin(16,2);
  sensors.begin();
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(12.0);  //voltage
  gravityTds.setAdcRange(4096); 
  gravityTds.begin();  //initialization
}

void loop() {
  Seq.run();                              //run the sequncer to do the polling
}


void main(){
  
  sensors.requestTemperatures();
 
  gravityTds.setTemperature(sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value 

  Serial.print("    ");
  lcd.setCursor(0, 0);
  lcd.print("TDS: ");
  lcd.print(tdsValue,0);
  lcd.print(" PPM");

  if (tdsValue  >= 1000) {                            //test condition against TDS reading
    PMP.send_cmd_with_num("d,", 6*volume + 750);                  //if condition is true, send command to turn on pump (called PMP) and dispense chlorine. Pump turns clockwise.
  }
  else {
    PMP.send_cmd("x");                                 //if condition is false, send command to turn off pump (called PMP)
   }
  Serial.println();
}