/***********************************************


************************************************/

#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();

unsigned long previousSecondMillis = 0UL;
long oneSecond = 1000UL; // milliseconds per second
long

#define startMinute 5
#define startSecond 00
int minutes = startMinute;
int seconds = startSecond;

void setup()
{
  matrix.begin(0x70);
}

void loop() {

  if (millis() - previousSecondMillis >= oneSecond) {
    //the countdown timer used the millis() function then converts to minutes and seconds
    if (millis() - previousSecondMillis >= oneSecond) {

      matrix.writeDigitNum(0, (minutes / 10));
      matrix.writeDigitNum(1, (minutes % 10));
      matrix.writeDigitNum(3, (seconds / 10));
      matrix.writeDigitNum(4, (seconds % 10));
      matrix.drawColon(true);
      matrix.writeDisplay();

      //if times runs out -> display starts flashing indicating a failure
      if (seconds-- == 0) {
        if (minutes == 0) {
          matrix.writeDigitNum(0, 0);
          matrix.writeDigitNum(1, 0);
          matrix.writeDigitNum(3, 0);
          matrix.writeDigitNum(4, 0);
          matrix.blinkRate(1);
          delay(10000);
          for (;;) {}
        } else {
          minutes -= 1;
          seconds = 59;
        }
      }
      previousSecondMillis += oneSecond;
    }

    //checks the total number of errors
    errorCountSum = errorCountWires + errorCountKnob + errorCountMorse + errorCountSymbols;

    switch (errorCountSum) {
      case 1: //if one error clock speed increases
        oneSecond = 600UL;
        break;
      case 2: //if two errors clock speed increases more
        oneSecond = 200UL;
        break;
      case 3: //if three errors game is over and time stops
        matrix.writeDigitNum(0, 0);
        matrix.writeDigitNum(1, 0);
        matrix.writeDigitNum(3, 0);
        matrix.writeDigitNum(4, 0);
        matrix.blinkRate(1);
        delay(10000);
        for (;;) {}
        break;
    }
  }



