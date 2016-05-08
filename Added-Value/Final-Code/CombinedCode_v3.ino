/*********************

   Symbol Puzzle Code
   John Kessinger

   Knob Puzzle Code
   Mark Sullivan

   Morse Puzzle Code
   John Buccilli

 **********************/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// On the Subject of Symbols                                                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define WHITE 0x7

//The byte images of the 8 custom characters we will be using to generate the symbols
byte newChar_0[8] = {
  B00000,
  B00000,
  B00000,
  B00111,
  B00111,
  B00100,
  B00100,
  B00100,
};

byte newChar_1[8] = {
  B00000,
  B00000,
  B00000,
  B11100,
  B11100,
  B00100,
  B00100,
  B00100
};

byte newChar_2[8] = {
  B00100,
  B00100,
  B00100,
  B00111,
  B00111,
  B00000,
  B00000,
  B00000
};

byte newChar_3[8] = {
  B00100,
  B00100,
  B00100,
  B11100,
  B11100,
  B00000,
  B00000,
  B00000
};

byte newChar_4[8] = {
  B00100,
  B00100,
  B00100,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};

byte newChar_5[8] = {
  B00100,
  B00100,
  B00100,
  B00111,
  B00111,
  B00100,
  B00100,
  B00100
};

byte newChar_6[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00100,
  B00100,
  B00100
};

byte newChar_7[8] = {
  B00100,
  B00100,
  B00100,
  B11100,
  B11100,
  B00100,
  B00100,
  B00100
};

//These varabiles each correspond to one of the byte images
int part_1;
int part_2;
int part_3;
int part_4;

//Each one of these variables is made of the 4 part varaibles
int symbol_1;
int symbol_2;
int symbol_3;
int symbol_4;

//These variables keep track of the button inputs
int countLeft;
int countUp;
int countRight;
int countDown;
int countSum; //This is used to determine the order the buttons are pressed

//After using the count varaibles the order is recorded and checked to determine if the correct input was made
int order_1;
int order_2;
int order_3;
int order_4;

//These varables check if each symbol is the the correct position as determined by the manual
int correct_1;
int correct_2;
int correct_3;
int correct_4;
int correctAll;

//These varables keep track of the errors for each game
int errorCountSum; //This varable keeps track of the total number of errors made
int errorCountKnob;
int errorCountWires;
int errorCountMorse;
int errorCountSymbols;

//Used to check the wire puzzle for completion or vailure depending on which variable is chosen
int checkWires = 0;
int wireError = 0;

//These varibles are used for the end win condition
int bigButton; //Checks that the big button was pressed
//If the button is pressed then these varables check if each puzzle has been completed correctly
int winWires;
int winKnob;
int winMorse;
int winSymbols;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// On the Subject of Knobs                                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int needPosition;//1 of 5 direction randomly calculated originally
int knobPosition;//1 of 5 directions calculated each loop
int knobPositionRead;//value read
int switchPosition;//current position of switch
int startingSwitchPosition;//initial position of switch
int positionIndex;//used in assigning the random combinations
int thresh1 = 172;//below is left, above is half-left
int thresh2 = 362;//below is half-left, above is up
int thresh3 = 493;//below is up, above is half-right
int thresh4 = 683;//below is half-right, above is right

//the possible 20 combinations the puzzle can choose to display
String lightsArray[20] = {"10001100", "01010010", "01001010", "01100001",
                          "00010101", "00001011", "11000100", "10000101",
                          "00000111", "01000110", "11010000", "01100010",
                          "00101100", "10100100", "00100110", "01100100",
                          "00010110", "01000101", "10100001", "10101000"
                         };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// On the Subject of Morse Code                                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Keypad.h>
int comboCorrect;
const byte rows = 4;
const byte cols = 4;

int toggle;

// Define the Keypad mapping
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pin assignments for keypad
byte rowPins[rows] = {48, 46, 44, 42};
byte colPins[cols] = {40, 38, 36, 34};


// Create an instance of the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols);

