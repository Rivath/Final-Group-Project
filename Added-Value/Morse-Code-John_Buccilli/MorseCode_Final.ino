#include <Keypad.h>

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

void setup() {

  int randArrayKey[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,  // 16 potential rand values for 16 possible words and passwords
                        11, 12, 13, 14, 15
                       };

  const size_t o = sizeof(randArrayKey) / sizeof(randArrayKey[0]);

  for (size_t i = 0; i < o - 1; i++) {
    size_t j = random(1, o - i);

    int t = randArrayKey[i];
    randArrayKey[i] = randArrayKey[j];
    randArrayKey[j] = t;
  }
  randWord(randArrayKey[0]);  // Call randWord function with a random integer, setting the outputted morse code word and respective password
}

void loop() {

  errorCountSum = errorCountWires + errorCountKnob + errorCountMorse + errorCountSymbols;

  switch (errorCountSum) {
    case 1:
      oneSecond = 600UL;
      break;
    case 2:
      oneSecond = 200UL;
      break;
    case 3:
      matrix.writeDigitNum(0, 0);
      matrix.writeDigitNum(1, 0);
      matrix.writeDigitNum(3, 0);
      matrix.writeDigitNum(4, 0);
      matrix.blinkRate(1);
      delay(10000);
      for (;;) {}
      break;
  }

  char startPress = keypad.getKey();
  if (startPress == '*') { // User presses the '*' key to play the morse code word
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

  if (startPress == '#')  { // user presses the '#' key to signal that they are about to enter a password attempt
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

    //Compares users input to current password and outputs a green LED if the password is correct else it emits a tone
    for (int i = 0; i < 4; i++) {
      if (combo[i] == password.charAt(i)) { // Comparing passwords
        comboCorrect += 1;
      }
    }
    if (comboCorrect == 4) { // 5 Dots in a row after entering the password signifies a win
      MorseDot();
      MorseDot();
      MorseDot();
      MorseDot();
      MorseDot();
      winMorse = 1;
    }
    else { // else the errorcount is 1 and the clock moves faster
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

void MorseDot() // Morse code dot
{
  tone(audio, note, dotLen); // start playing a tone
  delay(dotLen);              // hold in this position
}

void MorseDash() // Morse code dash
{
  tone(audio, note, dashLen);  // start playing a tone
  delay(dashLen);               // hold in this position
}

// Turn off speaker tone
void Off(int delayTime)
{
  noTone(audio);             // stop playing a tone
  delay(delayTime);             // hold in this position
}

void randWord(int randNum) {  // Function that set the word and password by requesting an integer that coorisponds to a case
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
