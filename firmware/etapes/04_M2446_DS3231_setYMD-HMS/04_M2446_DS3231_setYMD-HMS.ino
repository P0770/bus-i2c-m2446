//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 22 février 2024
//description: code permetant de regler l'heure dans la RTC DS3231
//---------------------------------------------------------------
//---------------------------------------------------------------
#include <DS3231.h>
#include <Wire.h>

DS3231 myRTC;

byte year = 24;
byte month = 9;
byte date = 20;
byte dOW = 5;
byte hour = 19;
byte minute = 20;
byte second = 0;


void setup() {
  // Start the serial port
  Serial.begin(9600);

  // Start the I2C interface
  Wire.begin();


  myRTC.setClockMode(false);  // set to 24h
  //setClockMode(true); // set to 12h

  myRTC.setYear(year);
  myRTC.setMonth(month);
  myRTC.setDate(date);
  myRTC.setDoW(dOW);
  myRTC.setHour(hour);
  myRTC.setMinute(minute);
  myRTC.setSecond(second);
}

void loop() {
 
}