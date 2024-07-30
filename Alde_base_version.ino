#include <SevSeg.h>

SevSeg sevseg;  // Création de l'objet SevSeg

// Définition des broches pour les LEDs et les boutons
const char LED_1 = A0;
const char LED_2 = A1;
const char LED_3 = A2;
const char LED_4 = A3;
const char LED_5 = A4;  
const char LED_6 = A11;  
const char LED_7 = A7;  
const char LED_8 = A8;  
const char LED_9 = A12;  
const char LED_START = A10;

const char bouton_START = 12;
const char bouton_1 = 14;
const char bouton_2 = 3;
const char bouton_3 = 13;
const char bouton_4 = 5;
const char bouton_5 = 11;  
const char bouton_6 = 15;  
const char bouton_7 = 7;  
const char bouton_8 = 9;  
const char bouton_9 = 10; 


const char signal1 = 40;
const char signal2 = 41;
const char signal3 = 42;
const char signal4 = 43;
const char signal5 = 44;
const char signal6 = 45;
const char signal7 = 46;
const char signal8 = 47;
const char signal9 = 48;
const char signal_start = 49;

// Variables de temps et de jeu
unsigned long startTime = 0;
const unsigned long sequenceDuration = 59000; // Durée de la séquence en millisecondes (20 secondes)
const unsigned long displayScoreDuration = 5000; // Durée d'affichage du score final en millisecondes (5 secondes)

unsigned long currentLedOnTime = 0;
const unsigned long maxLedOnTime = 2000; // Temps maximum en millisecondes

unsigned long lastIncorrectButtonTime = 0; //ariable pour stocker le dernier moment où un bouton incorrect a été pressé
const unsigned long minIncorrectButtonInterval = 350; //constante pour définir la durée minimale entre deux pressions de bouton incorrect (en millisecondes)

unsigned long lastBlinkTime = 0;
const unsigned long blinkDuration = 500; // Durée de chaque état du clignotement en millisecondes (0.5 seconde)
bool displayOn = true;

int ledToTurnOn = 0;
int lastLedTurnedOn = 0; // Nouvelle variable pour stocker le dernier numéro de LED allumée
long point = 0;
long highscore = 0;

void setup() {

  Serial.begin(9600); // Initialisation de la communication série

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  pinMode(LED_7, OUTPUT);
  pinMode(LED_8, OUTPUT);
  pinMode(LED_9, OUTPUT);
  pinMode(LED_START, OUTPUT);
  
  pinMode(signal1, OUTPUT);  
  pinMode(signal2, OUTPUT);
  pinMode(signal3, OUTPUT);
  pinMode(signal4, OUTPUT);
  pinMode(signal5, OUTPUT);  
  pinMode(signal6, OUTPUT);
  pinMode(signal7, OUTPUT);
  pinMode(signal8, OUTPUT);
  pinMode(signal9, OUTPUT);
  pinMode(signal_start, OUTPUT);

  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  digitalWrite(LED_4, HIGH);
  digitalWrite(LED_5, HIGH);
  digitalWrite(LED_6, HIGH);
  digitalWrite(LED_7, HIGH);
  digitalWrite(LED_8, HIGH);
  digitalWrite(LED_9, HIGH);
  digitalWrite(LED_START, HIGH);

  digitalWrite(signal1, HIGH);
  digitalWrite(signal2, HIGH);
  digitalWrite(signal3, HIGH);
  digitalWrite(signal4, HIGH);
  digitalWrite(signal5, HIGH);
  digitalWrite(signal6, HIGH);
  digitalWrite(signal7, HIGH);
  digitalWrite(signal8, HIGH);
  digitalWrite(signal9, HIGH);
  digitalWrite(signal_start, HIGH);


  pinMode(bouton_START, INPUT_PULLUP);
  pinMode(bouton_1, INPUT_PULLUP);
  pinMode(bouton_2, INPUT_PULLUP);
  pinMode(bouton_3, INPUT_PULLUP);
  pinMode(bouton_4, INPUT_PULLUP);
  pinMode(bouton_5, INPUT_PULLUP);
  pinMode(bouton_6, INPUT_PULLUP);
  pinMode(bouton_7, INPUT_PULLUP);
  pinMode(bouton_8, INPUT_PULLUP);
  pinMode(bouton_9, INPUT_PULLUP);

  randomSeed(analogRead(0)); // Initialiser la graine pour la génération de nombres aléatoires 

  // Configuration des broches pour les segments et les digits
  byte numDigits = 6;
  byte digitPins[] = {30, 31, 32, 50, 51, 52}; // Broches pour les digits
  byte segmentPins[] = {21, 22, 23, 24, 25, 26, 27}; // Broches pour les segments
  bool resistorsOnSegments = false; // pas de resistance integré
  byte hardwareConfig = COMMON_CATHODE; // type d'afficheur
  bool updateWithDelays = false;
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // true si pas de dote point


 sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint); // initialisation objet sevseg
  sevseg.setBrightness(100);  // Réglage de la luminosité de l'afficheur (0-100%)  
}