String MorseCodeWord = "";
String password = "";
char combo[4] = "";

int audio = 11;      // output audio from pin 11
int note = 1200;      // note/pitch

int dotLen = 200;     // length of the morse code dot
int dashLen = dotLen * 5;    // length of the morse code dash
int pause = dotLen * 7;     // length of the pause between characters

#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();

unsigned long previousSecondMillis = 0UL;
long oneSecond = 1000UL; // milliseconds per second

#define startMinute 5  //  Modify these defines to
#define startSecond 0 // change the timer interval
int minutes = startMinute;
int seconds = startSecond;

void setup() {
  Serial.begin(9600);
  matrix.begin(0x70);//4 digit 7 segment address
  pinMode(53, INPUT);//bigButton
  pinMode(14, INPUT);//checkWires
  digitalWrite(14, HIGH);
  pinMode(15, INPUT);//wireError
  digitalWrite(15, HIGH);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On the Subject of Symbols                                                                                                   //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //uses the bytes above to create new characters
  lcd.begin(16, 2);//gives size of lcd screen
  lcd.createChar(0, newChar_0);
  lcd.createChar(1, newChar_1);
  lcd.createChar(2, newChar_2);
  lcd.createChar(3, newChar_3);
  lcd.createChar(4, newChar_4);
  lcd.createChar(5, newChar_5);
  lcd.createChar(6, newChar_6);
  lcd.createChar(7, newChar_7);
  lcd.setBacklight(WHITE);//sets backlight color

  randomSeed(analogRead(A0));//used for rand function

  //used to generate additional randomness by shuffling an array
  //the rand function alone was not working to the level we wanted
  //source http://stackoverflow.com/questions/32413209/shuffle-an-array-in-arduino-software
  //this particular one selects one of the columns from the manual
  int randArray[] = {
    0, 1, 2, 3, 4, 5
  };

  const size_t n = sizeof(randArray) / sizeof(randArray[0]);

  for (size_t i = 0; i < n - 1; i++) {
    size_t j = random(1, n - i);

    int t = randArray[i];
    randArray[i] = randArray[j];
    randArray[j] = t;
  }

  //the same method as shown above
  //this particular one is used to select the random symbols from within the array chosen above
  const int symbolArray = randArray[0];

  int questionNumberArray[] = {
    0, 1, 2, 3, 4, 5, 6
  };

  const size_t m = sizeof(questionNumberArray) / sizeof(questionNumberArray[0]);

  for (size_t i = 0; i < m - 1; i++) {
    size_t j = random(1, m - i);

    int t = questionNumberArray[i];
    questionNumberArray[i] = questionNumberArray[j];
    questionNumberArray[j] = t;
  }

  //assigns the symbols
  symbol_1 = questionNumberArray[0];
  symbol_2 = questionNumberArray[1];
  symbol_3 = questionNumberArray[2];
  symbol_4 = questionNumberArray[3];

//this part assigns the correct parts needed to create the symbols selected
if (symbolArray == 0) {
array_1(symbol_1);
symbolCreate_1(part_1, part_2, part_3, part_4);
array_1(symbol_2);
symbolCreate_2(part_1, part_2, part_3, part_4);
array_1(symbol_3);
symbolCreate_3(part_1, part_2, part_3, part_4);
array_1(symbol_4);
symbolCreate_4(part_1, part_2, part_3, part_4);
}
if (symbolArray == 1) {
array_2(symbol_1);
symbolCreate_1(part_1, part_2, part_3, part_4);
array_2(symbol_2);
symbolCreate_2(part_1, part_2, part_3, part_4);
array_2(symbol_3);
symbolCreate_3(part_1, part_2, part_3, part_4);
array_2(symbol_4);
symbolCreate_4(part_1, part_2, part_3, part_4);
}
if (symbolArray == 2) {
array_3(symbol_1);
symbolCreate_1(part_1, part_2, part_3, part_4);
array_3(symbol_2);
symbolCreate_2(part_1, part_2, part_3, part_4);
array_3(symbol_3);
symbolCreate_3(part_1, part_2, part_3, part_4);
array_3(symbol_4);
symbolCreate_4(part_1, part_2, part_3, part_4);
}
if (symbolArray == 3) {
array_4(symbol_1);
symbolCreate_1(part_1, part_2, part_3, part_4);
array_4(symbol_2);
symbolCreate_2(part_1, part_2, part_3, part_4);
array_4(symbol_3);
symbolCreate_3(part_1, part_2, part_3, part_4);
array_4(symbol_4);
symbolCreate_4(part_1, part_2, part_3, part_4);
}
if (symbolArray == 4) {
array_5(symbol_1);
symbolCreate_1(part_1, part_2, part_3, part_4);
array_5(symbol_2);
symbolCreate_2(part_1, part_2, part_3, part_4);
array_5(symbol_3);
symbolCreate_3(part_1, part_2, part_3, part_4);
array_5(symbol_4);
symbolCreate_4(part_1, part_2, part_3, part_4);
}
if (symbolArray == 5) {
array_6(symbol_1);
symbolCreate_1(part_1, part_2, part_3, part_4);
array_6(symbol_2);
symbolCreate_2(part_1, part_2, part_3, part_4);
array_6(symbol_3);
symbolCreate_3(part_1, part_2, part_3, part_4);
array_6(symbol_4);
symbolCreate_4(part_1, part_2, part_3, part_4);
}

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On the Subject of Knobs                                                                                                     //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  needPosition = random(0, 4);
  positionIndex = random(0, 4);
  startingSwitchPosition = digitalRead(13);

  for (int i = 0; i < 8; i++) {
    if (lightsArray[needPosition * 4 + positionIndex].charAt(i) == '1') {
      digitalWrite(i + 2, HIGH);
    }
    else
      digitalWrite(i + 2, LOW);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On the Subject of Morse Code                                                                                                //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //the same sort of random number generator used in the symbol puzzle
  int randArrayKey[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                        11, 12, 13, 14, 15
                       };

  const size_t o = sizeof(randArrayKey) / sizeof(randArrayKey[0]);

  for (size_t i = 0; i < o - 1; i++) {
    size_t j = random(1, o - i);

    int t = randArrayKey[i];
    randArrayKey[i] = randArrayKey[j];
    randArrayKey[j] = t;
  }
  randWord(randArrayKey[0]);
}

uint8_t i = 0;

void loop() {

  bigButton = digitalRead(53);
  checkWires = digitalRead(14);
  wireError = digitalRead(15);

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

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On the Subject of Symbols                                                                                                   //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  uint8_t buttons = lcd.readButtons();
  countSum = countLeft + countUp + countRight + countDown;
  orderPressed(countSum, countLeft, countUp, countRight);

  //checks for button presses
  if (buttons) {
    if (buttons & BUTTON_LEFT) {
      countLeft = 1;
    }
    if (buttons & BUTTON_UP) {
      countUp = 1;
    }
    if (buttons & BUTTON_RIGHT) {
      countRight = 1;
    }
    if (buttons & BUTTON_DOWN) {
      countDown = 1;
    }
    if (buttons & BUTTON_SELECT) {
      winCon(order_1, order_2, order_3, order_4, symbol_1, symbol_2, symbol_3, symbol_4);
      if (correctAll == 1) {
        lcd.begin(16, 2);
        lcd.setCursor(4, 0);
        lcd.print("CORRECT!");
        winSymbols = 1;
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On the Subject of Knobs                                                                                                     //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  knobPositionRead = analogRead(0);
  switchPosition = digitalRead(13);

  if (switchPosition != startingSwitchPosition) {
    startingSwitchPosition = !startingSwitchPosition;
    winConKnob(knobPositionRead);
    if (knobPosition == needPosition) {
      for (int i = 0; i < 8; i++) { //turn on all LEDS
        winKnob = 1;
        digitalWrite(i + 2, HIGH);
        //player passed
      }
    }
    else {
      errorCountKnob = 1;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On the Subject of Morse Code                                                                                                //
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  char startPress = keypad.getKey();
  if (startPress == '*') {
    toggle = 1;
    if (toggle == 1) {
      // Loop through the string and get each character one at a time until the end is reached
      for (int i = 0; i < MorseCodeWord.length(); i++) {
        // Call the subroutine to get the morse code equivalent for this character
        CharToMorse(MorseCodeWord.charAt(i));
        Off(pause);
      }
      toggle = 0;
    }
  }

  if (startPress == '#')  {
    char key = keypad.getKey(); // temp variable for user input

    //Key 1 of the password
    key = keypad.waitForKey();
    delay(dotLen);
    combo[0] = key;

    //Key 2
    key = keypad.waitForKey();
    delay(dotLen);

    combo[1] = key;

    //Key 3
    key = keypad.waitForKey();
    delay(dotLen);
    combo[2] = key;

    //Key 4
    key = keypad.waitForKey();
    delay(dotLen);
    combo[3] = key;

    //Compares users input to current password and outputs series of tones not corresponding to any letter is correct
    for (int i = 0; i < 4; i++) {
      if (combo[i] == password.charAt(i)) { // Comparing passwords
        comboCorrect += 1;
      }
    }
    if (comboCorrect == 4) {
      MorseDot();
      MorseDot();
      MorseDot();
      MorseDot();
      MorseDot();
      winMorse = 1;
    }
    else {
      errorCountMorse = 1;
    }
  }

  if (bigButton) {
    if (winWires == 1 && winKnob == 1 && winMorse == 1 && winSymbols == 1) {
      Serial.println("Congrats you didn't explode");
    }
  }
  if (checkWires == LOW) {
    winWires = 1;
  }
  if (wireError == LOW) {
    errorCountWires = 1;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Assisting Methods                                                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// On the Subject of Symbols                                                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void symbolCreate_1(int part_1, int part_2, int part_3, int part_4) {
  //symbol 1
  lcd.setCursor(1, 0);
  lcd.write(part_1);
  lcd.setCursor(2, 0);
  lcd.write(part_2);
  lcd.setCursor(1, 1);
  lcd.write(part_3);
  lcd.setCursor(2, 1);
  lcd.write(part_4);
}

void symbolCreate_2(int part_1, int part_2, int part_3, int part_4) {
  //symbol 2
  lcd.setCursor(5, 0);
  lcd.write(part_1);
  lcd.setCursor(6, 0);
  lcd.write(part_2);
  lcd.setCursor(5, 1);
  lcd.write(part_3);
  lcd.setCursor(6, 1);
  lcd.write(part_4);
}

void symbolCreate_3(int part_1, int part_2, int part_3, int part_4) {
  //symbol 3
  lcd.setCursor(9, 0);
  lcd.write(part_1);
  lcd.setCursor(10, 0);
  lcd.write(part_2);
  lcd.setCursor(9, 1);
  lcd.write(part_3);
  lcd.setCursor(10, 1);
  lcd.write(part_4);
}

void symbolCreate_4(int part_1, int part_2, int part_3, int part_4) {
  //symbol 4
  lcd.setCursor(13, 0);
  lcd.write(part_1);
  lcd.setCursor(14, 0);
  lcd.write(part_2);
  lcd.setCursor(13, 1);
  lcd.write(part_3);
  lcd.setCursor(14, 1);
  lcd.write(part_4);
}

void array_1(int symbol) {
  switch (symbol) {
    case 0: // 27
      part_1 = 6;
      part_2 = 7;
      part_3 = 5;
      part_4 = 4;
      break;
    case 1: // 3
      part_1 = 0;
      part_2 = 1;
      part_3 = 3;
      part_4 = 2;
      break;
    case 2: // 17
      part_1 = 0;
      part_2 = 1;
      part_3 = 5;
      part_4 = 7;
      break;
    case 3: // 2
      part_1 = 1;
      part_2 = 0;
      part_3 = 2;
      part_4 = 3;
      break;
    case 4: // 9
      part_1 = 0;
      part_2 = 6;
      part_3 = 2;
      part_4 = 4;
      break;
    case 5: // 16
      part_1 = 5;
      part_2 = 7;
      part_3 = 2;
      part_4 = 3;
      break;
    case 6: // 13
      part_1 = 6;
      part_2 = 1;
      part_3 = 2;
      part_4 = 4;
      break;
  }
}

void array_2(int symbol) {
  switch (symbol) {
    case 0: // 12
      part_1 = 0;
      part_2 = 6;
      part_3 = 4;
      part_4 = 3;
      break;
    case 1: // 27
      part_1 = 6;
      part_2 = 7;
      part_3 = 5;
      part_4 = 4;
      break;
    case 2: // 13
      part_1 = 6;
      part_2 = 1;
      part_3 = 2;
      part_4 = 4;
      break;
    case 3: // 4
      part_1 = 1;
      part_2 = 0;
      part_3 = 3;
      part_4 = 2;
      break;
    case 4: // 1
      part_1 = 0;
      part_2 = 1;
      part_3 = 2;
      part_4 = 3;
      break;
    case 5: // 16
      part_1 = 5;
      part_2 = 7;
      part_3 = 2;
      part_4 = 3;
      break;
    case 6: // 5
      part_1 = 2;
      part_2 = 3;
      part_3 = 0;
      part_4 = 1;
      break;
  }
}

void array_3(int symbol) {
  switch (symbol) {
    case 0: // 18
      part_1 = 5;
      part_2 = 7;
      part_3 = 5;
      part_4 = 7;
      break;
    case 1: // 26
      part_1 = 5;
      part_2 = 7;
      part_3 = 4;
      part_4 = 4;
      break;
    case 2: // 4
      part_1 = 1;
      part_2 = 0;
      part_3 = 3;
      part_4 = 2;
      break;
    case 3: // 7
      part_1 = 0;
      part_2 = 4;
      part_3 = 2;
      part_4 = 6;
      break;
    case 4: // 11
      part_1 = 5;
      part_2 = 1;
      part_3 = 2;
      part_4 = 7;
      break;
    case 5: // 17
      part_1 = 0;
      part_2 = 1;
      part_3 = 5;
      part_4 = 7;
      break;
    case 6: // 1
      part_1 = 0;
      part_2 = 1;
      part_3 = 2;
      part_4 = 3;
      break;
  }
}

void array_4(int symbol) {
  switch (symbol) {
    case 0: // 21
      part_1 = 5;
      part_2 = 7;
      part_3 = 5;
      part_4 = 7;
      break;
    case 1: // 6
      part_1 = 5;
      part_2 = 7;
      part_3 = 4;
      part_4 = 4;
      break;
    case 2: // 20
      part_1 = 1;
      part_2 = 0;
      part_3 = 3;
      part_4 = 2;
      break;
    case 3: // 9
      part_1 = 0;
      part_2 = 4;
      part_3 = 2;
      part_4 = 6;
      break;
    case 4: // 7
      part_1 = 5;
      part_2 = 1;
      part_3 = 2;
      part_4 = 7;
      break;
    case 5: // 5
      part_1 = 0;
      part_2 = 1;
      part_3 = 5;
      part_4 = 7;
      break;
    case 6: // 10
      part_1 = 0;
      part_2 = 1;
      part_3 = 2;
      part_4 = 3;
      break;
  }
}

void array_5(int symbol) {
  switch (symbol) {
    case 0: // 14
      part_1 = 6;
      part_2 = 6;
      part_3 = 4;
      part_4 = 4;
      break;
    case 1: // 10
      part_1 = 0;
      part_2 = 7;
      part_3 = 5;
      part_4 = 3;
      break;
    case 2: // 20
      part_1 = 0;
      part_2 = 7;
      part_3 = 2;
      part_4 = 3;
      break;
    case 3: // 15
      part_1 = 4;
      part_2 = 4;
      part_3 = 6;
      part_4 = 6;
      break;
    case 4: // 6
      part_1 = 4;
      part_2 = 1;
      part_3 = 6;
      part_4 = 3;
      break;
    case 5: // 24
      part_1 = 4;
      part_2 = 5;
      part_3 = 6;
      part_4 = 5;
      break;
    case 6: // 8
      part_1 = 6;
      part_2 = 1;
      part_3 = 4;
      part_4 = 3;
      break;
  }
}

void array_6(int symbol) {
  switch (symbol) {
    case 0: // 21
      part_1 = 0;
      part_2 = 1;
      part_3 = 2;
      part_4 = 7;
      break;
    case 1: // 12
      part_1 = 0;
      part_2 = 6;
      part_3 = 4;
      part_4 = 3;
      break;
    case 2: // 22
      part_1 = 0;
      part_2 = 1;
      part_3 = 5;
      part_4 = 3;
      break;
    case 3: // 23
      part_1 = 5;
      part_2 = 1;
      part_3 = 2;
      part_4 = 3;
      break;
    case 4: // 14
      part_1 = 6;
      part_2 = 6;
      part_3 = 4;
      part_4 = 4;
      break;
    case 5: // 19
      part_1 = 7;
      part_2 = 5;
      part_3 = 7;
      part_4 = 5;
      break;
    case 6: // 25
      part_1 = 6;
      part_2 = 6;
      part_3 = 5;
      part_4 = 7;
      break;
  }
}

void orderPressed (int countSum, int countLeft, int countUp, int countRight) {
  switch (countSum) {
    case 1:
      if (countLeft == 1) {
        order_1 = 1;
      }
      else if (countUp == 1) {
        order_1 = 2;
      }
      else if (countRight == 1) {
        order_1 = 3;
      }
      else {
        order_1 = 4;
      }
      break;
    case 2:
      if (order_1 != 1 && countLeft == 1) {
        order_2 = 1;
      }
      else if (order_1 != 2 && countUp == 1) {
        order_2 = 2;
      }
      else if (order_1 != 3 && countRight == 1) {
        order_2 = 3;
      }
      else {
        order_2 = 4;
      }
      break;
    case 3:
      if (order_1 != 1 && order_2 != 1 && countLeft == 1) {
        order_3 = 1;
      }
      else if (order_1 != 2 && order_2 != 2 && countUp == 1) {
        order_3 = 2;
      }
      else if (order_1 != 3 && order_2 != 3 && countRight == 1) {
        order_3 = 3;
      }
      else {
        order_3 = 4;
      }
      break;
    case 4:
      if (order_1 != 1 && order_2 != 1 && order_3 != 1 && countLeft == 1) {
        order_4 = 1;
      }
      else if (order_1 != 2 && order_2 != 2 && order_3 != 2 && countUp == 1) {
        order_4 = 2;
      }
      else if (order_1 != 3 && order_2 != 3 && order_4 != 3 && countRight == 1) {
        order_4 = 3;
      }
      else {
        order_4 = 4;
      }
      break;
  }
}

void winCon (int order_1, int order_2, int order_3, int order_4, int symbol_1, int symbol_2, int symbol_3, int symbol_4) {
  switch (order_1) {
    case 1:
      if (symbol_1 < symbol_2 && symbol_1 < symbol_3  && symbol_1 < symbol_4) {
        correct_1 = 1;
      }
      break;
    case 2:
      if (symbol_2 < symbol_1 && symbol_2 < symbol_3  && symbol_2 < symbol_4) {
        correct_1 = 1;
      }
      break;
    case 3:
      if (symbol_3 < symbol_1 && symbol_3 < symbol_2  && symbol_3 < symbol_4) {
        correct_1 = 1;
      }
      break;
    case 4:
      if (symbol_4 < symbol_1 && symbol_4 < symbol_2  && symbol_4 < symbol_3) {
        correct_1 = 1;
      }
      break;
  }

  switch (order_2) {
    case 1:
      if (order_1 == 2 && symbol_1 < symbol_3 && symbol_1 < symbol_4) {
        correct_2 = 1;
      }
      if (order_1 == 3 && symbol_1 < symbol_2 && symbol_1 < symbol_4) {
        correct_2 = 1;
      }
      if (order_1 == 4 && symbol_1 < symbol_2 && symbol_1 < symbol_3) {
        correct_2 = 1;
      }
      break;
    case 2:
      if (order_1 == 1 && symbol_2 < symbol_3 && symbol_2 < symbol_4) {
        correct_2 = 1;
      }
      if (order_1 == 3 && symbol_2 < symbol_1 && symbol_2 < symbol_4) {
        correct_2 = 1;
      }
      if (order_1 == 4 && symbol_2 < symbol_1 && symbol_2 < symbol_3) {
        correct_2 = 1;
      }
      break;
    case 3:
      if (order_1 == 1 && symbol_3 < symbol_2 && symbol_3 < symbol_4) {
        correct_2 = 1;
      }
      if (order_1 == 2 && symbol_3 < symbol_1 && symbol_3 < symbol_4) {
        correct_2 = 1;
      }
      if (order_1 == 4 && symbol_3 < symbol_1 && symbol_3 < symbol_2) {
        correct_2 = 1;
      }
      break;
    case 4:
      if (order_1 == 1 && symbol_4 < symbol_2 && symbol_4 < symbol_3) {
        correct_2 = 1;
      }
      if (order_1 == 2 && symbol_4 < symbol_1 && symbol_4 < symbol_3) {
        correct_2 = 1;
      }
      if (order_1 == 3 && symbol_4 < symbol_1 && symbol_4 < symbol_2) {
        correct_2 = 1;
      }
      break;
  }

  switch (order_3) {
    case 1:
      if (order_4 == 2 && symbol_1 > symbol_3 && symbol_1 > symbol_4) {
        correct_3 = 1;
      }
      if (order_4 == 3 && symbol_1 > symbol_2 && symbol_1 > symbol_4) {
        correct_3 = 1;
      }
      if (order_4 == 4 && symbol_1 > symbol_2 && symbol_1 > symbol_3) {
        correct_3 = 1;
      }
      break;
    case 2:
      if (order_4 == 1 && symbol_2 > symbol_3 && symbol_2 > symbol_4) {
        correct_3 = 1;
      }
      if (order_4 == 3 && symbol_2 > symbol_1 && symbol_2 > symbol_4) {
        correct_3 = 1;
      }
      if (order_4 == 4 && symbol_2 > symbol_1 && symbol_2 > symbol_3) {
        correct_3 = 1;
      }
      break;
    case 3:
      if (order_4 == 1 && symbol_3 > symbol_2 && symbol_3 > symbol_4) {
        correct_3 = 1;
      }
      if (order_4 == 2 && symbol_3 > symbol_1 && symbol_3 > symbol_4) {
        correct_3 = 1;
      }
      if (order_4 == 4 && symbol_3 > symbol_1 && symbol_3 > symbol_2) {
        correct_3 = 1;
      }
      break;
    case 4:
      if (order_4 == 1 && symbol_4 > symbol_2 && symbol_4 > symbol_3) {
        correct_3 = 1;
      }
      if (order_4 == 2 && symbol_4 > symbol_1 && symbol_4 > symbol_3) {
        correct_3 = 1;
      }
      if (order_4 == 3 && symbol_4 > symbol_1 && symbol_4 > symbol_2) {
        correct_3 = 1;
      }
      break;
  }
  switch (order_4) {
    case 1:
      if (symbol_1 > symbol_2 && symbol_1 > symbol_3  && symbol_1 > symbol_4) {
        correct_4 = 1;
      }
      break;
    case 2:
      if (symbol_2 > symbol_1 && symbol_2 > symbol_3  && symbol_2 > symbol_4) {
        correct_4 = 1;
      }
      break;
    case 3:
      if (symbol_3 > symbol_1 && symbol_3 > symbol_2  && symbol_3 > symbol_4) {
        correct_4 = 1;
      }
      break;
    case 4:
      if (symbol_4 > symbol_1 && symbol_4 > symbol_2  && symbol_4 > symbol_3) {
        correct_4 = 1;
      }
      break;
  }
  if (correct_1 == 1 && correct_2 == 1 && correct_3 == 1 && correct_4 == 1) {
    correctAll = 1;
  }
  else {
    countLeft = 0;
    countUp = 0;
    countRight = 0;
    countDown = 0;
    countSum = 0;
    order_1 = 0;
    order_2 = 0;
    order_3 = 0;
    order_4 = 0;
    orderPressed(countSum, countLeft, countUp, countRight);
    errorCountSymbols = 1;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// On the Subject of Knobs                                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void winConKnob(int knobPositionRead) {
  if (knobPositionRead <= thresh1) {
    Serial.println("Left");
    knobPosition = 0;
  }
  else if (knobPositionRead >= thresh1 && knobPositionRead <= thresh2) {
    Serial.println("Half Left");
    knobPosition = 1;
  }
  else if (knobPositionRead >= thresh2 && knobPositionRead <= thresh3) {
    Serial.println("Up");
    knobPosition = 2;
  }
  else if (knobPositionRead >= thresh3 && knobPositionRead <= thresh4) {
    Serial.println("Half Right");
    knobPosition = 3;
  }
  else {
    Serial.println("Right");
    knobPosition = 4;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// On the Subject of Morse Code                                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MorseDot()
{
  tone(audio, note, dotLen); // start playing a tone
  delay(dotLen);              // hold in this position
}

void MorseDash()
{
  tone(audio, note, dashLen);  // start playing a tone
  delay(dashLen);               // hold in this position
}

// Turn Off
void Off(int delayTime)
{
  noTone(audio);             // stop playing a tone
  delay(delayTime);             // hold in this position
}

void randWord(int randNum) {
  switch (randNum) {

    case 0:
      MorseCodeWord = "ibex";
      password = "5D21";
      break;

    case 1:
      MorseCodeWord = "kale";
      password = "06AC";
      break;

    case 2:
      MorseCodeWord = "evil";
      password = "9280";
      break;

    case 3:
      MorseCodeWord = "doom";
      password = "7B91";
      break;

    case 4:
      MorseCodeWord = "coal";
      password = "A86B";
      break;

    case 5:
      MorseCodeWord = "mark";
      password = "1009";
      break;

    case 6:
      MorseCodeWord = "chew";
      password = "4DB0";
      break;

    case 7:
      MorseCodeWord = "john";
      password = "6CB5";
      break;

    case 8:
      MorseCodeWord = "bang";
      password = "DAC3";
      break;

    case 9:
      MorseCodeWord = "tock";
      password = "350D";
      break;

    case 10:
      MorseCodeWord = "tick";
      password = "86A7";
      break;

    case 11:
      MorseCodeWord = "boom";
      password = "C4B2";
      break;

    case 12:
      MorseCodeWord = "time";
      password = "230A";
      break;

    case 13:
      MorseCodeWord = "wire";
      password = "61D1";
      break;

    case 14:
      MorseCodeWord = "bomb";
      password = "B392";
      break;

    case 15:
      MorseCodeWord = "done";
      password = "9C14";
      break;
  }
}

// Character Conversion to Morse Code
void CharToMorse(char letter) {
  // Take the passed character and use a switch case to find the morse code for that character
  switch (letter) {

    case 'a':
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'b':
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'c':
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'd':
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'e':
      MorseDot();
      Off(dotLen);
      break;

    case 'f':
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'g':
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'h':
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'i':
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'j':
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'k':
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'l':
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'm':
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'n':
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'o':
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'p':
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 'q':
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'r':
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 's':
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;

    case 't':
      MorseDash();
      Off(dotLen);
      break;

    case 'u':
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'v':
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'w':
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'x':
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'y':
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      break;

    case 'z':
      MorseDash();
      Off(dotLen);
      MorseDash();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      MorseDot();
      Off(dotLen);
      break;
    default:
      break;
      // default is a blank space
  }
}
