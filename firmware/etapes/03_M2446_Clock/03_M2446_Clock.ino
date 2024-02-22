//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 22 février 2024
//description: code permetant d'afficher l'heure de la RTC DS3231
//---------------------------------------------------------------
//---------------------------------------------------------------
#include <Wire.h>
#include <DS3231.h>

RTClib myRTC;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    delay(500);
    Serial.println("Nano Ready!");
}

void loop () {
    
    delay(1000);
    
    DateTime now = myRTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    
}