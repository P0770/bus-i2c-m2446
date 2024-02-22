//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 22 février 2024
//description: code permetant de tester et afficher les température récupérer par le capteurs
//---------------------------------------------------------------
//---------------------------------------------------------------
#include <Wire.h>
#include "Adafruit_MCP9808.h"

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  tempsensor.setResolution(3);
  tempsensor.begin(0x18);
  tempsensor.wake();
}

void loop() {
  Serial.print("Temperature: ");
  Serial.print(tempsensor.readTempC());
  Serial.println(" *C");
  Serial.println("");
  delay(200);
}