void loop() {
  unsigned long currentMillis = millis();  // Obtention du temps actuel
  digitalWrite(LED_START, HIGH);
  
    
  long affichage = (point * 1000 ) + highscore ;

    sevseg.setNumber(affichage);  // Définition du nombre à afficher sur l'afficheur
    sevseg.refreshDisplay();  // Rafraîchissement de l'afficheur
  

  if (digitalRead(bouton_START) == LOW) { // Si le bouton de démarrage est pressé
    startTime = millis(); // Enregistrez le temps de début
    digitalWrite(LED_START, LOW);
    digitalWrite(signal_start, LOW);
    point = 0; // Réinitialisez le compteur de points
    Serial.println("Nouvelle séquence !");
    digitalWrite(signal_start, HIGH);
     
    while (millis() - startTime < sequenceDuration) { // Tant que la séquence est en cours
      currentMillis = millis();

      long affichage = (point * 1000 ) + highscore ; /* [Redeclacration of varaible, maybe implicit cast also] */

      

        sevseg.setNumber(affichage);  // Définition du nombre à afficher sur l'afficheur
        sevseg.refreshDisplay();  // Rafraîchissement de l'afficheur
      

    if (ledToTurnOn == 0 || (currentMillis - currentLedOnTime >= maxLedOnTime)) {
      // Si aucune LED n'est allumée ou si le temps maximum est dépassé, allumez une nouvelle LED
      ledToTurnOn = random(1, 10);
      lastIncorrectButtonTime = currentMillis;
      
      allumerLED(ledToTurnOn);
      Serial.print("LED à allumer : ");
      Serial.println(ledToTurnOn);
      currentLedOnTime = currentMillis; // Mémorisez le temps actuel
    }

      // Vérifiez si le bouton correspondant à la LED allumée a été pressé
      if (digitalRead(bouton_1) == LOW && ledToTurnOn == 1) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal1, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 1 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal1, HIGH);
        ledToTurnOn = 0;
      } 
       
      else if (digitalRead(bouton_2) == LOW && ledToTurnOn == 2) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal2, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 2 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal2, HIGH);
        ledToTurnOn = 0;
      } 
       
      else if (digitalRead(bouton_3) == LOW && ledToTurnOn == 3) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal3, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 3 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal3, HIGH);
        ledToTurnOn = 0;
      } 
      
      else if (digitalRead(bouton_4) == LOW && ledToTurnOn == 4) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 4 pressé. Point +1");
        afficherPoints();
        ledToTurnOn = 0;
      }

      else if (digitalRead(bouton_5) == LOW && ledToTurnOn == 5) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal5, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 5 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal5, HIGH);
        ledToTurnOn = 0;
      } 

      else if (digitalRead(bouton_6) == LOW && ledToTurnOn == 6) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal6, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 6 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal6, HIGH);
        ledToTurnOn = 0;
      } 

      else if (digitalRead(bouton_7) == LOW && ledToTurnOn == 7) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal7, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 7 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal7, HIGH);
        ledToTurnOn = 0;
      } 

      else if (digitalRead(bouton_8) == LOW && ledToTurnOn == 8) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal8, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 8 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal8, HIGH);
        ledToTurnOn = 0;
      } 

      else if (digitalRead(bouton_9) == LOW && ledToTurnOn == 9) {
        eteindreLED(ledToTurnOn);
        lastLedTurnedOn = ledToTurnOn;
        digitalWrite(signal9, LOW);
        point++; // Incrémentez le compteur de points
        Serial.println("Bouton 9 pressé. Point +1");
        afficherPoints();
        digitalWrite(signal9, HIGH);
        ledToTurnOn = 0;
      } 

      else if ((digitalRead(bouton_1) == LOW && ledToTurnOn != 1) ||
             (digitalRead(bouton_2) == LOW && ledToTurnOn != 2) ||
             (digitalRead(bouton_3) == LOW && ledToTurnOn != 3) ||
             (digitalRead(bouton_4) == LOW && ledToTurnOn != 4) ||
             (digitalRead(bouton_5) == LOW && ledToTurnOn != 5) ||
             (digitalRead(bouton_6) == LOW && ledToTurnOn != 6) ||
             (digitalRead(bouton_7) == LOW && ledToTurnOn != 7) ||
             (digitalRead(bouton_8) == LOW && ledToTurnOn != 8) ||
             (digitalRead(bouton_9) == LOW && ledToTurnOn != 9)) {

               
                  // Vérifiez si suffisamment de temps s'est écoulé depuis le dernier bouton incorrect pressé
            if ((currentMillis - lastIncorrectButtonTime >= minIncorrectButtonInterval) && point > 5 ) {
                // Décrémentez les points de 5 et enregistrez le moment actuel
              point -= 5;
              lastIncorrectButtonTime = currentMillis;
              Serial.println("Bouton incorrect pressé. Points -5");
              afficherPoints();
              }
       }
    }

    // La séquence est terminée, éteindre toutes les LEDs
    eteindreLED(1);
    eteindreLED(2);
    eteindreLED(3);
    eteindreLED(4);
    eteindreLED(5);
    eteindreLED(6);
    eteindreLED(7);
    eteindreLED(8);
    eteindreLED(9);
    ledToTurnOn = 0;
    

      if (point > highscore ) {
        highscore = point;
      }

    // Afficher le score final pendant 5 secondes
    unsigned long displayEndTime = millis() + displayScoreDuration;
   
    while (millis() < displayEndTime) 
    {
    unsigned long currentMillis2 = millis();
    long affichage = (point * 1000 ) + highscore ;

    if(highscore == point){
        
        if (currentMillis2 - lastBlinkTime >= blinkDuration) {
        lastBlinkTime = currentMillis2;  // Mise à jour du temps précédent
        displayOn = !displayOn;  // Inversion de l'état de l'affichage (allumé/éteint)
        }
        if (displayOn) {
        sevseg.setNumber(affichage);  // Définition du nombre à afficher sur l'afficheur
         } else {
         sevseg.blank();  // Éteindre l'afficheur
         }
         sevseg.refreshDisplay();  // Rafraîchissement de l'afficheur
    }
      else{
      sevseg.setNumber(affichage);
      sevseg.refreshDisplay();
      }
    }
    
    // Effacer l'afficheur après l'affichage du score final
    point = 0;
    
    sevseg.refreshDisplay();

    afficherScore();
  }
}

