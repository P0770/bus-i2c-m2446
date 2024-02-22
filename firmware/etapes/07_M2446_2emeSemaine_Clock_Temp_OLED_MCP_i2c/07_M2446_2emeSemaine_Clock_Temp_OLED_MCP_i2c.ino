//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 22 février 2024
//description: code réalisé apres la deuxième semaine avec système de prog1-5 et utilisation de : tempTC74 / OLEDSSD1351 / ClockDS3231 / AdresseI2C
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//Bibliothèque I2C
#include <Wire.h>
//---------------------------------------------------------------------------------------------------------
//bibliothèque Capteur température
#include "Adafruit_MCP9808.h"
#include <DS3231.h>
//---------------------------------------------------------------------------------------------------------
//bibliothèque Ecran
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#define WIRE Wire
//---------------------------------------------------------------------------------------------------------
//  Numerique pin bouton define
#define PIN_UP 8
#define PIN_DOWN 9
#define PIN_ENTER 10
#define PIN_EXIT 11
#define PIN_HP 12
//---------------------------------------------------------------------------------------------------------
//Ecran define
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
//pin ecran
#define MOSI_PIN 6
#define SCLK_PIN 5
#define DC_PIN 4
#define RST_PIN 3
#define CS_PIN 2
// couleurs
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
//---------------------------------------------------------------------------------------------------------
// variables pour bouton
bool bouton_UP = 0;
bool bouton_UP2 = 0;
bool bouton_DOWN = 0;
bool bouton_DOWN2 = 0;
bool bouton_ENTER = 0;
bool bouton_ENTER2 = 0;
bool bouton_EXIT = 0;
bool bouton_EXIT2 = 0;
//---------------------------------------------------------------------------------------------------------
//variables pour etats
int prog = 0;
int incr = 0;
bool enterstatus = 0;
bool affichageENTER = 0;
bool affichage = 0;
bool EnterClef = 0;
bool EnterClef5 = 0;
//---------------------------------------------------------------------------------------------------------
//variables tone
int sound = 0;      // en htz
int frequence = 0;  // en ms
//---------------------------------------------------------------------------------------------------------
//variable pour millis et clignotememnt
unsigned long tmillis = 0;
unsigned long tmillis2 = 0;
unsigned long tmillis3 = 0;
int timeled = 600;
int timetemp = 1000;
int ti2c = 1000;
//---------------------------------------------------------------------------------------------------------
//variable pour temperature
float temperature = 0;
int temperature1 = 0;
int temperature2 = 0;
bool affichagetempW = 0;
bool affichagetempB = 0;
//---------------------------------------------------------------------------------------------------------
// variable pour time
int Hour = 0;
int Hour2 = 0;
int Minute = 0;
int Minute2 = 0;
int Second = 0;
int Second2 = 0;
int Year = 0;
int Month = 0;
int Day = 0;


int nDevices = 0;
int ligneadresse = 0;

