//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 22 février 2024
//description: code permetant de tester et essayer les affichages de l'écran
//---------------------------------------------------------------
//---------------------------------------------------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#define MOSI_PIN 6
#define SCLK_PIN 5
#define DC_PIN 4
#define RST_PIN 3
#define CS_PIN 2

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF


int cursor = 0;

// Option 1: use any pins but a little slower
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.fillScreen(BLACK);
}
void loop() {
  tft.setCursor(0, cursor);
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  cursor = cursor + 9;
  tft.setCursor(0, cursor);
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.println("Coucou Guilhem!");
  cursor = cursor + 9;
  if (cursor > 100) {
    cursor = 0;
  }
  /*
  tft.setCursor(0,0);
  tft.setTextColor(BLACK);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  tft.setCursor(0,0);
  tft.setTextColor(GREEN);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  
  tft.setCursor(0,0);
  tft.setTextColor(BLACK);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  tft.setCursor(0,0);
  tft.setTextColor(CYAN);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  
  tft.setCursor(0,0);
  tft.setTextColor(BLACK);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  tft.setCursor(0,0);
  tft.setTextColor(MAGENTA);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
 
  tft.setCursor(0,0);
  tft.setTextColor(BLACK);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  tft.setCursor(0,0);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  
  tft.setCursor(0,0);
  tft.setTextColor(BLACK);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  tft.setCursor(0,0);
  tft.setTextColor(WHITE);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  
  tft.setCursor(0,0);
  tft.setTextColor(BLACK);  
  tft.setTextSize(1);
  tft.print("Coucou Guilhem!");
  tft.setCursor(0,0);
  */
}
