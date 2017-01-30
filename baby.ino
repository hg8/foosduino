/*macro definitions of PIR motion sensor pin and LED pin*/
#define PIR_MOTION_SENSOR_1 2//Use pin 2 to receive the signal from the module
#define PIR_MOTION_SENSOR_2 10

#include <ChainableLED.h>
#include <Wire.h>
#include "rgb_lcd.h"

#define NUM_LEDS  1

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_D5  587
#define NOTE_A5  880

#define melodyPin 4

 
ChainableLED ledTeam1(4, 5, NUM_LEDS);
ChainableLED ledTeam2(12, 13, NUM_LEDS);
rgb_lcd lcd;
const int redGamelleButton = 8; 
const int blueGamelleButton=6;
const int ledPin =  13;      // the number of the LED pin


int redGamelleButtonState = 0;  
int blueGamelleButtonState = 0;

float hue = 0.0;
int score1 = 0;
int score2 = 0;

int melodyGoal[] = {
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_D5
};

int melodyGoalTempo[] = {
 15, 15, 15, 1 
};

int melodyWin[] = {
  NOTE_E4, NOTE_C4, NOTE_A4, NOTE_A5, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B4
};

int melodyWinTempo[] = {
 3, 9, 6, 6, 3, 9, 9, 9, 9, 9, 3
};



void setup()
{
    pinMode(PIR_MOTION_SENSOR_1, INPUT);
    pinMode(PIR_MOTION_SENSOR_2, INPUT);

    //Rouge gamelle button
    pinMode(redGamelleButton, INPUT);

    pinMode(blueGamelleButton, INPUT);
    
    ledTeam1.init();
    ledTeam2.init();
    
    ledTeam1.setColorRGB(0, 0, 255, 0);
    ledTeam2.setColorRGB(0, 0, 255, 0);

    

    lcd.begin(16, 2);
    lcd.noBlinkLED();
    lcd.clear();
    lcd.setRGB(0, 255, 0);
    lcd.print("Get ready !");
    delay(3000);
    lcd.clear();
    lcd.print("Rouge / Bleu ");
    lcd.setCursor(0, 1);
    lcd.print(String(score1));
    lcd.setCursor(11, 1);
    lcd.print(String(score2));

    pinMode(melodyPin, OUTPUT);


}

void loop()
{

    redGamelleButtonState = digitalRead(redGamelleButton);
    blueGamelleButtonState = digitalRead(blueGamelleButton);

    //Button gamelle
    if (redGamelleButtonState == HIGH) {
        // turn LED on:
        score1 = score1 - 1;
        digitalWrite(ledPin, LOW);
        lcd.clear();
        lcd.print("Gamelle !");
        delay(1000);
        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(score1));
        lcd.setCursor(11, 1);
        lcd.print(String(score2));
        delay(2000);
        
    }

    if(blueGamelleButtonState==1){
        blueGamelleButtonState = 0;
        score2 = score2 - 1;
        digitalWrite(ledPin, LOW);
        lcd.clear();
        lcd.print("Gamelle !");
        delay(1000);
        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(score1));
        lcd.setCursor(11, 1);
        lcd.print(String(score2));
        delay(2000);
    }

  
    if(isGoalDetected(PIR_MOTION_SENSOR_1)){//if it detects the moving people?
        ledTeam1.setColorRGB(0, 255, 0, 0);
        lcd.setRGB(255, 0, 0);
        score1++;
        ledTeam1.setColorRGB(0, 0, 0, 255);

        goal();

        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(score1));
        lcd.setCursor(11, 1);
        lcd.print(String(score2));

        if(score1 == 10){
          lcd.clear();
          lcd.setRGB(255, 0, 0);
          lcd.print("Rouge gagne!");
          lcd.blinkLED();

          win();
          
          delay(100000);
        }

        delay(1000);

        lcd.setRGB(155, 155, 155);

        
    }

       if(isGoalDetected(PIR_MOTION_SENSOR_2)){//if it detects the moving people?
        ledTeam2.setColorRGB(0, 255, 0, 0);
        lcd.setRGB(0, 0, 255);
        score2++;
        ledTeam2.setColorRGB(0, 0, 0, 255);

        goal();

        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(score1));
        lcd.setCursor(11, 1);
        lcd.print(String(score2));

        if(score2 == 10){
          lcd.clear();
          lcd.setRGB(0, 0, 255);
          lcd.print("Bleu gagne!");
          lcd.blinkLED();

          win();
          
          delay(100000);
        }
        
        delay(1000);

        lcd.setRGB(0, 255, 0);
    }

}

/***************************************************************/
/*Function: Detect whether anyone moves in it's detecting range*/
/*Return:-boolean, true is someone detected.*/
boolean isGoalDetected(int SENSOR)
{
    int sensorValue = digitalRead(SENSOR);
    if(sensorValue == HIGH)//if the sensor value is HIGH?
    {
        Serial.println("Detected");
        return true;//yes,return true
    }
    else
    {
        return false;//no,return false
    }
}


void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds

(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);

}

void win() {
  // iterate over the notes of the melody:
  Serial.println(" 'Goal'");
  int size = sizeof(melodyWin) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / melodyWinTempo[thisNote];

    buzz(melodyPin, melodyWin[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(melodyPin, 0, noteDuration);
  }
    
}


void goal() {
  // iterate over the notes of the melody:
  Serial.println(" 'Goal'");
  int size = sizeof(melodyGoal) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / melodyGoalTempo[thisNote];

    buzz(melodyPin, melodyGoal[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(melodyPin, 0, noteDuration);
  }
}

