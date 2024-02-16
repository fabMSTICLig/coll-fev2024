#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "rgb_lcd.h"

//define PIN
#define SOUND_SENSOR_PIN  A2
#define LED_PIN  A0

//define params
#define LED_COUNT 3

// coll-fev2024

//Projets réalisés au Fablab Mastic (UGA) par les collegiens accueillis lors du stage 3eme organisé par Persyval 

// define colors
#define RED 1 
#define GREEN 2
#define BLUE 3
#define NOPE 0


//Variable temps du dernier clap(pour mesurer le temps quil reste avant de compter)
long timeLastClap = 0;

//Variable compte le nombre de claps sur un court laps de temps
int nombreDeClape = 0;

//Variable pour manipuler le ruban de led:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//Variable pour manipuler l'ecran LCD
rgb_lcd lcd;

// Variables pour manipuler les servo-moteurs
Servo myservo_roue_1;
Servo myservo_roue_2;

Servo myservo_bras;


void setup() 
{
    // initialisaiton moniteur serie
    Serial.begin(9600);

    // initialisation ruban de leds
    strip.begin();
    strip.show();
    strip.setBrightness(50);

    myservo_roue_1.attach(5);
    myservo_roue_2.attach(6);
    myservo_bras.attach(3);

    myservo_bras.write(10);

    // initialisation ecran LCD
    lcd.begin(16, 2);
    lcd.setRGB(255, 255, 255);

    delay(500);
    lcd.print("setup OK");
}

//fonction pour recuperer le volume sonore
int get_sound_value(void)
{
    int soundValue = 0;
    for (int i = 0; i < 32; i++)
    {
        soundValue += analogRead(SOUND_SENSOR_PIN);
    }
    soundValue >>= 5;
//    Serial.println(soundValue);
    return (soundValue); 
}

//fonction pour appliquer une couleur aux leds
void set_leds_colors(int color)
{
    int32_t col = strip.Color(0, 0, 0);

    if (color == RED)
    {
        col = strip.Color(80, 0, 0);
        lcd.setRGB(255, 0, 0);
    }
    else if (color == GREEN)
    {
        col = strip.Color(0, 80, 0);
        lcd.setRGB(0, 255, 0);
    }
    else if (color == BLUE)
    {
        col = strip.Color(0, 0, 80);
        lcd.setRGB(0, 0, 255);
    }
    strip.fill(col);
    strip.show();
}

//fonction pour faire 'dancer' le robot
void dancing(void)
{
  Serial.println("fonction Dancing ...");  
  printLCD("Dance...");
  myservo_roue_1.write(80);
  myservo_roue_2.write(80);
  delay(300);
  myservo_roue_1.write(100);
  myservo_roue_2.write(100);
  delay(300);
  myservo_roue_1.write(80);
  myservo_roue_2.write(100);
  delay(200);
  myservo_roue_1.write(80);
  myservo_roue_2.write(100);
  delay(200);
  myservo_roue_1.write(90);
  myservo_roue_2.write(90);
  delay(500);
  Serial.println("fonction Dancing END");
}

//fonction pour ecrire une phrase sur l'ecran
void printLCD(const char *str)
{
  lcd.clear();
  lcd.home();
  lcd.print(str);
}

//fonction pour faire un check: lever le bras et tourner
void fonction_check(void)
{
  Serial.println("fonction check ...");
//  if (nombreDeClape == 1)
//  {
    myservo_bras.write(90);
    delay(200);
    myservo_roue_1.write(85);
    myservo_roue_2.write(95);
    delay(300);
    myservo_roue_1.write(95);
    myservo_roue_2.write(85);
    delay(300);
    myservo_bras.write(10);
    myservo_roue_1.write(90);
    myservo_roue_2.write(90);
//  }
//  else if (nombreDeClape == 2)
//  {
//    myservo_bras.write(90);
//    delay(2000);
//    myservo_roue_2.write(90);
//    delay(2000);
//    myservo_bras.write(10);
//  }
//  else if (nombreDeClape >= 3)
//  {
//    myservo_bras.write(90);
//    delay(2000);
//    myservo_roue_1.write(90);
//    delay(2000);
//    myservo_roue_2.write(90);
//    delay(2000);
//    myservo_bras.write(10);
//  }
  delay(500);
  Serial.println("fonction check END");
}

void loop()
{
    int soundValue = get_sound_value();
    Serial.print("Sound value > ");
    Serial.println(soundValue);

    // si un pic de son est detecté:
    if (soundValue > 600)
    {
        Serial.print(" ||||||| CLAP |||||||||| ");
        timeLastClap = millis();
        nombreDeClape += 1;
        if (nombreDeClape == 1)
        {
          printLCD("1 Clape...");
        }
        else if (nombreDeClape == 2)
        {
          printLCD("2 Clapes...");
        }
        else if (nombreDeClape == 3)
        {
            printLCD("3 Clapes.");
        }
    }
    // si un (ou plusieur) clape a ete entendu et que le temps d'attente est ecoulé
    if (nombreDeClape != 0 && millis() - timeLastClap > 1200) // temps d'attente écoulé
    {
        if (nombreDeClape == 1)
        {
            Serial.println("nombreDeClape == 1");
            set_leds_colors(RED);
            fonction_check();
            dancing();
            printLCD("Bien Joue");
        }
        else if (nombreDeClape == 2)
        {
            Serial.println("nombreDeClape == 2");
            set_leds_colors(BLUE);
            fonction_check();
            dancing();
            printLCD("Pas Mal");
        }
        else if (nombreDeClape >= 3)
        {
            Serial.print("nombreDeClape: ");
            Serial.println(nombreDeClape);
            set_leds_colors(GREEN);
            fonction_check();
            dancing();
            printLCD("Excellent!");
        }
        timeLastClap = 0;
        nombreDeClape = 0;
    }
    delay(20);
}
