// PVC Chair

#include <Arduino.h>
// https://github.com/olikraus/u8g2/wiki/u8x8reference
#include <U8g2lib.h>
#include <Wire.h>
// https://www.dimensionengineering.com/info/arduino
#include <SabertoothSimplified.h>
#include "MotorControl.h"

// Connections to make:
//   Arduino TX->1  ->  Sabertooth S1
//   Arduino GND    ->  Sabertooth 0V
//   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
SabertoothSimplified ST;

// https://www.amazon.com/gp/customer-reviews/RE5PIJFQ7K685/ref=cm_cr_getr_d_rvw_ttl?ie=UTF8&ASIN=B01HEBIJKK
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); // OLEDs without Reset of the Display

const int batteryPin = A0;

const int joystickUpperLeftToLowerRightPin = A2;
const int joystickLowerLeftToUpperRightPin = A3;

int joystickUpperLeftToLowerRightValue;
int joystickLowerLeftToUpperRightValue;
int threshold = 10;

MotorControl controller;

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  ST.motor(1, 0); // Stop.
  ST.motor(2, 0); // Stop.

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.clearDisplay();
  u8x8.drawString(0,0,"PVC Chair v2.0.0");

  // get initial settings for calibration
  joystickUpperLeftToLowerRightValue = analogRead(joystickUpperLeftToLowerRightPin);
  joystickLowerLeftToUpperRightValue = analogRead(joystickLowerLeftToUpperRightPin);

  // use initial values as zeros in MotorControl object
  controller.initialize(joystickUpperLeftToLowerRightValue, joystickLowerLeftToUpperRightValue, threshold);
}

void loop()
{
  char buff[16];
  
  // Battery Voltage
  // http://www.skillbank.co.uk/arduino/measure.htm
  // batteryPin is between 0-1023 that scales to 0-5V from voltage divider
  // voltage divider divides source voltage by 5
  int batterySensorValue = analogRead(batteryPin);
  float voltage = (batterySensorValue * 5.0 * 5.0) / 1023.0;

  sprintf(buff, "Sensor : %7d", batterySensorValue);
  u8x8.setCursor(0,2);
  u8x8.print(buff);

  char voltage_buff[7];
  dtostrf(voltage, 4, 1, voltage_buff);
  sprintf(buff, "Voltage: %7s", voltage_buff);
  u8x8.setCursor(0,3);
  u8x8.print(buff);
  
  // https://sciencing.com/determine-ah-12volt-battery-7733095.html
  int batteryPercentage = (int) ((voltage / 12.6) * 100);
  sprintf(buff, "Batt. %%: %6d%%", batteryPercentage);
  u8x8.setCursor(0,4);
  u8x8.print(buff);
    
  // Collect raw analog data (range: 0-1023)
  joystickUpperLeftToLowerRightValue = analogRead(joystickUpperLeftToLowerRightPin);
  joystickLowerLeftToUpperRightValue = analogRead(joystickLowerLeftToUpperRightPin);

  sprintf(buff, "Val ULLR: %6d", joystickUpperLeftToLowerRightValue);
  u8x8.setCursor(0,5);
  u8x8.print(buff);

  sprintf(buff, "Val LLUR: %6d", joystickLowerLeftToUpperRightValue);
  u8x8.setCursor(0,6);
  u8x8.print(buff);

  int motor1Command;
  int motor2Command;
  controller.translateJoystickToMotorCommands(joystickUpperLeftToLowerRightValue, joystickLowerLeftToUpperRightValue, motor1Command, motor2Command);

  ST.motor(1, motor1Command);
  ST.motor(2, motor2Command);

  /*
  ST.motor(1, 0);    // Stop.
  ST.motor(2, 0);    // Stop.

  ST.motor(1, 127);  // Go forward at full power.
  delay(2000);       // Wait 2 seconds.
  ST.motor(1, 0);    // Stop.
  delay(2000);       // Wait 2 seconds.
  ST.motor(1, -127); // Reverse at full power.
  delay(2000);       // Wait 2 seconds.
  ST.motor(1, 0);    // Stop.
  delay(2000);

  ST.motor(2, 127);  // Go forward at full power.
  delay(2000);       // Wait 2 seconds.
  ST.motor(2, 0);    // Stop.
  delay(2000);       // Wait 2 seconds.
  ST.motor(2, -127); // Reverse at full power.
  delay(2000);       // Wait 2 seconds.
  ST.motor(2, 0);    // Stop.
  delay(2000);
  */
}
