#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "rgb_lcd.h"


//define PIN
#define SOUND_SENSOR_PIN  A2
#define LED_PIN  A0

//define params
#define LED_COUNT 3
 
// define colors
#define RED 1 
#define GREEN 2
#define BLUE 3
#define NOPE 0


//Variable temps du dernier clap(pour mesurer le temps quil rest avant de compter)
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
  //Servo myservo_roue_bras;

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
    
    // initialisation ecran LCD
    lcd.begin(16, 2);
    lcd.setRGB(255, 255, 255);
    lcd.print("setup OK");
    
}

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

void dancing(void)
{
  myservo_roue_1.write(80);
  myservo_roue_2.write(80);
  delay(300);
  myservo_roue_1.write(100);
  myservo_roue_2.write(100);
  delay(1000);
  myservo_roue_1.write(90);
  myservo_roue_2.write(90);

}

void printLCD(char *str)
{
          lcd.clear();
          lcd.home();
          lcd.print(str);
}

void loop()
{
    int soundValue = get_sound_value();
    if (soundValue > 600)
    {
        Serial.print(" ||||||||||||||||| ");
        Serial.println(soundValue);
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
    

    if (nombreDeClape != 0 && millis() - timeLastClap > 1200) // temps d'attente écoulé
    {
        if (nombreDeClape == 1)
        {
            Serial.println("nombreDeClape == 1");
            set_leds_colors(RED);
            printLCD("Bien Joue");
        }
        else if (nombreDeClape == 2)
        {
            Serial.println("nombreDeClape == 2");
            set_leds_colors(BLUE);
 
            printLCD("Pas Mal");
        }
        else if (nombreDeClape >= 3)
        {
            Serial.print("nombre de clape: ");
            Serial.println(nombreDeClape);
            
            set_leds_colors(GREEN);
            printLCD("Excellent!");
            dancing();
        }
        timeLastClap = 0;
        nombreDeClape = 0;
    }
    delay(20);
}
