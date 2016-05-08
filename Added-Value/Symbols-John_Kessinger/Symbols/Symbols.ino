/*********************

   Symbol Puzzle Code
   John Kessinger

 **********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define WHITE 0x7

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

int part_1;
int part_2;
int part_3;
int part_4;

int symbol_1;
int symbol_2;
int symbol_3;
int symbol_4;

int countLeft;
int countUp;
int countRight;
int countDown;
int countSum;

int order_1;
int order_2;
int order_3;
int order_4;

int correct_1;
int correct_2;
int correct_3;
int correct_4;
int correctAll;

int tries;
int errorCount;

void setup() {
  // Debugging output
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(0, newChar_0);
  lcd.createChar(1, newChar_1);
  lcd.createChar(2, newChar_2);
  lcd.createChar(3, newChar_3);
  lcd.createChar(4, newChar_4);
  lcd.createChar(5, newChar_5);
  lcd.createChar(6, newChar_6);
  lcd.createChar(7, newChar_7);
  lcd.setBacklight(WHITE);

  randomSeed(analogRead(A0));

  // assign a random number from 0 to 5
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

  const int symbolArray = randArray[0];
  //Serial.println(symbolArray);

  //http://stackoverflow.com/questions/32413209/shuffle-an-array-in-arduino-software
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

  symbol_1 = questionNumberArray[0];
  symbol_2 = questionNumberArray[1];
  symbol_3 = questionNumberArray[2];
  symbol_4 = questionNumberArray[3];

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
}

uint8_t i = 0;

void loop() {
  uint8_t buttons = lcd.readButtons();
  countSum = countLeft + countUp + countRight + countDown;
  orderPressed(countSum, countLeft, countUp, countRight);

  if (buttons) {
    if (buttons & BUTTON_LEFT) {
      countLeft = 1;
     Serial.println("Left");
      Serial.println(order_1);
      Serial.println(order_2);
      Serial.println(order_3);
     Serial.println(order_4);
    }
    if (buttons & BUTTON_UP) {
      countUp = 1;
      Serial.println("Up");
      Serial.println(order_1);
      Serial.println(order_2);
      Serial.println(order_3);
      Serial.println(order_4);
    }
    if (buttons & BUTTON_RIGHT) {
      countRight = 1;
      Serial.println("Right");
      Serial.println(order_1);
      Serial.println(order_2);
      Serial.println(order_3);
      Serial.println(order_4);
    }
    if (buttons & BUTTON_DOWN) {
      countDown = 1;
      Serial.println("Down");
      Serial.println(order_1);
      Serial.println(order_2);
      Serial.println(order_3);
      Serial.println(order_4);
    }
    if (buttons & BUTTON_SELECT) {
      Serial.println("Checking win...");
      winCon(order_1, order_2, order_3, order_4, symbol_1, symbol_2, symbol_3, symbol_4);
      if (correctAll == 1) {
        Serial.println("Correct");
      }
    }
  }
}

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
    errorCount = 1;
    Serial.println("Wrong! Button Entries Reset");
    Serial.println(errorCount);
    //Serial.println(order_1);
    //Serial.println(order_2);
    //Serial.println(order_3);
    //Serial.println(order_4);
  }
}