//---------------------------------------------------------------------------------------------------------
//Setup Ecran
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
//---------------------------------------------------------------------------------------------------------
//Setup Capteur température
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
RTClib myRTC;
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);  // setup pour arduino MEGA
  Wire.begin();        // setup pour I2C
                       //---------------------------------------------------------------------------------------------------------
  while (!Serial)
    delay(10);
  //---------------------------------------------------------------------------------------------------------
  tempsensor.setResolution(2);  // resolution de la température captée
  tempsensor.begin(0x18);       // adresse du capteurs de température
  tempsensor.wake();            // activation du capteurs
  //---------------------------------------------------------------------------------------------------------
  tft.begin();            // setup ecran
  tft.fillScreen(BLACK);  // afficahge ecran tout noir
  //---------------------------------------------------------------------------------------------------------
  pinMode(PIN_UP, INPUT);     // assignations des inputs
  pinMode(PIN_DOWN, INPUT);   // assignations des inputs
  pinMode(PIN_ENTER, INPUT);  // assignations des inputs
  pinMode(PIN_EXIT, INPUT);   // assignations des inputs
  pinMode(PIN_HP, OUTPUT);    // assignations des inputs
  //---------------------------------------------------------------------------------------------------------
  prog = 1;            // mise de prog a 1
  affichageENTER = 1;  // affichage de enter on
  affichage = 1;       // affichage des bande prog
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void loop() {
  DateTime now = myRTC.now();  // fonction time dans une variable
  //---------------------------------------------------------------------------------------------------------
  tmillis = millis();
  // --------------------------------------------------------------------------------------------------------
  bouton_UP = digitalRead(PIN_UP);        // 1
  bouton_DOWN = digitalRead(PIN_DOWN);    // 2
  bouton_ENTER = digitalRead(PIN_ENTER);  // 3
  bouton_EXIT = digitalRead(PIN_EXIT);    // 4
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  if ((bouton_UP == 1) && (bouton_UP2 == 0) && (enterstatus == 0)) {  // detection de front montant pour bouton 1
    if (prog > 1) {                                                   // limite
      prog--;                                                         // programme lié au switch et decr de prog
      affichage = 1;                                                  // permet de faire l'affichage 1 fois car remise a 0 direct
    }
  }
  bouton_UP2 = bouton_UP;
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_DOWN == 1) && (bouton_DOWN2 == 0) && (enterstatus == 0)) {  // detection de front montant pour bouton 2
    if (prog < 5) {                                                       // limit
      prog++;                                                             // programme lié au switch et incr de prog
      affichage = 1;                                                      // permet de faire l'affichage 1 fois car remise a 0 direct
    }
  }
  bouton_DOWN2 = bouton_DOWN;
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_ENTER == 1) && (bouton_ENTER2 == 0) && (enterstatus == 0)) {  // detection de front montant pour bouton 3
    enterstatus = 1;                                                        // confirme letat enter pour empeche les changement de prog
    affichageENTER = 0;                                                     // desactive laffichage des choix de progamme
    EnterClef = 1;                                                          // variable utilisé au débuts des switch/case pour initialisé les millis et autres variables
    Serial.println("-------------------------------------------------------------------");
  }
  bouton_ENTER2 = bouton_ENTER;
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_EXIT == 1) && (bouton_EXIT2 == 0) && (enterstatus == 1)) {  // detection de front montant pour bouton 4
    enterstatus = 0;                                                      // confirme letat enter pour empeche les changement de prog
    affichageENTER = 1;                                                   // active laffichage des choix de progamme
    lcdExit();
    Serial.println("-------------------------------------------------------------------");  // raffraichit l'ecran en noir
  }
  bouton_EXIT2 = bouton_EXIT;
  //---------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  switch (prog) {
    // --------------------------------------------------------------------------------------------------------
    case 1:
      if (affichage == 1) {
        lcdProg1();     // affichage de la bande "programme 1"
        affichage = 0;  // remise a 0 pour le réalisé 1 fois
      }
      if (enterstatus == 1) {                    // etats de programme lorsque enter est presse
                                                 //--------------------------------------------------------------------------------------------------------
        if (EnterClef == 1) {                    // action réalisé 1 fois
          temperature = tempsensor.readTempC();  // varriable float qui prend température
          temperature1 = round(temperature);     // arrondi de variable Float et implementation dans variable int pour avoir valeur arrondie a l'unité
          tft.setCursor(0, 20);                  // selection des pixels qu'on vont être utilisé
          tft.setTextSize(1);                    // choix de la taille de police
          tft.setTextColor(WHITE);               // choix de la couleur
          tft.print("Temperature:");             // impression sur ecran
          tft.setCursor(30, 30);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);
          tft.print("*C");
          tft.setCursor(0, 30);
          tft.setTextSize(2);
          tft.setTextColor(WHITE);
          tft.print(temperature1);
          temperature2 = temperature1;  // température mémoire
          tmillis2 = tmillis;
          EnterClef = 0;
        }
        // --------------------------------------------------------------------------------------------------------
        if (temperature2 != temperature1) {  // détection de différence entre nouvelle température et température mémoire
          tft.setCursor(0, 30);
          tft.setTextSize(2);
          tft.setTextColor(BLACK);
          tft.print(temperature2);
          delay(100);
          tft.setCursor(0, 30);
          tft.setTextSize(2);
          tft.setTextColor(WHITE);
          tft.print(temperature1);
          temperature2 = temperature1;
          Serial.print("Temperature: ");
          Serial.print(temperature1);
          Serial.println(" *C");
          Serial.println("");
        }
        // --------------------------------------------------------------------------------------------------------
        if ((tmillis > tmillis2 + timetemp) && (temperature2 == temperature1)) {  // actualisation de la température toutes les secondes si et seulement si la mémoire est actualisé
          temperature = tempsensor.readTempC();
          temperature1 = round(temperature);
          tmillis2 = tmillis;
        }
      } else {
        Serial.println("Prog 1 Température");
      }
      break;
      // --------------------------------------------------------------------------------------------------------
    case 2:
      if (affichage == 1) {
        lcdProg2();  // affichage de la bande "programme 2"
        affichage = 0;
      }
      if (enterstatus == 1) {
        Hour = now.hour();      // actualisation de l'heure a chaque cycle arduino
        Minute = now.minute();  // actualisation de la minute a chaque cycle arduino
        Second = now.second();  // actualisation de la seconde a chaque cycle arduino
        // --------------------------------------------------------------------------------------------------------
        if (EnterClef == 1) {
          tft.setCursor(0, 20);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);
          tft.print("Heure et Date:");
          Year = now.year();    // actualisation de  l'anne 1 seule fois au debut du programme 2
          Month = now.month();  // actualisation du mois 1 seule fois au debut du programme 2
          Day = now.day();      // actualisation du jour 1 seule fois au debut du programme 2
          tft.setCursor(0, 50);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);
          tft.print(Year);  // impression de lannée 1 fois sans actualisation au debut du programme 2
          tft.print("/");
          tft.print(Month);  // impression de lannée 1 fois sans actualisation au debut du programme 2
          tft.print("/");
          tft.print(Day);  // impression de lannée 1 fois sans actualisation au debut du programme 2
          Hour = now.hour();
          Minute = now.minute();
          Second = now.second();
          tft.setCursor(0, 40);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);
          tft.print(Hour);  // premiere affichage de lheure avant actualisation
          tft.print(":");
          tft.print(Minute);  // premiere affichage de la minute avant actualisation
          tft.print(":");
          tft.print(Second);  // premiere affichage de la seconde avant actualisation
          Hour2 = Hour;       // memoire heure
          Minute2 = Minute;   // memoire minute
          Second2 = Second;   // memoire seconde
          EnterClef = 0;      //reset de la variable
        }
        // --------------------------------------------------------------------------------------------------------
        if (Hour2 != Hour) {  // detection de difference avec actualisation et memoire
          tft.setCursor(0, 40);
          tft.setTextSize(1);
          tft.setTextColor(BLACK);  // affichage effacé pour réecrire
          tft.print(Hour2);
          tft.setCursor(0, 40);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);  // affichage réecrit
          tft.print(Hour);
          Hour2 = Hour;
        }
        if (Minute2 != Minute) {  // detection de difference avec actualisation et memoire
          tft.setCursor(12, 40);
          tft.setTextSize(1);
          tft.setTextColor(BLACK);  // affichage effacé pour réecrire
          tft.print(Minute2);
          tft.setCursor(12, 40);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);  // affichage réecrit
          tft.print(Minute);
          Minute2 = Minute;
        }
        if (Second2 != Second) {  // detection de difference avec actualisation et memoire
          tft.setCursor(36, 40);
          tft.setTextSize(1);
          tft.setTextColor(BLACK);  // affichage effacé pour réecrire
          tft.print(Second2);
          tft.setCursor(36, 40);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);  // affichage réecrit
          tft.print(Second);
          Second2 = Second;
        }
        // --------------------------------------------------------------------------------------------------------
        Serial.println("Temps & Date");
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
      } else {
        Serial.println("Prog 2 Temps & Date");
      }
      break;
      // --------------------------------------------------------------------------------------------------------
      // --------------------------------------------------------------------------------------------------------
    case 3:
      if (affichage == 1) {
        lcdProg3();  // affichage de la bande "programme 3"
        affichage = 0;
      }
      if (enterstatus == 1) {
        Serial.println("Prog 3 set");
      } else {
        Serial.println("Prog 3");
      }
      break;
      // --------------------------------------------------------------------------------------------------------
    case 4:
      if (affichage == 1) {
        lcdProg4();  // affichage de la bande "programme 4"
        affichage = 0;
      }
      if (enterstatus == 1) {
        Serial.println("Prog 4 set");
      } else {
        Serial.println("Prog 4");
      }
      break;
      // --------------------------------------------------------------------------------------------------------
    case 5:
      if (affichage == 1) {
        lcdProg5();  // affichage de la bande "programme 5"
        affichage = 0;
      }
      if (EnterClef == 1) {
        tmillis3 = tmillis;
        EnterClef = 0;
        EnterClef5 = 1;
        ligneadresse = 50;
      }

      if ((enterstatus == 1) && (EnterClef5 == 1)) {

        byte error, address;
        tft.setCursor(0, 20);
        tft.setTextSize(1);
        tft.setTextColor(WHITE);
        tft.print("Scanning...");


        for (address = 1; address < 127; address++) {
          // The i2c_scanner uses the return value of
          // the Write.endTransmisstion to see if
          // a device did acknowledge to the address.
          WIRE.beginTransmission(address);
          error = WIRE.endTransmission();



          if (error == 0) {
            tft.setCursor(0, 30);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("I2C trouve a");
            tft.setCursor(0, 40);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("l'address :");
            tft.setCursor(93, ligneadresse);

            if (address < 16)
              tft.print("0");
            tft.setCursor(80, ligneadresse);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("0x");


            if (address < 16)
              tft.setCursor(96, ligneadresse);
            else
              tft.setCursor(93, ligneadresse);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print(address, HEX);
            ligneadresse = ligneadresse + 10;

            nDevices++;
          } else if (error == 4) {
            tft.setCursor(0, 60);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("Erreur inconnu a");
            tft.setCursor(0, 70);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("l'address :");
            tft.setCursor(0, 80);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("0x");
            if (address < 16)
              tft.setCursor(16, 80);
            else
              tft.setCursor(13, 80);
            tft.print("0");
            tft.setCursor(2, 31);
            tft.print(address, HEX);
          }
        }
        if (nDevices == 0) {

          tft.setCursor(0, 30);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);
          tft.print(" I2C introuvable\n");
          EnterClef5 = 0;
          nDevices = 0;
        } else {



          tft.setCursor(0, 90);
          tft.setTextSize(1);
          tft.setTextColor(WHITE);
          tft.print("Scan fini.\n");
          EnterClef5 = 0;
          nDevices = 0;
        }
      } else {
        Serial.println("Prog 5");
      }
      break;
  }
  // --------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  if (affichageENTER == 1) {  // affichage de l'info enter
    tft.setCursor(0, 60);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Appuyez sur ENTER ");
    tft.setCursor(0, 70);
    tft.print("pour validez la ");
    tft.setCursor(0, 80);
    tft.print("selection.");
  } else {  // supression de laffichage de l'info enter
    tft.setCursor(0, 60);
    tft.setTextColor(BLACK);
    tft.setTextSize(1);
    tft.print("Appuyez sur ENTER ");
    tft.setCursor(0, 70);
    tft.print("pour validez la ");
    tft.setCursor(0, 80);
    tft.print("selection.");
  }
}
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void lcdProg1(void) {  // affichage bande et texte "programe 1"
  static const uint16_t PROGMEM colors[] = { RED };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 1");
  }
}
// --------------------------------------------------------------------------------------------------------
void lcdProg2(void) {  // affichage bande et texte "programe 2"
  static const uint16_t PROGMEM colors[] = { BLUE };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 2");
  }
}
// --------------------------------------------------------------------------------------------------------
void lcdProg3(void) {  // affichage bande et texte "programe 3"
  static const uint16_t PROGMEM colors[] = { GREEN };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 3");
  }
}
// --------------------------------------------------------------------------------------------------------
void lcdProg4(void) {  // affichage bande et texte "programe 4"
  static const uint16_t PROGMEM colors[] = { MAGENTA };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 4");
  }
}
// --------------------------------------------------------------------------------------------------------
void lcdProg5(void) {  // affichage bande et texte "programe 5"
  static const uint16_t PROGMEM colors[] = { CYAN };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 5");
  }
}
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void lcdExit(void) {  // Rénitialisation de l'ecran tout noir et affichage de la bande programme
  tft.fillScreen(BLACK);
  affichage = 1;
}