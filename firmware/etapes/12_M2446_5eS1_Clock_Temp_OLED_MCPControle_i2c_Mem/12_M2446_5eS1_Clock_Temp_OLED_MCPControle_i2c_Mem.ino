//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 4 Mars 2024
//description: code Réalisé avec implimentation de toute les composantes sur le cahier des charges sauf de la memoire externe pour la validation finale
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
//---------------------------------------------------------------------------------------------------------
//Bibliothèque MCP
#include <DFRobot_MCP23017.h>
//---------------------------------------------------------------------------------------------------------
//Bibliothèque EEPROM
#include <EEPROM.h>
//---------------------------------------------------------------------------------------------------------
#define WIRE Wire
//---------------------------------------------------------------------------------------------------------
//  Numerique pin bouton define
#define SWITCH_UP 8
#define SWITCH_DOWN 9
#define SWITCH_ENTER 10
#define SWITCH_EXIT 11
#define SWITCH_HP 7
//---------------------------------------------------------------------------------------------------------
//Ecran define
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
//pin ecran
#define MOSI_PIN 2
#define SCLK_PIN 3
#define DC_PIN 4
#define RST_PIN 5
#define CS_PIN 6
// couleurs
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xEB60
//---------------------------------------------------------------------------------------------------------
// variables pour memoire
int AdresseTemperature = 1;  // adresse mise a 9
int AdresseGPB = 40;         // adresse mise a 7
int AdresseGPA = 30;         // adresse mise a 8
int AdresseClock = 15;       // adresse mise a 5 et 6 en proposant 2 cellule de memoire
int AdresseInput = 60;       // adresse mise a 100
bool timeMemoire = 0;
const int maxInputSize = 80;  // Définir la taille maximale de la chaîne de caractères
//_____________________________________________________________________
struct StructureTemp {
  float ProfilTemperature;
};
//_____________________________________________________________________
struct StructureClock {
  int ClockYear;
  int ClockMonth;
  int ClockDay;
  int ClockHour;
  int ClockMinute;
  int ClockSecond;
};
//_____________________________________________________________________
struct StructureGPA {
  bool etat_GPA0_mem;
  bool etat_GPA1_mem;
  bool etat_GPA2_mem;
  bool etat_GPA3_mem;
  bool etat_GPA4_mem;
  bool etat_GPA5_mem;
  bool etat_GPA6_mem;
  bool etat_GPA7_mem;
};
struct StructureGPB {
  bool etat_GPB0_mem;
  bool etat_GPB1_mem;
  bool etat_GPB2_mem;
  bool etat_GPB3_mem;
  bool etat_GPB4_mem;
  bool etat_GPB5_mem;
  bool etat_GPB6_mem;
  bool etat_GPB7_mem;
};
//_____________________________________________________________________
struct StructureInput {
  char ProfilInput[maxInputSize];
};
//_____________________________________________________________________
int MenuProg4GP = 0;
int MenuProg4GP_memoire = 0;
int printOnceMenuProg4GP = 0;
int Cercle_X_Prog4_GP = 10;
int Cercle_Y_Prog4_GPA = 0;
int Cercle_Y_Prog4_GPB = 0;
int Bulle_Y_Prog4_GPA = 71;
int Bulle_Y_Prog4_GPB = 93;
int Text_Y_Prog4_GPA = 73;
int Text_Y_Prog4_GPB = 95;
int Coordonne_X_TextGP = 10;
int TextGP = 10;
bool on_off_MenuProg4GP = 0;
int decalage_text_GP = 11;
int decalage_cercle_GP = 14;
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
bool startend = 0;
bool enterstatus = 0;
bool affichage = 0;
bool EnterClef = 0;
bool EnterClef5 = 0;
int MenuProg3GPB = 0;
int MenuProg3GPB_memoire = 0;
//---------------------------------------------------------------------------------------------------------
//variables tone
int sound = 0;      // en htz
int frequence = 0;  // en ms
//---------------------------------------------------------------------------------------------------------
//variable pour millis et clignotememnt
unsigned long tmillis = 0;
unsigned long tmillis2 = 0;
unsigned long tmillis3 = 0;
unsigned long tmillis_checkI2C = 0;
unsigned long tmillis_3sec = 0;
unsigned long tmillisstart = 0;
bool tmillistartset = 0;
int timeled = 600;
int timetemp = 1000;
int ti2c = 1000;
bool alarm = 0;
bool alarm_sound = 0;
//---------------------------------------------------------------------------------------------------------
//variable programe 1 température
float temperature = 0;
int temperature1 = 0;
int temperature2 = 0;
float temperature_float = 0;
int temperature_int = 0;
int temperature_int2 = 0;
bool affichagetempW = 0;
bool affichagetempB = 0;
//---------------------------------------------------------------------------------------------------------
// variable programe 2 heure
int Hour = 0;
int HourMem = 0;
int Minute = 0;
int MinuteMem = 0;
int Second = 0;
int SecondMem = 0;
int Year = 0;
int Month = 0;
int Day = 0;
//---------------------------------------------------------------------------------------------------------
//variables programe 3 affichage GPA GPB
bool printOnceMenuProg3GPB = 0;
bool on_off_MenuProg3GPB = 0;
// varaible pour stocker coordonées pour tft.print les texte et cercles des GPA et GPB
int TextGPA = 20;
int Coordonne_X_TextGPB = 90;
int Cercle_X_Prog3_GPA = 10;
int Cercle_X_Prog3_GPB = 80;
int Cercle_Y_Prog3_GPA = 0;
int Cercle_Y_Prog3_GPB = 0;
//memoires d'etats
int Bulle_Y_Prog3_GPB = 0;
int Bulle_Y_Prog3_GPB_Mem = 0;
int Text_Y_Prog3_GPB = 0;
int Text_Y_Prog3_GPB_Mem = 0;
// variables bouton test GPA
bool GPA0 = 0;
bool GPA1 = 0;
bool GPA2 = 0;
bool GPA3 = 0;
bool GPA4 = 0;
bool GPA5 = 0;
bool GPA6 = 0;
bool GPA7 = 0;
//
bool GPA0_2 = 0;
bool GPA1_2 = 0;
bool GPA2_2 = 0;
bool GPA3_2 = 0;
bool GPA4_2 = 0;
bool GPA5_2 = 0;
bool GPA6_2 = 0;
bool GPA7_2 = 0;
// variables bouton test GPA
bool etat_GPA0 = 0;
bool etat_GPA1 = 0;
bool etat_GPA2 = 0;
bool etat_GPA3 = 0;
bool etat_GPA4 = 0;
bool etat_GPA5 = 0;
bool etat_GPA6 = 0;
bool etat_GPA7 = 0;
//
bool etat_GPB0 = 0;
bool etat_GPB1 = 0;
bool etat_GPB2 = 0;
bool etat_GPB3 = 0;
bool etat_GPB4 = 0;
bool etat_GPB5 = 0;
bool etat_GPB6 = 0;
bool etat_GPB7 = 0;
//---------------------------------------------------------------------------------------------------------
//variable programe 5 Affichage adresse i2c
int nDevices = 0;  // variable permettant de compter le nmbr d'escalves sur I2C
int nDevices2 = 3;
int Adresse_Y_Prog5 = 0;  // varaible pour stocker coordonées pour tft.print les adresse i2c
//---------------------------------------------------------------------------------------------------------
char InputOperateurCharArray[maxInputSize];
String InputOperateur;
String InputOperateur2;
bool WriteInput = 0;
bool finsetprog6 = 0;
//Setup Ecran
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
//---------------------------------------------------------------------------------------------------------
//Setup Capteur température
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
RTClib myRTC;
//---------------------------------------------------------------------------------------------------------
//Setup MCP
DFRobot_MCP23017 mcp(Wire, 0x27);
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);  // setup pour arduino MEGA
  Wire.begin();        // setup pour I2C
  //---------------------------------------------------------------------------------------------------------
  tempsensor.setResolution(2);  // resolution de la température captée
  tempsensor.begin(0x18);       // adresse du capteurs de température
  tempsensor.wake();            // activation du capteurs
  //---------------------------------------------------------------------------------------------------------
  tft.begin();            // setup ecran
  tft.fillScreen(BLACK);  // affichage ecran tout noir
  //---------------------------------------------------------------------------------------------------------
  while (!Serial)
    delay(10);
  //---------------------------------------------------------------------------------------------------------
  byte error, address;
  for (address = 1; address < 127; address++) {  // fonction permetant de tester toute les addresse possible
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();
    if (error == 0)  // si une de ces adresses repond ce sera avec un 0 du coup l'adresse sera print
      nDevices++;
  }
  if (nDevices < 3)
    alarm = 1;
  nDevices = 0;
  //---------------------------------------------------------------------------------------------------------
  pinMode(SWITCH_UP, INPUT);     // assignations des inputs
  pinMode(SWITCH_DOWN, INPUT);   // assignations des inputs
  pinMode(SWITCH_ENTER, INPUT);  // assignations des inputs
  pinMode(SWITCH_EXIT, INPUT);   // assignations des inputs
  pinMode(SWITCH_HP, OUTPUT);    // assignations des inputs
  //---------------------------------------------------------------------------------------------------------
  //pin GPA et GPB
  mcp.pinMode(mcp.eGPB0, OUTPUT);
  mcp.pinMode(mcp.eGPB1, OUTPUT);
  mcp.pinMode(mcp.eGPB2, OUTPUT);
  mcp.pinMode(mcp.eGPB3, OUTPUT);
  mcp.pinMode(mcp.eGPB4, OUTPUT);
  mcp.pinMode(mcp.eGPB5, OUTPUT);
  mcp.pinMode(mcp.eGPB6, OUTPUT);
  mcp.pinMode(mcp.eGPB7, OUTPUT);

  mcp.pinMode(mcp.eGPA0, INPUT);
  mcp.pinMode(mcp.eGPA1, INPUT);
  mcp.pinMode(mcp.eGPA2, INPUT);
  mcp.pinMode(mcp.eGPA3, INPUT);
  mcp.pinMode(mcp.eGPA4, INPUT);
  mcp.pinMode(mcp.eGPA5, INPUT);
  mcp.pinMode(mcp.eGPA6, INPUT);
  mcp.pinMode(mcp.eGPA7, INPUT);
  //---------------------------------------------------------------------------------------------------------
  // pin Sortie GPB mis a 0
  mcp.digitalWrite(mcp.eGPB0, LOW);
  mcp.digitalWrite(mcp.eGPB1, LOW);
  mcp.digitalWrite(mcp.eGPB2, LOW);
  mcp.digitalWrite(mcp.eGPB3, LOW);
  mcp.digitalWrite(mcp.eGPB4, LOW);
  mcp.digitalWrite(mcp.eGPB5, LOW);
  mcp.digitalWrite(mcp.eGPB6, LOW);
  mcp.digitalWrite(mcp.eGPB7, LOW);
  affichage = 1;  // affichage des bande prog
  timeMemoire = 1;
  tmillistartset = 1;
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void loop() {
  // fonction time dans une variable
  DateTime now = myRTC.now();
  //---------------------------------------------------------------------------------------------------------
  tmillis = millis();
  // --------------------------------------------------------------------------------------------------------
  bouton_UP = digitalRead(SWITCH_UP);
  bouton_DOWN = digitalRead(SWITCH_DOWN);
  bouton_ENTER = digitalRead(SWITCH_ENTER);
  bouton_EXIT = digitalRead(SWITCH_EXIT);
  // --------------------------------------------------------------------------------------------------------
  GPA0 = mcp.digitalRead(mcp.eGPA0);
  GPA1 = mcp.digitalRead(mcp.eGPA1);
  GPA2 = mcp.digitalRead(mcp.eGPA2);
  GPA3 = mcp.digitalRead(mcp.eGPA3);
  GPA4 = mcp.digitalRead(mcp.eGPA4);
  GPA5 = mcp.digitalRead(mcp.eGPA5);
  GPA6 = mcp.digitalRead(mcp.eGPA6);
  GPA7 = mcp.digitalRead(mcp.eGPA7);
  //---------------------------------------------------------------------------------------------------------
  if ((alarm == 1) && (prog >= 1)) {
    if (alarm_sound == 1) {
      tone(SWITCH_HP, 800);
      alarm_sound = 0;
    } else {
      tone(SWITCH_HP, 100);
      alarm_sound = 1;
    }
  }
  //---------------------------------------------------------------------------------------------------------

  if ((tmillis > tmillis_checkI2C + 500) && (prog >= 1)) {
    byte error, address;
    for (address = 1; address < 127; address++) {  // fonction permetant de tester toute les addresse possible
      WIRE.beginTransmission(address);
      error = WIRE.endTransmission();
      if (error == 0)  // si une de ces adresses repond ce sera avec un 0 du coup l'adresse sera print
        nDevices++;
    }
    if (nDevices >= 2) {
      alarm = 0;
      noTone(SWITCH_HP);
    }
    if (nDevices < 2) {
      alarm = 1;
    }
    if (nDevices2 > nDevices) {
      tone(SWITCH_HP, 800);
      delay(125);
      tone(SWITCH_HP, 100);
      delay(125);
      noTone(SWITCH_HP);
    }
    if (nDevices2 < nDevices) {
      tone(SWITCH_HP, 100);
      delay(125);
      tone(SWITCH_HP, 800);
      delay(125);
      noTone(SWITCH_HP);
    }
    nDevices2 = nDevices;
    nDevices = 0;
    tmillis_checkI2C = tmillis;
  }
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  if ((bouton_UP == 1) && (bouton_UP2 == 0) && (startend == 1)) {           // detection de front montant pour bouton 1
    if ((prog > 1) && (enterstatus == 0)) {                                 // limite
      prog--;                                                               // programme lié au switch et decr de prog
      affichage = 1;                                                        // permet de faire l'affichage 1 fois car remise a 0 direct
    } else if ((MenuProg3GPB > -1) && (prog == 3) && (enterstatus == 1)) {  // controle up dans programe 3
      MenuProg3GPB--;
      printOnceMenuProg3GPB = 1;

    } else if ((MenuProg4GP > 0) && (prog == 4) && (enterstatus == 1)) {  // controle down dans programe 3
      MenuProg4GP--;
      printOnceMenuProg4GP = 1;
    }
    if (MenuProg3GPB == -1)  // faire une boucle entre 0 et 7
      MenuProg3GPB = 7;
    if (MenuProg4GP == 0)  // faire une boucle entre 0 et 7
      MenuProg4GP = 2;
  }
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_DOWN == 1) && (bouton_DOWN2 == 0) && (startend == 1)) {      // detection de front montant pour bouton 2
    if ((prog < 6) && (enterstatus == 0)) {                                // limit
      prog++;                                                              // programme lié au switch et incr de prog
      affichage = 1;                                                       // permet de faire l'affichage 1 fois car remise a 0 direct
    } else if ((MenuProg3GPB < 8) && (prog == 3) && (enterstatus == 1)) {  // controle down dans programe 3
      MenuProg3GPB++;
      printOnceMenuProg3GPB = 1;
    } else if ((MenuProg4GP < 3) && (prog == 4) && (enterstatus == 1)) {  // controle down dans programe 3
      MenuProg4GP++;
      printOnceMenuProg4GP = 1;
    }
    if (MenuProg3GPB == 8)  // faire une boucle entre 0 et 7
      MenuProg3GPB = 0;
    if (MenuProg4GP == 3)  // faire une boucle entre 0 et 7
      MenuProg4GP = 1;
  }
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_ENTER == 1) && (bouton_ENTER2 == 0) && (startend == 1)) {  // detection de front montant pour bouton 3
    if (enterstatus == 0) {
      enterstatus = 1;   // confirme letat enter pour empeche les changement de prog
      AffichageENTER();  // desactive laffichage du text enter
      EnterClef = 1;     // variable utilisé au débuts des switch/case pour initialisé les millis et autres variables

    } else if (prog == 3) {
      on_off_MenuProg3GPB = 1;  // activé desactivé les GPB (output) dans le programe 3
    } else if (prog == 4) {
      on_off_MenuProg4GP = 1;  // activé desactivé les GPB (output) dans le programe 3
    }
  }
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_EXIT == 1) && (bouton_EXIT2 == 0) && (enterstatus == 1) && (startend == 1)) {  // detection de front montant pour bouton 4
    enterstatus = 0;                                                                         // confirme letat enter pour empeche les changement de prog
    AffichageEXIT();                                                                         // rénitialisation de l'ecran en noir et affichage du text enter
    Serial.println("-------------------------------------------------------------------");   // raffraichit l'ecran en noir
  }
  //---------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  switch (prog) {
    case 0:

      if (tmillis < tmillisstart + 6000) {
        if (affichage == 1) {
          setupstart();   // affichage de setup start
          affichage = 0;  // remise a 0 pour le réalisé 1 fois
        }
        if ((bouton_UP == 1) && (bouton_DOWN == 1)) {
          if (tmillistartset == 1) {
            tmillis_3sec = tmillis;
            tmillistartset = 0;
          }
          if (tmillis > tmillis_3sec + 2000) {
            for (int i = 0; i < 127; i++) {
              EEPROM.write(i, 0);
            }
            tone(SWITCH_HP, 800);
            delay(125);
            tone(SWITCH_HP, 200);
            delay(125);
            tone(SWITCH_HP, 600);
            delay(125);
            noTone(SWITCH_HP);
            tft.fillScreen(RED);
            tft.fillScreen(BLACK);
            prog = 1;
            startend = 1;
            affichage = 1;
            tft.setCursor(0, 60);
            tft.setTextColor(WHITE);
            tft.setTextSize(1);
            tft.print("Appuyez sur ENTER ");
            tft.setCursor(0, 70);
            tft.print("pour validez la ");
            tft.setCursor(0, 80);
            tft.print("selection.");
          }
        }
      }
      // --------------------------------------------------------------------------------------------------------
      else {
        tft.fillScreen(BLACK);
        prog = 1;
        startend = 1;
        affichage = 1;
        tft.setCursor(0, 60);
        tft.setTextColor(WHITE);
        tft.setTextSize(1);
        tft.print("Appuyez sur ENTER ");
        tft.setCursor(0, 70);
        tft.print("pour validez la ");
        tft.setCursor(0, 80);
        tft.print("selection.");
      }
      break;
    case 1:  // température
      if (affichage == 1) {
        AffichageProg1();  // affichage de la bande "programme 1"
        affichage = 0;     // remise a 0 pour le réalisé 1 fois
      }
      // --------------------------------------------------------------------------------------------------------
      if (EnterClef == 1) {  // action réalisé 1 fois
        AffichageSAVE();
        temperature = tempsensor.readTempC();  // varriable float qui prend température
        temperature1 = temperature;            // arrondi de variable Float et implementation dans variable int pour avoir valeur arrondie a l'unité
        temperature_float = temperature - temperature1;
        temperature_int = temperature_float * 100;
        tft.setCursor(0, 20);       // selection des pixels qu'on vont être utilisé
        tft.setTextColor(WHITE);    // choix de la couleur
        tft.print("Temperature:");  // impression sur ecran
        tft.setCursor(65, 30);
        tft.print("*C");
        tft.setCursor(0, 30);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);
        tft.print(temperature1);
        tft.setCursor(25, 40);
        tft.setTextSize(1);
        tft.print(",");
        tft.setCursor(35, 30);
        tft.setTextSize(2);
        tft.print(temperature_int);
        temperature2 = temperature1;  // température mémoire
        temperature_int2 = temperature_int;
        tmillis2 = tmillis;
        EnterClef = 0;
      }

      if (enterstatus == 1) {  // etats de programme lorsque enter est presse
        // --------------------------------------------------------------------------------------------------------
        if ((tmillis > tmillis2 + timetemp) && (temperature2 == temperature1)) {  // actualisation de la température toutes les secondes si et seulement si la mémoire est actualisé
          temperature = tempsensor.readTempC();
          temperature1 = temperature;
          temperature_float = temperature - temperature1;
          temperature_int = temperature_float * 100;
          tmillis2 = tmillis;
        }
        if (temperature2 != temperature1) {  // détection de différence entre nouvelle température et température mémoire
          tft.setCursor(0, 30);
          tft.setTextSize(2);
          tft.setTextColor(BLACK);
          tft.print(temperature2);
          tft.setCursor(0, 30);
          tft.setTextSize(2);
          tft.setTextColor(WHITE);
          tft.print(temperature1);
          temperature2 = temperature1;
        }
        if (temperature_int2 != temperature_int) {  // détection de différence entre nouvelle température et température mémoire
          tft.setCursor(35, 30);
          tft.setTextSize(2);
          tft.setTextColor(BLACK);
          tft.print(temperature_int2);
          tft.setCursor(35, 30);
          tft.setTextSize(2);
          tft.setTextColor(WHITE);
          tft.print(temperature_int);
          temperature_int2 = temperature_int;
        }
        if ((bouton_ENTER == 1) && (timeMemoire == 1)) {
          if (bouton_ENTER2 == 0) {
            tmillis_3sec = tmillis;
          }
          if (tmillis > tmillis_3sec + 3000) {
            tft.fillScreen(GREEN);
            tft.fillScreen(BLACK);
            tone(SWITCH_HP, 400);
            delay(125);
            tone(SWITCH_HP, 200);
            delay(125);
            tone(SWITCH_HP, 800);
            delay(125);
            noTone(SWITCH_HP);
            StructureTemp TempProfile = { temperature };
            ecrireTempEEPROM(AdresseTemperature, TempProfile);
            affichage = 1;
            EnterClef = 1;
            timeMemoire = 0;
          }
        } else
          timeMemoire = 1;
      }
      break;
      // --------------------------------------------------------------------------------------------------------
    case 2:  // heure et date
      if (affichage == 1) {
        AffichageProg2();  // affichage de la bande "programme 2"
        affichage = 0;
      }
      // --------------------------------------------------------------------------------------------------------
      if (EnterClef == 1) {
        AffichageSAVE();
        tft.setCursor(0, 20);
        tft.setTextSize(1);
        tft.setTextColor(WHITE);
        tft.print("Heure et Date:");
        Year = now.year();      // actualisation de  l'anne 1 seule fois au debut du programme 2
        Month = now.month();    // actualisation du mois 1 seule fois au debut du programme 2
        Day = now.day();        // actualisation du jour 1 seule fois au debut du programme 2
        Hour = now.hour();      // actualisation de l'heure
        Minute = now.minute();  // actualisation de la minute
        Second = now.second();  // actualisation de la seconde
        tft.setCursor(0, 50);
        tft.print(Year);  // impression de lannée 1 fois sans actualisation au debut du programme 2
        tft.print("/");
        tft.print(Month);  // impression de lannée 1 fois sans actualisation au debut du programme 2
        tft.print("/");
        tft.print(Day);  // impression de lannée 1 fois sans actualisation au debut du programme 2
        tft.setCursor(0, 40);
        if (Hour < 10)
          tft.print("0");
        tft.print(Hour);  // premiere affichage de lheure avant actualisation
        tft.setCursor(10, 40);
        tft.print(":");
        tft.setCursor(15, 40);
        if (Minute < 10)
          tft.print("0");
        tft.print(Minute);  // premiere affichage de la minute avant actualisation
        tft.setCursor(25, 40);
        tft.print(":");
        tft.setCursor(30, 40);
        tft.print(Second);   // premiere affichage de la seconde avant actualisation
        HourMem = Hour;      // memoire heure
        MinuteMem = Minute;  // memoire minute
        SecondMem = Second;  // memoire seconde
        EnterClef = 0;       //reset de la variable
      }

      if (enterstatus == 1) {
        Hour = now.hour();      // actualisation de l'heure a chaque cycle arduino
        Minute = now.minute();  // actualisation de la minute a chaque cycle arduino
        Second = now.second();  // actualisation de la seconde a chaque cycle arduino

        // --------------------------------------------------------------------------------------------------------
        if (HourMem != Hour) {  // detection de difference avec actualisation et memoire
          tft.setCursor(0, 40);
          tft.setTextColor(BLACK);  // affichage effacé pour réecrire
          if (Hour < 10)
            tft.print("0");
          tft.print(HourMem);
          tft.setCursor(0, 40);
          tft.setTextColor(WHITE);  // affichage réecrit
          if (Hour < 10)
            tft.print("0");
          tft.print(Hour);
          HourMem = Hour;
        }
        if (MinuteMem != Minute) {  // detection de difference avec actualisation et memoire
          tft.setCursor(15, 40);
          tft.setTextColor(BLACK);  // affichage effacé pour réecrire
          if (Minute < 10)
            tft.print("0");
          tft.print(MinuteMem);
          tft.setCursor(15, 40);
          tft.setTextColor(WHITE);  // affichage réecrit
          if (Minute < 10)
            tft.print("0");
          tft.print(Minute);
          MinuteMem = Minute;
        }
        if (SecondMem != Second) {  // detection de difference avec actualisation et memoire
          tft.setCursor(30, 40);
          tft.setTextColor(BLACK);  // affichage effacé pour réecrire
          tft.print(SecondMem);
          tft.setCursor(30, 40);
          tft.setTextColor(WHITE);  // affichage réecrit
          tft.print(Second);
          SecondMem = Second;
        }
        if ((bouton_ENTER == 1) && (timeMemoire == 1)) {
          if (bouton_ENTER2 == 0) {
            tmillis_3sec = tmillis;
          }
          if (tmillis > tmillis_3sec + 3000) {
            tone(SWITCH_HP, 400);
            delay(125);
            tone(SWITCH_HP, 200);
            delay(125);
            tone(SWITCH_HP, 800);
            delay(125);
            noTone(SWITCH_HP);
            StructureClock ProfilClock = { Year, Month, Day, HourMem, MinuteMem, SecondMem };
            ecrireClockEEPROM(AdresseClock, ProfilClock);
            tft.fillScreen(GREEN);
            tft.fillScreen(BLACK);
            affichage = 1;
            EnterClef = 1;
            timeMemoire = 0;
          }
        }
        if (bouton_ENTER == 0) {
          timeMemoire = 1;
        }
      }
      break;
      // --------------------------------------------------------------------------------------------------------
      // --------------------------------------------------------------------------------------------------------
    case 3:  // controle MCP23017
      if (affichage == 1) {
        AffichageMenuProg3GPB();  // affichage
        affichage = 0;
      }
      if (EnterClef == 1) {
        tft.setCursor(0, 30);
        tft.setTextColor(WHITE);
        tft.print("Bouton enter on/off");
        tft.setCursor(0, 40);
        tft.print("___________________");

        tft.setCursor(TextGPA, 50);
        tft.print("GPA0");
        tft.setCursor(Coordonne_X_TextGPB, 50);
        tft.print("GPB0");

        tft.setCursor(TextGPA, 60);
        tft.print("GPA1");
        tft.setCursor(Coordonne_X_TextGPB, 60);
        tft.print("GPB1");

        tft.setCursor(TextGPA, 70);
        tft.print("GPA2");
        tft.setCursor(Coordonne_X_TextGPB, 70);
        tft.print("GPB2");

        tft.setCursor(TextGPA, 80);
        tft.print("GPA3");
        tft.setCursor(Coordonne_X_TextGPB, 80);
        tft.print("GPB3");

        tft.setCursor(TextGPA, 90);
        tft.print("GPA4");
        tft.setCursor(Coordonne_X_TextGPB, 90);
        tft.print("GPB4");

        tft.setCursor(TextGPA, 100);
        tft.print("GPA5");
        tft.setCursor(Coordonne_X_TextGPB, 100);
        tft.print("GPB5");

        tft.setCursor(TextGPA, 110);
        tft.print("GPA6");
        tft.setCursor(Coordonne_X_TextGPB, 110);
        tft.print("GPB6");

        tft.setCursor(TextGPA, 120);
        tft.print("GPA7");
        tft.setCursor(Coordonne_X_TextGPB, 120);
        tft.print("GPB7");
        //________________________________________________________________
        // affichage des etats on/off memoire GPA

        if (etat_GPA0 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 53, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 53, 4, GREEN);
        }
        if (etat_GPA1 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 63, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 63, 4, GREEN);
        }
        if (etat_GPA2 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 73, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 73, 4, GREEN);
        }
        if (etat_GPA3 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 83, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 83, 4, GREEN);
        }
        if (etat_GPA4 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 93, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 93, 4, GREEN);
        }
        if (etat_GPA5 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 103, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 103, 4, GREEN);
        }
        if (etat_GPA6 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 113, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 113, 4, GREEN);
        }
        if (etat_GPA7 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPA, 123, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPA, 123, 4, GREEN);
        }


        //________________________________________________________________
        // affichage des etats on/off memoire GPB


        if (etat_GPB0 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 53, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 53, 4, GREEN);
        }
        if (etat_GPB1 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 63, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 63, 4, GREEN);
        }
        if (etat_GPB2 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 73, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 73, 4, GREEN);
        }
        if (etat_GPB3 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 83, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 83, 4, GREEN);
        }
        if (etat_GPB4 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 93, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 93, 4, GREEN);
        }
        if (etat_GPB5 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 103, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 103, 4, GREEN);
        }
        if (etat_GPB6 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 113, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 113, 4, GREEN);
        }
        if (etat_GPB7 == 0) {
          tft.fillCircle(Cercle_X_Prog3_GPB, 123, 4, RED);
        } else {
          tft.fillCircle(Cercle_X_Prog3_GPB, 123, 4, GREEN);
        }


        MenuProg3GPB = 0;
        printOnceMenuProg3GPB = 1;
        Bulle_Y_Prog3_GPB_Mem = 48;
        Text_Y_Prog3_GPB_Mem = 50;
        MenuProg3GPB_memoire = MenuProg3GPB;
        EnterClef = 0;
      }

      if (enterstatus == 1) {
        //________________________________________________________________
        // detection de font montant pour changement d'etats GPA
        if ((GPA0 == 1) && (GPA0_2 == 0)) {
          Cercle_Y_Prog3_GPA = 53;
          if (etat_GPA0 == 1) {
            CercleRougeGPA();
            etat_GPA0 = 0;
          } else if (etat_GPA0 == 0) {
            CercleVertGPA();
            etat_GPA0 = 1;
          }
        }
        GPA0_2 = GPA0;

        if ((GPA1 == 1) && (GPA1_2 == 0)) {
          Cercle_Y_Prog3_GPA = 63;
          if (etat_GPA1 == 1) {
            CercleRougeGPA();
            etat_GPA1 = 0;
          } else if (etat_GPA1 == 0) {
            CercleVertGPA();
            etat_GPA1 = 1;
          }
        }
        GPA1_2 = GPA1;
        if ((GPA2 == 1) && (GPA2_2 == 0)) {
          Cercle_Y_Prog3_GPA = 73;
          if (etat_GPA2 == 1) {
            CercleRougeGPA();
            etat_GPA2 = 0;
          } else if (etat_GPA2 == 0) {
            CercleVertGPA();
            etat_GPA2 = 1;
          }
        }
        GPA2_2 = GPA2;
        if ((GPA3 == 1) && (GPA3_2 == 0)) {
          Cercle_Y_Prog3_GPA = 83;
          if (etat_GPA3 == 1) {
            CercleRougeGPA();
            etat_GPA3 = 0;
          } else if (etat_GPA3 == 0) {
            CercleVertGPA();
            etat_GPA3 = 1;
          }
        }
        GPA3_2 = GPA3;
        if ((GPA4 == 1) && (GPA4_2 == 0)) {
          Cercle_Y_Prog3_GPA = 93;
          if (etat_GPA4 == 1) {
            CercleRougeGPA();
            etat_GPA4 = 0;
          } else if (etat_GPA4 == 0) {
            CercleVertGPA();
            etat_GPA4 = 1;
          }
        }
        GPA4_2 = GPA4;
        if ((GPA5 == 1) && (GPA5_2 == 0)) {
          Cercle_Y_Prog3_GPA = 103;
          if (etat_GPA5 == 1) {
            CercleRougeGPA();
            etat_GPA5 = 0;
          } else if (etat_GPA5 == 0) {
            CercleVertGPA();
            etat_GPA5 = 1;
          }
        }
        GPA5_2 = GPA5;
        if ((GPA6 == 1) && (GPA6_2 == 0)) {
          Cercle_Y_Prog3_GPA = 113;
          if (etat_GPA6 == 1) {
            CercleRougeGPA();
            etat_GPA6 = 0;
          } else if (etat_GPA6 == 0) {
            CercleVertGPA();
            etat_GPA6 = 1;
          }
        }
        GPA6_2 = GPA6;
        if ((GPA7 == 1) && (GPA7_2 == 0)) {
          Cercle_Y_Prog3_GPA = 123;
          if (etat_GPA7 == 1) {
            CercleRougeGPA();
            etat_GPA7 = 0;
          } else if (etat_GPA7 == 0) {
            CercleVertGPA();
            etat_GPA7 = 1;
          }
        }
        GPA7_2 = GPA7;
        //________________________________________________________________
        // menu des Sortie GPB avec controle SWITCH
        // controle sur etats avec switch enter
        switch (MenuProg3GPB) {
          case 0:
            Bulle_Y_Prog3_GPB = 48;
            Text_Y_Prog3_GPB = 50;
            Cercle_Y_Prog3_GPB = 53;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB0 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB0, LOW);
                etat_GPB0 = 0;
              } else if (etat_GPB0 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB0, HIGH);
                etat_GPB0 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
          case 1:
            Bulle_Y_Prog3_GPB = 58;
            Text_Y_Prog3_GPB = 60;
            Cercle_Y_Prog3_GPB = 63;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB1 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB1, LOW);
                etat_GPB1 = 0;
              } else if (etat_GPB1 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB1, HIGH);
                etat_GPB1 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
          case 2:
            Bulle_Y_Prog3_GPB = 68;
            Text_Y_Prog3_GPB = 70;
            Cercle_Y_Prog3_GPB = 73;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB2 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB2, LOW);
                etat_GPB2 = 0;
              } else if (etat_GPB2 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB2, HIGH);
                etat_GPB2 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
          case 3:
            Bulle_Y_Prog3_GPB = 78;
            Text_Y_Prog3_GPB = 80;
            Cercle_Y_Prog3_GPB = 83;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB3 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB3, LOW);
                etat_GPB3 = 0;
              } else if (etat_GPB3 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB3, HIGH);
                etat_GPB3 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
          case 4:
            Bulle_Y_Prog3_GPB = 88;
            Text_Y_Prog3_GPB = 90;
            Cercle_Y_Prog3_GPB = 93;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB4 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB4, LOW);
                etat_GPB4 = 0;
              } else if (etat_GPB4 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB4, HIGH);
                etat_GPB4 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
          case 5:
            Bulle_Y_Prog3_GPB = 98;
            Text_Y_Prog3_GPB = 100;
            Cercle_Y_Prog3_GPB = 103;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB5 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB5, LOW);
                etat_GPB5 = 0;
              } else if (etat_GPB5 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB5, HIGH);
                etat_GPB5 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
          case 6:
            Bulle_Y_Prog3_GPB = 108;
            Text_Y_Prog3_GPB = 110;
            Cercle_Y_Prog3_GPB = 113;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB6 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB6, LOW);
                etat_GPB6 = 0;
              } else if (etat_GPB6 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB6, HIGH);
                etat_GPB6 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
          case 7:
            Bulle_Y_Prog3_GPB = 118;
            Text_Y_Prog3_GPB = 120;
            Cercle_Y_Prog3_GPB = 123;
            if (on_off_MenuProg3GPB == 1) {
              if (etat_GPB7 == 1) {
                CercleRougeGPB();
                mcp.digitalWrite(mcp.eGPB7, LOW);
                etat_GPB7 = 0;
              } else if (etat_GPB7 == 0) {
                CercleVertGPB();
                mcp.digitalWrite(mcp.eGPB7, HIGH);
                etat_GPB7 = 1;
              }
              on_off_MenuProg3GPB = 0;
            }
            break;
        }
        if (printOnceMenuProg3GPB == 1) {  // fonction permetant de print 1 fois les affichage avec remise a 0
          PrintMenuProg3GPB();
          Serial.println("print");
          printOnceMenuProg3GPB = 0;
        }
      }
      break;

      // --------------------------------------------------------------------------------------------------------
    case 4:  // memoire
      if (affichage == 1) {
        AffichageProg4();  // affichage de la bande "programme 4"
        affichage = 0;
      }
      if (enterstatus == 1) {

        if (EnterClef == 1) {
          StructureTemp TempRecup;
          lireTempEEPROM(AdresseTemperature, TempRecup);  // appelle de la fonction température pour recupérer les données dans la mémoire
          //
          StructureClock ClockRecup;
          lireClockEEPROM(AdresseClock, ClockRecup);  // appelle de la fonction température pour recupérer les données dans la mémoire
          //_______________________________________
          tft.setCursor(0, 30);
          tft.setTextColor(WHITE);
          tft.fillRect(0, 36, 3, 17, RED);
          tft.println("--------------------");
          tft.println(" Temperature : ");
          tft.print(" ");
          tft.println(TempRecup.ProfilTemperature);  // température écrite depuis la mémoire
          tft.fillRect(0, 53, 3, 17, BLUE);
          tft.println(" Clock : ");
          tft.print(" ");
          tft.print(ClockRecup.ClockYear);  // annee depuis memoire
          tft.print("/");
          tft.print(ClockRecup.ClockMonth);  // mois depuis memoire
          tft.print("/");
          tft.print(ClockRecup.ClockDay);  // jour depuis memoire
          tft.print("-");
          tft.print(ClockRecup.ClockHour);  // heure depuis memoire
          tft.print(":");
          tft.print(ClockRecup.ClockMinute);  // minute depuis memoire
          tft.print(":");
          tft.print(ClockRecup.ClockSecond);  // seconde depuis memoire
          tft.fillRect(0, 70, 3, 46, GREEN);
          PrintMenuProg4GPA();  // affichage du menu et des selections
          printProg4EtatGP();   // affichage des memoire gpa et gpb
          AffichageSAVE();      // affichage des commande pour save
          EnterClef = 0;
          MenuProg4GP = 1;
        }
        switch (MenuProg4GP) {  // menu pour choix entre gpa et gpb
          case 1:               // GPA
            if (printOnceMenuProg4GP == 1) {
              PrintMenuProg4GPA();
              printOnceMenuProg4GP = 0;
            }
            if ((bouton_ENTER == 1) && (timeMemoire == 1)) {  // detection de bouton pression 3 sec ou plus 1x
              if (bouton_ENTER2 == 0) {
                tmillis_3sec = tmillis;
              }
              if (tmillis > tmillis_3sec + 3000) {
                tone(SWITCH_HP, 400);
                delay(125);
                tone(SWITCH_HP, 200);
                delay(125);
                tone(SWITCH_HP, 800);
                delay(125);
                noTone(SWITCH_HP);
                StructureGPA GPAProfile = { etat_GPA0, etat_GPA1, etat_GPA2, etat_GPA3, etat_GPA4, etat_GPA5, etat_GPA6, etat_GPA7 };  // placement des données dans structure
                ecrireGPAEEPROM(AdresseGPA, GPAProfile);                                                                               // envoie des données dans la structure
                tft.fillScreen(GREEN);
                tft.fillScreen(BLACK);
                affichage = 1;
                EnterClef = 1;
                timeMemoire = 0;
              }
            } else
              timeMemoire = 1;
            break;
          case 2:  // GPB
            if (printOnceMenuProg4GP == 1) {
              PrintMenuProg4GPB();
              printOnceMenuProg4GP = 0;
            }
            if ((bouton_ENTER == 1) && (timeMemoire == 1)) {
              if (bouton_ENTER2 == 0) {
                tmillis_3sec = tmillis;
              }
              if (tmillis > tmillis_3sec + 3000) {
                tone(SWITCH_HP, 400);
                delay(125);
                tone(SWITCH_HP, 200);
                delay(125);
                tone(SWITCH_HP, 800);
                delay(125);
                noTone(SWITCH_HP);

                StructureGPB GPBProfile = { etat_GPB0, etat_GPB1, etat_GPB2, etat_GPB3, etat_GPB4, etat_GPB5, etat_GPB6, etat_GPB7 };
                ecrireGPBEEPROM(AdresseGPB, GPBProfile);
                tft.fillScreen(GREEN);
                tft.fillScreen(BLACK);
                affichage = 1;
                EnterClef = 1;
                timeMemoire = 0;
              }
            } else
              timeMemoire = 1;
            break;
        }
      }
      break;
      // --------------------------------------------------------------------------------------------------------
    case 5:  // affichage adresse I2C
      if (affichage == 1) {
        AffichageProg5();  // affichage de la bande "programme 5"
        affichage = 0;
      }
      if (EnterClef == 1) {  // reset des variable importante
        tmillis3 = tmillis;
        EnterClef = 0;
        EnterClef5 = 1;
        Adresse_Y_Prog5 = 50;

        tft.setCursor(0, 20);
        tft.setTextColor(BLACK);
        tft.print("Affichage adresse I2C");
      }

      if (EnterClef5 == 1) {
        byte error, address;
        for (address = 1; address < 127; address++) {  // fonction permetant de tester toute les addresse possible
          WIRE.beginTransmission(address);
          error = WIRE.endTransmission();
          if (error == 0) {  // si une de ces adresses repond ce sera avec un 0 du coup l'adresse sera print
            tft.setCursor(5, 30);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("I2C trouve a");
            tft.setCursor(5, 40);
            tft.print("l'address :");
            tft.setCursor(80, Adresse_Y_Prog5);
            tft.print("0x");
            if (address < 16) {  // si ladresse est plus petite que 16 elle sera a 1 chiffre necessitant ainsi un 0
              tft.setCursor(93, Adresse_Y_Prog5);
              tft.print("0");
              tft.setCursor(96, Adresse_Y_Prog5);
            } else
              tft.setCursor(93, Adresse_Y_Prog5);
            tft.print(address, HEX);
            Adresse_Y_Prog5 = Adresse_Y_Prog5 + 10;
            nDevices++;
          } else if (error == 4) {
            tft.setCursor(5, 60);
            tft.print("Erreur inconnu a");
            tft.setCursor(5, 70);
            tft.print("l'address :");
            tft.setCursor(5, 80);
            tft.print("0x");
            if (address < 16) {
              tft.setCursor(16, 80);
              tft.print("0");
            } else
              tft.setCursor(13, 80);
            tft.print(address, HEX);
          }
        }
        if (nDevices == 0) {  // si aucun esclaves est detecté alors nous avons 0 adresse
          tft.setTextColor(WHITE);
          tft.setCursor(15, 60);
          tft.print("I2C introuvable\n");
          EnterClef5 = 0;
          tft.setCursor(5, 90);
          tft.println("Nbres Esclaves I2C:");
          tft.setCursor(60, 110);
          tft.setTextColor(CYAN);
          tft.print(0);
        } else {
          tft.setCursor(5, 90);
          tft.println("Nbres Esclaves I2C:");
          tft.setCursor(60, 110);
          tft.setTextColor(CYAN);
          tft.print(nDevices);
          EnterClef5 = 0;
          nDevices = 0;
        }
      }
      if ((tmillis > tmillis3 + 5000) && (enterstatus == 1)) {  // detection des adresses toutes les 5 secondes
        tft.fillScreen(BLACK);
        AffichageProg5();
        EnterClef = 1;
      }
      break;
    case 6:  // Text écrit input utilisateur
      if (affichage == 1) {
        AffichageProg6();  // affichage de la bande "programme 6"
        affichage = 0;
      }
      if (EnterClef == 1) {
        StructureInput InputRecup;
        lireInputEEPROM(AdresseInput, InputRecup);  // recuperation dans la memoire
        //_______________________________________
        while (Serial.available() > 0) {
          char c = Serial.read();  // nettoyage du serial pour sassurer quelle soit libre
        }
        InputOperateur = InputRecup.ProfilInput;
        InputOperateur.toCharArray(InputOperateurCharArray, maxInputSize);
        AffichageProg6READ();

        EnterClef = 0;
      }
      if (enterstatus == 1) {

        if ((bouton_ENTER == 1) && (timeMemoire == 1)) {  // detection apres 3 sec
          if (bouton_ENTER2 == 0) {
            tmillis_3sec = tmillis;
          }
          if (tmillis > tmillis_3sec + 3000) {
            tone(SWITCH_HP, 400);
            delay(125);
            tone(SWITCH_HP, 200);
            delay(125);
            tone(SWITCH_HP, 800);
            delay(125);
            noTone(SWITCH_HP);
            InputOperateur2 = InputOperateurCharArray;  // mise en memoire pour effacage sur ecran
            AffichageProg6WRITE();
            for (int i = AdresseInput; i < AdresseInput + maxInputSize; i++) {  // netoyage de lécriture a partir de l'adresse input et de la taille attribué pour la memoire
              EEPROM.write(i, 0);
            }
            while (Serial.available() > 0) {  // vidage de serial
              char c = Serial.read();
            }
            Serial.println("Entrez votre text : ");
            delay(100);
            while (Serial.available() == 0) {
            }
            Serial.readString().toCharArray(InputOperateurCharArray, maxInputSize);  // lecture de Serial et mise dans varibale
            StructureInput InputProfile;
            strncpy(InputProfile.ProfilInput, InputOperateurCharArray, maxInputSize);  // placement de mon text dans ma structure
            ecrireInputEEPROM(AdresseInput, InputProfile);  // stockage dans memoire
            AffichageProg6READ();
            WriteInput = 0;
            timeMemoire = 0;
          } else
            timeMemoire = 0;
        } else
          timeMemoire = 1;
      }
      break;
  }
  bouton_UP2 = bouton_UP;
  bouton_DOWN2 = bouton_DOWN;
  bouton_ENTER2 = bouton_ENTER;
  bouton_EXIT2 = bouton_EXIT;
}
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void AffichageProg1() {  // affichage bande et texte "programe 1"
  static const uint16_t PROGMEM colors[] = { RED };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 1");
  }
  tft.setCursor(0, 20);
  tft.setTextColor(BLACK);
  tft.print("Heure et Date");  //Effacer
  tft.setTextColor(WHITE);
  tft.setCursor(0, 20);
  tft.print("Temperature");  //Afficher
}
void AffichageProg2() {  // affichage bande et texte "programe 2"
  static const uint16_t PROGMEM colors[] = { BLUE };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 2");
  }
  tft.setCursor(0, 20);
  tft.setTextColor(BLACK);
  tft.print("Temperature");  //Effacer
  tft.setCursor(0, 20);
  tft.print("Controle MCP23017");  //Effacer

  tft.setTextColor(WHITE);
  tft.setCursor(0, 20);
  tft.print("Heure et Date");  //Afficher
}
void AffichageMenuProg3GPB() {  // affichage bande et texte "programe 3"
  static const uint16_t PROGMEM colors[] = { GREEN };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 3");
  }
  tft.setCursor(0, 20);
  tft.setTextColor(BLACK);
  tft.print("Heure et Date");  //Effacer
  tft.setCursor(0, 20);
  tft.print("Memoire");  //Effacer

  tft.setTextColor(WHITE);
  tft.setCursor(0, 20);
  tft.print("Controle MCP23017");  //Afficher
}
void AffichageProg4() {  // affichage bande et texte "programe 4"
  static const uint16_t PROGMEM colors[] = { MAGENTA };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 4");
  }
  tft.setCursor(0, 20);
  tft.setTextColor(BLACK);
  tft.print("Controle MCP23017");  //Effacer
  tft.setCursor(0, 20);
  tft.print("Affichage adresse I2C");  //Effacer

  tft.setTextColor(WHITE);
  tft.setCursor(0, 20);
  tft.print("Memoire");  //Afficher
}
void AffichageProg5() {  // affichage bande et texte "programe 5"
  static const uint16_t PROGMEM colors[] = { CYAN };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 5");
  }
  tft.setCursor(0, 20);
  tft.setTextColor(BLACK);
  tft.print("Memoire");
  tft.setCursor(0, 20);
  tft.setTextColor(BLACK);
  tft.print("Text input");

  tft.setTextColor(WHITE);
  tft.setCursor(0, 20);
  tft.print("Affichage adresse I2C");  //Afficher
}
void AffichageProg6() {  // affichage bande et texte "programe 5"
  static const uint16_t PROGMEM colors[] = { ORANGE };

  for (uint8_t c = 0; c < 1; c++) {
    tft.fillRect(0, tft.height() * c / 6, tft.width(), tft.height() / 8,
                 pgm_read_word(&colors[c]));
    tft.setCursor(30, 5);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("Programme 6");
  }
  tft.setCursor(0, 20);
  tft.setTextColor(BLACK);
  tft.print("Affichage adresse I2C");

  tft.setTextColor(WHITE);
  tft.setCursor(0, 20);
  tft.print("Text input");  //Afficher
}
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void PrintMenuProg3GPB() {

  static const uint16_t PROGMEM colors[] = { BLACK };
  tft.fillRoundRect(88, Bulle_Y_Prog3_GPB_Mem, 27, 11, 8, BLACK);
  tft.setCursor(Coordonne_X_TextGPB, Text_Y_Prog3_GPB_Mem);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("GPB");
  tft.print(MenuProg3GPB_memoire);

  tft.fillRoundRect(88, Bulle_Y_Prog3_GPB, 27, 11, 8, WHITE);
  tft.setCursor(Coordonne_X_TextGPB, Text_Y_Prog3_GPB);
  tft.setTextColor(BLACK);
  tft.print("GPB");
  tft.print(MenuProg3GPB);

  MenuProg3GPB_memoire = MenuProg3GPB;
  Bulle_Y_Prog3_GPB_Mem = Bulle_Y_Prog3_GPB;
  Text_Y_Prog3_GPB_Mem = Text_Y_Prog3_GPB;
}
void CercleRougeGPA() {
  tft.fillCircle(Cercle_X_Prog3_GPA, Cercle_Y_Prog3_GPA, 4, RED);
}
void CercleVertGPA() {
  tft.fillCircle(Cercle_X_Prog3_GPA, Cercle_Y_Prog3_GPA, 4, GREEN);
}
void CercleRougeGPB() {
  tft.fillCircle(Cercle_X_Prog3_GPB, Cercle_Y_Prog3_GPB, 4, RED);
}
void CercleVertGPB() {
  tft.fillCircle(Cercle_X_Prog3_GPB, Cercle_Y_Prog3_GPB, 4, GREEN);
}
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void PrintMenuProg4GPA() {

  tft.fillRoundRect(8, Bulle_Y_Prog4_GPA, 21, 11, 8, WHITE);
  tft.setCursor(Coordonne_X_TextGP, Text_Y_Prog4_GPA);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("GPA");

  tft.fillRoundRect(8, Bulle_Y_Prog4_GPB, 21, 11, 8, BLACK);
  tft.setCursor(Coordonne_X_TextGP, Text_Y_Prog4_GPB);
  tft.setTextColor(WHITE);
  tft.print("GPB");
}
void PrintMenuProg4GPB() {

  tft.fillRoundRect(8, Bulle_Y_Prog4_GPA, 21, 11, 8, BLACK);
  tft.setCursor(Coordonne_X_TextGP, Text_Y_Prog4_GPA);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("GPA");

  tft.fillRoundRect(8, Bulle_Y_Prog4_GPB, 21, 11, 8, WHITE);
  tft.setCursor(Coordonne_X_TextGP, Text_Y_Prog4_GPB);
  tft.setTextColor(BLACK);
  tft.print("GPB");
}
void printProg4EtatGP() {
  //
  StructureGPA GPARecup;
  lireGPAEEPROM(AdresseGPA, GPARecup);
  //
  StructureGPB GPBRecup;
  lireGPBEEPROM(AdresseGPB, GPBRecup);
  //_______________________________________
  if (GPARecup.etat_GPA0_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.setCursor(Coordonne_X_TextGP - 2, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("0");
  } else {
    tft.fillCircle(Coordonne_X_TextGP, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP - 2, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("0");
  }
  if (GPARecup.etat_GPA1_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 10, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 8, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("1");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 10, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 8, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("1");
  }
  if (GPARecup.etat_GPA2_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 20, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 18, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("2");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 20, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 18, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("2");
  }
  if (GPARecup.etat_GPA3_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 30, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 28, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("3");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 30, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 28, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("3");
  }
  if (GPARecup.etat_GPA4_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 40, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 38, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("4");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 40, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 38, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("4");
  }
  if (GPARecup.etat_GPA5_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 50, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 48, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("5");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 50, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 48, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("5");
  }
  if (GPARecup.etat_GPA6_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 60, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 58, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("6");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 60, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 58, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("6");
  }
  if (GPARecup.etat_GPA7_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 70, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 68, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("7");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 70, Text_Y_Prog4_GPA + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 68, Text_Y_Prog4_GPA + decalage_text_GP);
    tft.print("7");
  }
  //________________________________________________________________
  // affichage des etats on/off memoire GPB
  if (GPBRecup.etat_GPB0_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP - 2, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("0");
  } else {
    tft.fillCircle(Coordonne_X_TextGP, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP - 2, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("0");
  }
  if (GPBRecup.etat_GPB1_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 10, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 8, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("1");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 10, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 8, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("1");
  }
  if (GPBRecup.etat_GPB2_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 20, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 18, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("2");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 20, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 18, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("2");
  }
  if (GPBRecup.etat_GPB3_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 30, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 28, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("3");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 30, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 28, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("3");
  }
  if (GPBRecup.etat_GPB4_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 40, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 38, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("4");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 40, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 38, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("4");
  }
  if (GPBRecup.etat_GPB5_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 50, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 48, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("5");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 50, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 48, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("5");
  }
  if (GPBRecup.etat_GPB6_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 60, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 58, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("6");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 60, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 58, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("6");
  }
  if (GPBRecup.etat_GPB7_mem == 0) {
    tft.fillCircle(Coordonne_X_TextGP + 70, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, RED);
    tft.setCursor(Coordonne_X_TextGP + 68, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("7");
  } else {
    tft.fillCircle(Coordonne_X_TextGP + 70, Text_Y_Prog4_GPB + decalage_cercle_GP, 4, GREEN);
    tft.setCursor(Coordonne_X_TextGP + 68, Text_Y_Prog4_GPB + decalage_text_GP);
    tft.print("7");
  }
}
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void ecrireTempEEPROM(int AdresseTemperature, const StructureTemp& dataTemp) {  // adresse mise a 9
  EEPROM.put(AdresseTemperature, dataTemp.ProfilTemperature);
}
void lireTempEEPROM(int AdresseTemperature, StructureTemp& dataTemp) {
  // Lire chaque membre de la structure depuis l'EEPROM
  EEPROM.get(AdresseTemperature, dataTemp.ProfilTemperature);
}
void ecrireClockEEPROM(int AdresseClock, const StructureClock& dataClock) {
  EEPROM.put(AdresseClock, dataClock.ClockYear);
  EEPROM.put(AdresseClock + sizeof(dataClock.ClockYear), dataClock.ClockMonth);
  EEPROM.put(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth), dataClock.ClockDay);
  EEPROM.put(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth) + sizeof(dataClock.ClockDay), dataClock.ClockHour);
  EEPROM.put(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth) + sizeof(dataClock.ClockDay) + sizeof(dataClock.ClockHour), dataClock.ClockMinute);
  EEPROM.put(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth) + sizeof(dataClock.ClockDay) + sizeof(dataClock.ClockHour) + sizeof(dataClock.ClockMinute), dataClock.ClockSecond);
}
void lireClockEEPROM(int AdresseClock, StructureClock& dataClock) {
  EEPROM.get(AdresseClock, dataClock.ClockYear);
  EEPROM.get(AdresseClock + sizeof(dataClock.ClockYear), dataClock.ClockMonth);
  EEPROM.get(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth), dataClock.ClockDay);
  EEPROM.get(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth) + sizeof(dataClock.ClockDay), dataClock.ClockHour);
  EEPROM.get(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth) + sizeof(dataClock.ClockDay) + sizeof(dataClock.ClockHour), dataClock.ClockMinute);
  EEPROM.get(AdresseClock + sizeof(dataClock.ClockYear) + sizeof(dataClock.ClockMonth) + sizeof(dataClock.ClockDay) + sizeof(dataClock.ClockHour) + sizeof(dataClock.ClockMinute), dataClock.ClockSecond);
}
void ecrireGPAEEPROM(int AdresseGPA, const StructureGPA& dataGPA) {
  EEPROM.put(AdresseGPA, dataGPA.etat_GPA0_mem);
  EEPROM.put(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem), dataGPA.etat_GPA1_mem);
  EEPROM.put(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem), dataGPA.etat_GPA2_mem);
  EEPROM.put(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem), dataGPA.etat_GPA3_mem);
  EEPROM.put(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem), dataGPA.etat_GPA4_mem);
  EEPROM.put(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem) + sizeof(dataGPA.etat_GPA4_mem), dataGPA.etat_GPA5_mem);
  EEPROM.put(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem) + sizeof(dataGPA.etat_GPA4_mem) + sizeof(dataGPA.etat_GPA5_mem), dataGPA.etat_GPA6_mem);
  EEPROM.put(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem) + sizeof(dataGPA.etat_GPA4_mem) + sizeof(dataGPA.etat_GPA5_mem) + sizeof(dataGPA.etat_GPA6_mem), dataGPA.etat_GPA7_mem);
}
void lireGPAEEPROM(int AdresseGPA, StructureGPA& dataGPA) {
  EEPROM.get(AdresseGPA, dataGPA.etat_GPA0_mem);
  EEPROM.get(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem), dataGPA.etat_GPA1_mem);
  EEPROM.get(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem), dataGPA.etat_GPA2_mem);
  EEPROM.get(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem), dataGPA.etat_GPA3_mem);
  EEPROM.get(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem), dataGPA.etat_GPA4_mem);
  EEPROM.get(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem) + sizeof(dataGPA.etat_GPA4_mem), dataGPA.etat_GPA5_mem);
  EEPROM.get(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem) + sizeof(dataGPA.etat_GPA4_mem) + sizeof(dataGPA.etat_GPA5_mem), dataGPA.etat_GPA6_mem);
  EEPROM.get(AdresseGPA + sizeof(dataGPA.etat_GPA0_mem) + sizeof(dataGPA.etat_GPA1_mem) + sizeof(dataGPA.etat_GPA2_mem) + sizeof(dataGPA.etat_GPA3_mem) + sizeof(dataGPA.etat_GPA4_mem) + sizeof(dataGPA.etat_GPA5_mem) + sizeof(dataGPA.etat_GPA6_mem), dataGPA.etat_GPA7_mem);
}
void ecrireGPBEEPROM(int AdresseGPB, const StructureGPB& dataGPB) {
  EEPROM.put(AdresseGPB, dataGPB.etat_GPB0_mem);
  EEPROM.put(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem), dataGPB.etat_GPB1_mem);
  EEPROM.put(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem), dataGPB.etat_GPB2_mem);
  EEPROM.put(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem), dataGPB.etat_GPB3_mem);
  EEPROM.put(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem), dataGPB.etat_GPB4_mem);
  EEPROM.put(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem) + sizeof(dataGPB.etat_GPB4_mem), dataGPB.etat_GPB5_mem);
  EEPROM.put(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem) + sizeof(dataGPB.etat_GPB4_mem) + sizeof(dataGPB.etat_GPB5_mem), dataGPB.etat_GPB6_mem);
  EEPROM.put(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem) + sizeof(dataGPB.etat_GPB4_mem) + sizeof(dataGPB.etat_GPB5_mem) + sizeof(dataGPB.etat_GPB6_mem), dataGPB.etat_GPB7_mem);
}
void lireGPBEEPROM(int AdresseGPB, StructureGPB& dataGPB) {
  EEPROM.get(AdresseGPB, dataGPB.etat_GPB0_mem);
  EEPROM.get(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem), dataGPB.etat_GPB1_mem);
  EEPROM.get(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem), dataGPB.etat_GPB2_mem);
  EEPROM.get(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem), dataGPB.etat_GPB3_mem);
  EEPROM.get(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem), dataGPB.etat_GPB4_mem);
  EEPROM.get(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem) + sizeof(dataGPB.etat_GPB4_mem), dataGPB.etat_GPB5_mem);
  EEPROM.get(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem) + sizeof(dataGPB.etat_GPB4_mem) + sizeof(dataGPB.etat_GPB5_mem), dataGPB.etat_GPB6_mem);
  EEPROM.get(AdresseGPB + sizeof(dataGPB.etat_GPB0_mem) + sizeof(dataGPB.etat_GPB1_mem) + sizeof(dataGPB.etat_GPB2_mem) + sizeof(dataGPB.etat_GPB3_mem) + sizeof(dataGPB.etat_GPB4_mem) + sizeof(dataGPB.etat_GPB5_mem) + sizeof(dataGPB.etat_GPB6_mem), dataGPB.etat_GPB7_mem);
}
// --------------------------------------------------------------------------------------------------------
void AffichageProg6WRITE() {
  tft.setTextColor(BLACK);
  tft.setCursor(0, 30);
  tft.print(InputOperateur2);
  tft.setCursor(0, 90);
  tft.println("Appuyez sur enter");
  tft.print("pour input:");
  tft.setTextColor(WHITE);
  tft.setCursor(0, 110);
  tft.println("Entrez le text dans ");
  tft.print("la console:");  //Afficher
}
void AffichageProg6READ() {
  tft.setTextColor(BLACK);
  tft.setCursor(0, 110);
  tft.println("Entrez le text dans ");
  tft.print("la console:");  //Afficher
  tft.setTextColor(WHITE);
  tft.setCursor(0, 30);
  tft.print(InputOperateurCharArray);
  tft.setCursor(0, 90);
  tft.println("Appuyez sur enter");
  tft.print("pour input:");
}
void lireInputEEPROM(int AdresseInput, StructureInput& dataInput) {
  EEPROM.get(AdresseInput, dataInput.ProfilInput);
}
void ecrireInputEEPROM(int AdresseInput, const StructureInput& dataInput) {
  EEPROM.put(AdresseInput, dataInput.ProfilInput);
}
// --------------------------------------------------------------------------------------------------------
void AffichageENTER() {
  tft.setCursor(0, 60);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("Appuyez sur ENTER ");
  tft.setCursor(0, 70);
  tft.print("pour validez la ");
  tft.setCursor(0, 80);
  tft.print("selection.");
}
void AffichageEXIT() {  // Rénitialisation de l'ecran tout noir et affichage de la bande programme
  tft.fillScreen(BLACK);
  affichage = 1;
  tft.setCursor(0, 60);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("Appuyez sur ENTER ");
  tft.setCursor(0, 70);
  tft.print("pour validez la ");
  tft.setCursor(0, 80);
  tft.print("selection.");
}
void AffichageSAVE() {
  tft.setCursor(0, 120);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("Hold ENTER = Save");
}
void setupstart() {  // afficahge de l'ecran de demarage
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("PROJET    M2446 I2C");
  tft.setCursor(0, 40);
  tft.setTextSize(1);
  tft.print("GRR");
  tft.setCursor(0, 50);
  tft.print("MPO");
  tft.fillCircle(64, 128, 65, WHITE);
  tft.fillCircle(64, 128, 55, CYAN);
  tft.fillCircle(64, 128, 45, BLUE);
  tft.fillCircle(64, 128, 35, GREEN);
  tft.fillCircle(64, 128, 25, YELLOW);
  tft.fillCircle(64, 128, 15, MAGENTA);
  tft.fillCircle(64, 128, 5, RED);
}
