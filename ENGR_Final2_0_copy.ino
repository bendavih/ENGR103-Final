#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay timer;

const byte buttonPinR = 5;
const byte buttonPinL = 4;
const byte switchPin = 7;
volatile bool switchFlag = false;
volatile bool buttonFlagR = false;
volatile bool buttonFlagL = false;
volatile bool switchState = false;

int speed = 10000;
volatile int noise;
volatile int rando;
volatile int score;

volatile bool yay;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE); // from my timer code
  attachInterrupt(digitalPinToInterrupt(4), buttonISRL, RISING); // from my timer code
  attachInterrupt(digitalPinToInterrupt(5), buttonISRR, RISING); // from my timer code

  CircuitPlayground.setBrightness(1);
  
  yay = true;
  score = 0;
}

void buttonISRR() {
  delay(5);
  buttonFlagR = true;
} // from my timer code

void buttonISRL() {
  delay(5);
  buttonFlagL = true;
} // from my timer code

void switchISR() {
  delay(5);
  switchState = !switchState;
  switchFlag = true;
} // sort of from my timer code

void lose(){
  //for (int j=0; j<11; j++) { // turns red
  //  CircuitPlayground.setPixelColor(j, 225,   0,   0);}
  CircuitPlayground.playTone(196, 200);
  score = 0;
  Serial.print("LOSE:");
  Serial.println(score);
  delay(2000);
  score = 0;
}

void point(){
  score++;
  Serial.print("POINT:");
  Serial.println(score);
}

void loop() {
  yay = true;
  if (switchState) {
  //  CircuitPlayground.clearPixels();
  //  delay(100);
    rando = random(1,4);
    timer.start(4000, AsyncDelay::MILLIS);
    switch (rando){ //picking a random color
      case 1: //                             green ---> left button (cord going up)
        for (int j=0; j<11; j++){
          CircuitPlayground.setPixelColor(j, 0,   225,   0);
        }
        while ((!timer.isExpired()) && (yay)) {
          if (digitalRead(buttonPinL) == HIGH) {
            point();
            yay = false;
          }
          if (digitalRead(buttonPinR) == HIGH){
            lose();
            yay = false;
          }
        }
        if (timer.isExpired()){
          lose();
        }
        CircuitPlayground.clearPixels();
        delay(100);
      break;
      case 2: //                             red ---> volume
        for (int j=0; j<11; j++){
          CircuitPlayground.setPixelColor(j, 225,   0,   0);
        }
        while ((!timer.isExpired()) && (yay)) {
          if (70 < CircuitPlayground.mic.soundPressureLevel(10)) {
            point();
            yay = false;
          }
          if (digitalRead(buttonPinL) == HIGH || digitalRead(buttonPinR) == HIGH){
            lose();
            yay = false;
          }
        }
        if (timer.isExpired()){
          lose();
        }
        CircuitPlayground.clearPixels();
        delay(100);
      break;
      case 3: //                             blue ---> right button (cord going up)
        for (int j=0; j<11; j++){
          CircuitPlayground.setPixelColor(j, 0,   0,   225);
        }
        while ((!timer.isExpired()) && (yay)) {
          if (digitalRead(buttonPinR) == HIGH) {
            point();
            yay = false;
          }
          if (digitalRead(buttonPinL) == HIGH){
            lose();
            yay = false;
          }
        }
        if (timer.isExpired()){
          lose();
        }
        CircuitPlayground.clearPixels();
        delay(100);
      break;
    }
  }
  else {
    CircuitPlayground.clearPixels();
  }
}