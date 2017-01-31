#include <Wire.h>
#include "rgb_lcd.h"

#define PIR_MOTION_SENSOR_1 10
#define PIR_MOTION_SENSOR_2 2
#define MELODYPIN 4
#define REDGAMELLEBUTTON 8
#define BLUEGAMELLEBUTTON 6

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_D5  587
#define NOTE_A5  880


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

rgb_lcd lcd;

int redGamelleButtonState = 0;  
int blueGamelleButtonState = 0;

int scoreRed = 0;
int scoreBlue = 0;


void setup()
{
    pinMode(PIR_MOTION_SENSOR_1, INPUT);
    pinMode(PIR_MOTION_SENSOR_2, INPUT);

    pinMode(REDGAMELLEBUTTON, INPUT);
    pinMode(BLUEGAMELLEBUTTON, INPUT);
    

    lcd.begin(16, 2);
    lcd.noBlinkLED();
    lcd.clear();
    lcd.setRGB(0, 255, 0);
    lcd.print("Get ready !");
    delay(3000);
    lcd.clear();
    lcd.print("Rouge / Bleu ");
    lcd.setCursor(0, 1);
    lcd.print(String(scoreRed));
    lcd.setCursor(11, 1);
    lcd.print(String(scoreBlue));

    pinMode(MELODYPIN, OUTPUT);


}

void loop()
{

    redGamelleButtonState = digitalRead(REDGAMELLEBUTTON);
    blueGamelleButtonState = digitalRead(BLUEGAMELLEBUTTON);

    //Button gamelle
    if (redGamelleButtonState == HIGH) {
        scoreRed = scoreRed - 1;
        lcd.clear();
        lcd.print("Gamelle !");
        delay(1000);
        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(scoreRed));
        lcd.setCursor(11, 1);
        lcd.print(String(scoreBlue));
        delay(2000);
        
    }

    if(blueGamelleButtonState==1){
        blueGamelleButtonState = 0;
        scoreBlue = scoreBlue - 1;
        lcd.clear();
        lcd.print("Gamelle !");
        delay(1000);
        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(scoreRed));
        lcd.setCursor(11, 1);
        lcd.print(String(scoreBlue));
        delay(2000);
    }

  
    if(isGoalDetected(PIR_MOTION_SENSOR_1)){
        lcd.setRGB(255, 0, 0);
        scoreRed++;

        
        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(scoreRed));
        lcd.setCursor(11, 1);
        lcd.print(String(scoreBlue));

        goal();

        if(scoreRed == 10){
          lcd.clear();
          lcd.setRGB(255, 0, 0);
          lcd.print("Rouge gagne!");
          lcd.blinkLED();

          win();
          
          delay(100000);
        }

        delay(5000);

        lcd.setRGB(155, 155, 155);

        
    }

       if(isGoalDetected(PIR_MOTION_SENSOR_2)){
        lcd.setRGB(0, 0, 255);
        scoreBlue++;

        lcd.clear();
        lcd.print("Rouge / Bleu ");
        lcd.setCursor(0, 1);
        lcd.print(String(scoreRed));
        lcd.setCursor(11, 1);
        lcd.print(String(scoreBlue));

        goal();

        if(scoreBlue == 10){
          lcd.clear();
          lcd.setRGB(0, 0, 255);
          lcd.print("Bleu gagne!");
          lcd.blinkLED();

          win();
          
          delay(100000);
        }
        
        delay(5000);

        lcd.setRGB(155, 155, 155);
    }

}


boolean isGoalDetected(int SENSOR)
{
    int sensorValue = digitalRead(SENSOR);
    if(sensorValue == HIGH)
    {
        Serial.println("Detected");
        return true;
    }
    else
    {
        return false;
    }
}



void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000; 
  for (long i = 0; i < numCycles; i++) { 
    digitalWrite(targetPin, HIGH); 
    delayMicroseconds

(delayValue); 
    digitalWrite(targetPin, LOW); 
    delayMicroseconds(delayValue); 
  }
  digitalWrite(13, LOW);

}

// Winning melody
void win() {
  int size = sizeof(melodyWin) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / melodyWinTempo[thisNote];
    buzz(MELODYPIN, melodyWin[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    buzz(MELODYPIN, 0, noteDuration);
  }
    
}

// Goal melody
void goal() {
  int size = sizeof(melodyGoal) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / melodyGoalTempo[thisNote];
    buzz(MELODYPIN, melodyGoal[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    buzz(MELODYPIN, 0, noteDuration);
  }
}
