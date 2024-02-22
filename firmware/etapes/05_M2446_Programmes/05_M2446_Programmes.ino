//---------------------------------------------------------------
//---------------------------------------------------------------
//NOM DU PROJET: M2446
//AUTEUR: MPO
//DATE: 22 février 2024
//description: code permetant de programmer et essayer le menu de notre programme
//---------------------------------------------------------------
//---------------------------------------------------------------
//  Numerique
#define UP 8
#define DOWN 9
#define ENTER 10
#define EXIT 11

#define HP 13

// variables pour bouton
bool bUP = 0;
bool bUP2 = 0;
bool bDOWN = 0;
bool bDOWN2 = 0;
bool bENTER = 0;
bool bENTER2 = 0;
bool bEXIT = 0;
bool bEXIT2 = 0;

bool enterstatus = 0;

//variables pour etats
int prog = 0;
int incr = 0;

//variables tone
int sound = 0;      // en htz
int frequence = 0;  // en ms

void setup() {
  Serial.begin(9600);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(ENTER, INPUT);
  pinMode(EXIT, INPUT);

  pinMode(HP, OUTPUT);
  prog = 1;
}
void loop() {

  // -------------------------------------------------------------------------------------------------------------------------------------------------
  bUP = digitalRead(UP);        // 1
  bDOWN = digitalRead(DOWN);    // 2
  bENTER = digitalRead(ENTER);  // 3
  bEXIT = digitalRead(EXIT);    // 4
  // -------------------------------------------------------------------------------------------------------------------------------------------------

  if ((bUP == 1) && (bUP2 == 0) && (enterstatus == 0)) {
    Serial.println("Bouton UP");
    if (prog > 1) {
      prog--;
    }
    //action réalisé 1 fois pour bouton 1
  }
  bUP2 = bUP;


  if ((bDOWN == 1) && (bDOWN2 == 0) && (enterstatus == 0)) {
    Serial.println("Bouton DOWN");
    if (prog < 4) {
      prog++;
    }
    //action réalisé 1 fois pour bouton 2
  }
  bDOWN2 = bDOWN;


  if ((bENTER == 1) && (bENTER2 == 0)) {
    Serial.println("Bouton ENTER");
    enterstatus = 1;
    //action réalisé 1 fois pour bouton 3
  }
  bENTER2 = bENTER;


  if ((bEXIT == 1) && (bEXIT2 == 0)) {
    Serial.println("Bouton EXIT");
    enterstatus = 0;
    //action réalisé 1 fois pour bouton 4
  }
  bEXIT2 = bEXIT;
  
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  switch (prog) {
    case 1:
      if (enterstatus == 1) {
        Serial.println("Prog 1 set");
      } else {
        Serial.println("Prog 1");
      }
      break;
    case 2:
      if (enterstatus == 1) {
        Serial.println("Prog 2 set");
      } else {
        Serial.println("Prog 2");
      }
      break;
    case 3:
      if (enterstatus == 1) {
        Serial.println("Prog 3 set");
      } else {
        Serial.println("Prog 3");
      }
      break;
    case 4:
      if (enterstatus == 1) {
        Serial.println("Prog 4 set");
      } else {
        Serial.println("Prog 4");
      }
      break;
  }
}
