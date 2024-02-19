
//https://github.com/Arduino-IRremote/Arduino-IRremote
#include <IRremote.h>

#include <Adafruit_NeoPixel.h>

//https://github.com/Seeed-Studio/Seeed_Arduino_UltrasonicRanger
#include "Ultrasonic.h"

#include <Servo.h>

// DEFINITION PIN DE CONNEXION DU CAPTEUR INFRA ROUGE
#define IR_PIN 8

// DEFINITION DES BOUTONS DE LA TELECOMMANDE
#define FLECHE_HAUT 0x46
#define FLECHE_BAS 0x15
#define FLECHE_GAUCHE 0x44
#define FLECHE_DROITE 0x43
#define TOUCHE_1 0x16
#define TOUCHE_2 0x19
#define TOUCHE_3 0xD
#define TOUCHE_4 0xC
#define TOUCHE_5 0x18
#define TOUCHE_6 0x5E
#define TOUCHE_7 0x8
#define TOUCHE_8 0x1C
#define TOUCHE_9 0x5A
#define TOUCHE_0 0x52
#define TOUCHE_ETOILE 0x42
#define TOUCHE_DIESE 0x4A
#define TOUCHE_OK 0x40

//Nombre de LED sur le dos (une de caché)
#define N_LEDS 7
//Numéro de la pin des LED
#define LED_PIN 7

//Pin servo gauche
#define SERVOL_PIN 5
//Pin servo droite
#define SERVOR_PIN 6

//Position servo arret
#define SERVO_MID 90
//Position servo positive
#define SERVO_PLUS 100
//Position servo negative
#define SERVO_MINUS 80

//controle des LED
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(N_LEDS, LED_PIN);

Servo servoL;
Servo servoR;

//Ultrason connecté à la pin 2
Ultrasonic ultrasonic(2);

void setup() {

  //Laison série vers le PC
  Serial.begin(9600);

  //initialisation des servos
  servoL.attach(SERVOL_PIN);
  servoR.attach(SERVOR_PIN);
  servoL.write(SERVO_MID);
  servoR.write(SERVO_MID);

  //Initialisationd des LEDs
  pixels.begin();

  //Initialisation Recpteur Infrarouche de la télécomande
  Serial.println("Enabling IR");
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Enabled IR");

}

// Booleen a vrai quand la tortue avance.
bool foward = false;

void loop() {

  if (IrReceiver.decode()) { //Si un commande est reçu de la télécommande
    switch (IrReceiver.decodedIRData.command) { // on execute des actions selon le bouton appuyé.
    case FLECHE_HAUT:
      servoL.write(SERVO_PLUS);
      servoR.write(SERVO_MINUS);
      foward = true;
      pixels.fill(pixels.Color(150, 150, 150));
      pixels.show();
      break;
    case FLECHE_BAS:
      servoL.write(SERVO_MINUS);
      servoR.write(SERVO_PLUS);
      foward = false;
      pixels.fill(pixels.Color(0, 150, 0));
      pixels.show();
      break;
    case FLECHE_DROITE:
      servoL.write(SERVO_PLUS);
      servoR.write(SERVO_MID);
      foward = false;
      pixels.fill(pixels.Color(0, 0, 150));
      pixels.show();
      break;
    case FLECHE_GAUCHE:
      servoL.write(SERVO_MID);
      servoR.write(SERVO_MINUS);
      foward = false;
      pixels.fill(pixels.Color(0, 150, 150));
      pixels.show();
      break;
    case TOUCHE_OK:
      servoL.write(SERVO_MID);
      servoR.write(SERVO_MID);
      foward = false;
      pixels.fill(pixels.Color(0, 0, 0));
      pixels.show();
      break;
    }
    //On s'assure que l'on recoive les prochaines commandes
    IrReceiver.resume();
  }

  //pause de 50 millisecondes
  delay(50);
  
  if (foward) { //Si on avance on vérifie l'ultrason.
    long RangeInCentimeters = 999;
    RangeInCentimeters = ultrasonic.MeasureInCentimeters();

    if (RangeInCentimeters > 0 && RangeInCentimeters < 10) { // > 0 car 0=erreur et < 10cm
      Serial.print("Detect ! ");
      Serial.println(RangeInCentimeters);
      foward = false;
      pixels.fill(pixels.Color(150, 0, 0));
      pixels.show();
      //on tourne pendand 1 secondes
      servoL.write(SERVO_MID);
      servoR.write(SERVO_MINUS);
      delay(1000);
      //On s'arrête
      servoL.write(SERVO_MID);
      servoR.write(SERVO_MID);
      pixels.fill(pixels.Color(0, 0, 0));
      pixels.show();
    }
  }

}
