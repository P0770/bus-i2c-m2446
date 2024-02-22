//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 22 février 2024
//description: code réalisé apres la troisième semaine 2emepartie pour la validation avec système de prog1-5 et utilisation de : tempTC74 / OLEDSSD1351 / ClockDS3231 / AdresseI2C / MCP9808 
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
#define WIRE Wire
//---------------------------------------------------------------------------------------------------------
//  Numerique pin bouton define
#define SWITCH_UP 8
#define SWITCH_DOWN 9
#define SWITCH_ENTER 10
#define SWITCH_EXIT 11
#define SWITCH_HP 12
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
int timeled = 600;
int timetemp = 1000;
int ti2c = 1000;
//---------------------------------------------------------------------------------------------------------
//variable programe 1 température
float temperature = 0;
int temperature1 = 0;
int temperature2 = 0;
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

bool GPA0_2 = 0;
bool GPA1_2 = 0;
bool GPA2_2 = 0;
bool GPA3_2 = 0;
bool GPA4_2 = 0;
bool GPA5_2 = 0;
bool GPA6_2 = 0;
bool GPA7_2 = 0;

//memoires d'etats GPA GPB
bool etat_GPA0 = 0;
bool etat_GPA1 = 0;
bool etat_GPA2 = 0;
bool etat_GPA3 = 0;
bool etat_GPA4 = 0;
bool etat_GPA5 = 0;
bool etat_GPA6 = 0;
bool etat_GPA7 = 0;

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
int nDevices = 0;         // variable permettant de compter le nmbr d'escalves sur I2C
int Adresse_Y_Prog5 = 0;  // varaible pour stocker coordonées pour tft.print les adresse i2c