void allumerLED(int led) {
  // Éteindre toutes les LEDs
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  digitalWrite(LED_4, HIGH);
  digitalWrite(LED_5, HIGH);  
  digitalWrite(LED_6, HIGH);  
  digitalWrite(LED_7, HIGH);  
  digitalWrite(LED_8, HIGH);  
  digitalWrite(LED_9, HIGH);

  // Identifier la broche de la LED en fonction du numéro de LED
  int brocheLED;
  switch (led) {
    case 1:
      brocheLED = LED_1;
      break;
    case 2:
      brocheLED = LED_2;
      break;
    case 3:
      brocheLED = LED_3;
      break;
    case 4:
      brocheLED = LED_4;
      break;
    case 5:
      brocheLED = LED_5;
      break;
    case 6:
      brocheLED = LED_6;
      break;
    case 7:
      brocheLED = LED_7;
      break;
    case 8:
      brocheLED = LED_8;
      break;
    case 9:
      brocheLED = LED_9;
      break;
    default:
      // Gestion d'une valeur incorrecte
      return;
  }

  // Allumer la LED correspondante
  digitalWrite(brocheLED, LOW);
}

void eteindreLED(int led) {
  // Identifier la broche de la LED en fonction du numéro de LED
  int brocheLED;
  switch (led) {
    case 1:
      brocheLED = LED_1;
      break;
    case 2:
      brocheLED = LED_2;
      break;
    case 3:
      brocheLED = LED_3;
      break;
    case 4:
      brocheLED = LED_4;
      break;
    case 5:
      brocheLED = LED_5;
      break;
    case 6:
      brocheLED = LED_6;
      break;
    case 7:
      brocheLED = LED_7;
      break;
    case 8:
      brocheLED = LED_8;
      break;
    case 9:
      brocheLED = LED_9;
      break;
    default:
      // Gestion d'une valeur incorrecte
      return;
  }

  // Éteindre la LED correspondante
  digitalWrite(brocheLED, HIGH);
}

void afficherPoints() {
  Serial.print("Points : ");
  Serial.println(point);
}

void afficherScore() {
  Serial.print("Score final : ");
  Serial.println(point);
}
