#include <Wire.h>
#include "rgb_lcd.h"

#define PIR_MOTION_SENSOR_1 12
#define PIR_MOTION_SENSOR_2 2
#define MELODYPIN 4
#define REDGAMELLEBUTTON 10
#define BLUEGAMELLEBUTTON 6

//Gamelle rouge a le bouton poussoire
//Gamelle bleue a le bouton tactile

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

int scoreRed;
int scoreBlue;


void setup()
{
    pinMode(PIR_MOTION_SENSOR_1, INPUT);
    pinMode(PIR_MOTION_SENSOR_2, INPUT);

    pinMode(REDGAMELLEBUTTON, INPUT);
    pinMode(BLUEGAMELLEBUTTON, INPUT);

    pinMode(MELODYPIN, OUTPUT);

    //Init score + écran
    initialisation();

    //Afichage du score 
    displayScore();
}

void loop()
{

    redGamelleButtonState = digitalRead(REDGAMELLEBUTTON);
    blueGamelleButtonState = digitalRead(BLUEGAMELLEBUTTON);

    //Button gamelle
    if (redGamelleButtonState == HIGH) {
        scoreRed = gamelle(scoreRed);
        
        displayScore();
    }

    if(blueGamelleButtonState==1){
        //TODO : voir si nécessaire
        blueGamelleButtonState = 0;

        scoreBlue = gamelle(scoreBlue);
        
        displayScore();
    }

  
    if(isGoalDetected(PIR_MOTION_SENSOR_1)){
        lcd.setRGB(248, 16, 16);
        scoreRed++;
        
        displayScore();

        goalMelody();

        hasWin(scoreRed, "Rouge");
    }

    if(isGoalDetected(PIR_MOTION_SENSOR_2)){
        lcd.setRGB(42, 56, 211);
        scoreBlue++;

        displayScore();

        goalMelody();

        hasWin(scoreBlue, "Bleu");
        
    }

    delay(5000);
    lcd.setRGB(155, 155, 155);

}

void hasWin(int score, String team){
    if(score == 10){
        lcd.clear();
        if(team == "Rouge"){
          lcd.setRGB(248, 16, 16);
        }
        else{
          lcd.setRGB(42, 56, 211);
        }
        
        lcd.print(team+ " gagne!");
        lcd.blinkLED();

        winMelody();
        delay(20000);
        
        setup();
    }
}

int gamelle(int score){
    score = score - 1;
    lcd.clear();
    lcd.print("Gamelle !");
    delay(1000);
    return score;
}

void displayScore(){
    lcd.clear();
    lcd.print("Rouge / Bleu ");
    lcd.setCursor(0, 1);
    lcd.print(String(scoreRed));
    lcd.setCursor(11, 1);
    lcd.print(String(scoreBlue));
}

boolean isGoalDetected(int SENSOR){
  
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

void initialisation(){

    scoreBlue = 0;
    scoreRed = 0;
    
    //Initialisation de l'écran
    lcd.begin(16, 2);
    lcd.noBlinkLED();
    lcd.clear();
    lcd.setRGB(0, 255, 0);
    lcd.print("Get ready !");
    delay(3000);
}

// Winning melody
void winMelody() {
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
void goalMelody() {
  int size = sizeof(melodyGoal) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / melodyGoalTempo[thisNote];
    buzz(MELODYPIN, melodyGoal[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    buzz(MELODYPIN, 0, noteDuration);
  }
}