//---------------------------------------------------------------------------------------------------------
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
  setupstart();
  while (!Serial)
    delay(10);
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

  prog = 1;       // mise de prog a 1
  affichage = 1;  // affichage des bande prog

  delay(3000);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 60);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("Appuyez sur ENTER ");
  tft.setCursor(0, 70);
  tft.print("pour validez la ");
  tft.setCursor(0, 80);
  tft.print("selection.");
}

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void loop() {
  DateTime now = myRTC.now();  // fonction time dans une variable
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
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  if ((bouton_UP == 1) && (bouton_UP2 == 0)) {                              // detection de front montant pour bouton 1
    if ((prog > 1) && (enterstatus == 0)) {                                 // limite
      prog--;                                                               // programme lié au switch et decr de prog
      affichage = 1;                                                        // permet de faire l'affichage 1 fois car remise a 0 direct
    } else if ((MenuProg3GPB > -1) && (prog == 3) && (enterstatus == 1)) {  // controle up dans programe 3
      MenuProg3GPB--;
      printOnceMenuProg3GPB = 1;
      Serial.println("up");
    }
    if (MenuProg3GPB == -1)  // faire une boucle entre 0 et 7
      MenuProg3GPB = 7;
  }
  bouton_UP2 = bouton_UP;
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_DOWN == 1) && (bouton_DOWN2 == 0)) {                         // detection de front montant pour bouton 2
    if ((prog < 5) && (enterstatus == 0)) {                                // limit
      prog++;                                                              // programme lié au switch et incr de prog
      affichage = 1;                                                       // permet de faire l'affichage 1 fois car remise a 0 direct
    } else if ((MenuProg3GPB < 8) && (prog == 3) && (enterstatus == 1)) {  // controle down dans programe 3
      MenuProg3GPB++;
      printOnceMenuProg3GPB = 1;
      Serial.println("down");
    }
    if (MenuProg3GPB == 8)  // faire une boucle entre 0 et 7
      MenuProg3GPB = 0;
  }
  bouton_DOWN2 = bouton_DOWN;
  //---------------------------------------------------------------------------------------------------------
  if ((bouton_ENTER == 1) && (bouton_ENTER2 == 0)) {  // detection de front montant pour bouton 3
    if (enterstatus == 0) {
      enterstatus = 1;   // confirme letat enter pour empeche les changement de prog
      AffichageENTER();  // desactive laffichage du text enter
      EnterClef = 1;     // variable utilisé au débuts des switch/case pour initialisé les millis et autres variables
      Serial.println("-------------------------------------------------------------------");
    } else if (prog == 3) {
      on_off_MenuProg3GPB = 1;  // activé desactivé les GPB (output) dans le programe 3
      Serial.println("on_off--------------------");
    }
  }
  bouton_ENTER2 = bouton_ENTER;

  //---------------------------------------------------------------------------------------------------------
  if ((bouton_EXIT == 1) && (bouton_EXIT2 == 0) && (enterstatus == 1)) {                    // detection de front montant pour bouton 4
    enterstatus = 0;                                                                        // confirme letat enter pour empeche les changement de prog
    AffichageEXIT();                                                                        // rénitialisation de l'ecran en noir et affichage du text enter
    Serial.println("-------------------------------------------------------------------");  // raffraichit l'ecran en noir
  }
  bouton_EXIT2 = bouton_EXIT;
  //---------------------------------------------------------------------------------------------------------
  // --------------------------------------------------------------------------------------------------------
  switch (prog) {
    // --------------------------------------------------------------------------------------------------------
    case 1:  // température
      if (affichage == 1) {
        AffichageProg1();  // affichage de la bande "programme 1"
        affichage = 0;     // remise a 0 pour le réalisé 1 fois
      }
      // --------------------------------------------------------------------------------------------------------

      if (EnterClef == 1) {                    // action réalisé 1 fois
        temperature = tempsensor.readTempC();  // varriable float qui prend température
        temperature1 = round(temperature);     // arrondi de variable Float et implementation dans variable int pour avoir valeur arrondie a l'unité
        tft.setCursor(0, 20);                  // selection des pixels qu'on vont être utilisé
        tft.setTextColor(WHITE);               // choix de la couleur
        tft.print("Temperature:");             // impression sur ecran
        tft.setCursor(30, 30);
        tft.print("*C");
        tft.setCursor(0, 30);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);
        tft.print(temperature1);
        temperature2 = temperature1;  // température mémoire
        tmillis2 = tmillis;
        EnterClef = 0;
      }

      if (enterstatus == 1) {  // etats de programme lorsque enter est presse
        // --------------------------------------------------------------------------------------------------------
        if ((tmillis > tmillis2 + timetemp) && (temperature2 == temperature1)) {  // actualisation de la température toutes les secondes si et seulement si la mémoire est actualisé
          temperature = tempsensor.readTempC();
          temperature1 = round(temperature);
          tmillis2 = tmillis;
        }
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
      } else {
        Serial.println("Prog 1 Température");
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
        tft.setCursor(0, 20);
        tft.setTextSize(1);
        tft.setTextColor(WHITE);
        tft.print("Heure et Date:");
        Year = now.year();    // actualisation de  l'anne 1 seule fois au debut du programme 2
        Month = now.month();  // actualisation du mois 1 seule fois au debut du programme 2
        Day = now.day();      // actualisation du jour 1 seule fois au debut du programme 2
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
          tft.setCursor(0, 20);
          tft.setTextColor(BLACK);
          tft.print("Memoire");  //Effacer
          EnterClef = 0;
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
        for (address = 1; address < 127; address++) { // fonction permetant de tester toute les addresse possible
          WIRE.beginTransmission(address);
          error = WIRE.endTransmission();
          if (error == 0) { // si une de ces adresses repond ce sera avec un 0 du coup l'adresse sera print
            tft.setCursor(5, 30);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);
            tft.print("I2C trouve a");
            tft.setCursor(5, 40);
            tft.print("l'address :");
            tft.setCursor(80, Adresse_Y_Prog5);
            tft.print("0x");
            if (address < 16) { // si ladresse est plus petite que 16 elle sera a 1 chiffre necessitant ainsi un 0
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
        if (nDevices == 0) { // si aucun esclaves est detecté alors nous avons 0 adresse
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
      if ((tmillis > tmillis3 + 5000) && (enterstatus == 1)) { // detection des adresses toutes les 5 secondes
        tft.fillScreen(BLACK);
        AffichageProg5();
        EnterClef = 1;
      }
      break;
  }
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
// --------------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------------
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

  tft.setTextColor(WHITE);
  tft.setCursor(0, 20);
  tft.print("Affichage adresse I2C");  //Afficher
}
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
void setupstart() { // afficahge de l'ecran de demarage
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