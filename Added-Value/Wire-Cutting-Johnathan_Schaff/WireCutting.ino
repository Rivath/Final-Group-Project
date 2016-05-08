#define DEBUG//Debug Statement for controlling the Serial Communication
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h> //Touchscreen library for registering touches

//LCD Pins that are necessary for display
#define LCD_RESET A4
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

//Touch Pins requred for determining touch
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//Threshold Values for evaluating pressure
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
//Objects controlling the touchscreen and regular screen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
//#define ROZ     0xFD20
#define ROZ     0xFBE0
#define GRI     0xBDF7

#define PENRADIUS 3//Width of the pen on the screen if a touch is registered
#define MINPRESSURE 10//Minimum pressure (z) for screen
#define MAXPRESSURE 1000//Maximum pressure (z) for screen

boolean wire_cut; int wireCut; //Boolean Value for if a wire has been cut or not. The int for wireCut is determined once a wire has been cut
int correctWire;//Integer representing the correct wire that should be cut to complete the puzzle
int errors = 0;//counts the number of times the incorrect wire has been cut

//Wire Characters for each wire sequence
char wire1; uint16_t wireColor1;
char wire2; uint16_t wireColor2;
char wire3; uint16_t wireColor3;
char wire4; uint16_t wireColor4;

void setup(void) {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(("Wire Cutting"));
#endif // DEBUG
  tft.reset();

  uint16_t identifier = tft.readID();

  if (identifier == 0x9325)
  {
#ifdef DEBUG
    Serial.println(F("Found ILI9325 LCD driver"));
  }
  else if (identifier == 0x9328) {

    Serial.println(F("Found ILI9328 LCD driver"));
  } else if (identifier == 0x7575) {

    Serial.println(F("Found HX8347G LCD driver"));
  } else if (identifier == 0x9341) {

    Serial.println(F("Found ILI9341 LCD driver"));
  } else if (identifier == 0x8357) {

    Serial.println(F("Found HX8357D LCD driver"));
#endif  DEBUG
  } else {
#ifdef DEBUG
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.print(F("I try use ILI9341 LCD driver "));
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
#endif  DEBUG
    identifier = 0x9341;
  }
  tft.begin(identifier);

  // assign a random number from 1 to 24, this corresponds to a certain wire combination to be generated
  int randArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};//24 combinations for wires
  const size_t n = sizeof(randArray) / sizeof(randArray[0]);
  for (size_t i = 0   ; i < n - 1; i++) {
    size_t j = random(1, n - i);
    int t = randArray[i];
    randArray[i] = randArray[j];
    randArray[j] = t;
  }//end of shuffling process

  randomSeed(analogRead(A5));//Randomizes the placement to be taken from the randomArray if A5 is free for use
  int randomPlacement = random(0, 23);//random point to take from the randomized array
  const int sequence = randArray[randomPlacement];
  wireColorGeneration(sequence);//Creates the wires to be created on the screen
  determineWireColor();//Determines the correct wire color
  Serial.println(wire1); Serial.println(wire2);
  Serial.println(wire3); Serial.println(wire4);
  Serial.println(sequence);

  //Game Logic Tree
  //Situation with at least one wire that is Red and the last wire being Green
  if ((wire1 == 'R' | wire2 == 'R' | wire3 == 'R' | wire4 == 'R') & wire4 == 'G')
  {
    correctWire = 4;
    Serial.println("Last 1");//Cut Last Wire
  }
  //Situation with at least one wire that is Red and the last wire isn't Green and a blue wire
  else if ((wire1 == 'R' | wire2 == 'R' | wire3 == 'R' | wire4 == 'R') & wire4 != 'G')
  {
    int blueWireCount = 0;
    if (wire1 == 'B')
    {
      blueWireCount++;
    }

    if (wire2 == 'B')
    {
      blueWireCount++;
    }

    if (wire3 == 'B')
    {
      blueWireCount++;
    }

    if (wire4 == 'B')
    {
      blueWireCount++;
    }


    if (blueWireCount <= 1)
    {
      correctWire = 3;
      Serial.println("Third 2");//Cut Third Wire
    }

    else if (blueWireCount > 1)
    {
      if (wire4 == 'B')
      {
        correctWire = 4;
      }

      else if (wire3 == 'B')
      {
        correctWire = 3;
      }

      else if (wire2 == 'B')
      {
        correctWire = 2;
      }

      else
      {
        correctWire = 1;
      }
      Serial.println("Last Blue 3");//Cut Last Blue wire
    }
  }
  //Situation for no Red wires and the last wire is yellow
  else if ((wire1 != 'R' & wire2 != 'R' & wire3 != 'R' & wire4 != 'R') & wire4 == 'Y')
  {
    correctWire = 1;
    Serial.println("First 4");//Cut First Wire
  }
  //Covers all other situations
  else
  {
    int blueWireCount = 0;
    if (wire1 == 'B')
    {
      blueWireCount++;
    }

    if (wire2 == 'B')
    {
      blueWireCount++;
    }

    if (wire3 == 'B')
    {
      blueWireCount++;
    }

    if (wire4 == 'B')
    {
      blueWireCount++;
    }

    int yellowWireCount = 0;
    if (wire1 == 'Y')
    {
      yellowWireCount++;
    }

    if (wire2 == 'Y')
    {
      yellowWireCount++;
    }

    if (wire3 == 'Y')
    {
      yellowWireCount++;
    }
    if (wire4 == 'Y')
    {
      yellowWireCount++;
    }

    Serial.println(blueWireCount);
    if (blueWireCount == 1)
    {
      correctWire = 2;
      Serial.println("Second 5");//Cut the Second Wire
    }

    else if (blueWireCount != 1)
    {
      if (yellowWireCount > 1)
      {
        correctWire = 4;
        Serial.println("Last 6");//Cut the Fourth Wire
      }

      else
      {
        correctWire = 2;
        Serial.println("Second 7");//Cut the Second Wire
      }
    }
  }

  //Game Initialization
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, 320, 240, BLACK);
  tft.setTextColor(WHITE);
  tft.fillRect(0, 0, 320, 240, BLACK); //Resets Screen

  tft.setRotation(0);

  tft.drawFastVLine(50, 50, 230, wireColor1);//First Wire
  tft.drawCircle(50, 50, 5, WHITE);
  tft.drawCircle(50, 50, 3, WHITE);
  tft.drawCircle(50, 275, 5, WHITE);
  tft.drawCircle(50, 275, 3, WHITE);

  tft.drawFastVLine(100, 50, 230, wireColor2);//Second Wire
  tft.drawCircle(100, 50, 5, WHITE);
  tft.drawCircle(100, 50, 3, WHITE);
  tft.drawCircle(100, 275, 5, WHITE);
  tft.drawCircle(100, 275, 3, WHITE);

  tft.drawFastVLine(150, 50, 230, wireColor3);//Third Wire
  tft.drawCircle(150, 50, 5, WHITE);
  tft.drawCircle(150, 50, 3, WHITE);
  tft.drawCircle(150, 275, 5, WHITE);
  tft.drawCircle(150, 275, 3, WHITE);

  tft.drawFastVLine(200, 50, 230, wireColor4);//Fourth Wire
  tft.drawCircle(200, 50, 5, WHITE);
  tft.drawCircle(200, 50, 3, WHITE);
  tft.drawCircle(200, 275, 5, WHITE);
  tft.drawCircle(200, 275, 3, WHITE);
  wire_cut = false;
  wireCut = 0;
}

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  //if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
#ifdef DEBUG
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
#endif  DEBUG
    if (p.y < (TS_MINY - 5))
      // scale from 0->1023 to tft.width
      p.x = tft.width() - (map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
    p.y = tft.height() - (map(p.y, TS_MAXY, TS_MINY, tft.height(), 0));
#ifdef DEBUG
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
#endif DEBUG

    if (p.x > 290 && p.x < 310 && !(wireCut == 1))//First Wire cut
    {
      wire_cut == true;
      wireCut = 1;
      tft.setRotation(0);
      tft.drawFastVLine(50, 50, 230, BLACK);
      tft.drawFastVLine(100, 50, 230, wireColor2);
      tft.drawFastVLine(150, 50, 230, wireColor3);
      tft.drawFastVLine(200, 50, 230, wireColor4);
     
      if (wireCut == correctWire)
      {
        digitalWrite(12, LOW);
        success();
      }
      else if (wireCut != correctWire)
      {
        errors++;
        reset();
      }
    }//end First Wire

    if (p.x > 440 && p.x < 460 && !(wireCut == 2))//Second Wire cut
    {
      wire_cut == true;
      wireCut = 2;
      tft.setRotation(0);
      tft.drawFastVLine(50, 50, 230, wireColor1);//Red Line
      tft.drawFastVLine(100, 50, 230, BLACK); //Cuts Blue Line
      tft.drawFastVLine(150, 50, 230, wireColor3);//Green Line
      tft.drawFastVLine(200, 50, 230, wireColor4);//Yellow Line
 
      if (wireCut == correctWire)
      {
        digitalWrite(12, LOW);
        success();
      }
      else if (wireCut != correctWire)
      {
        errors++;
        reset();
      }
    }//end second Wire

    if (p.x > 615 && p.x < 635 && !(wireCut == 3))//Third Wire cut
    {
      wire_cut == true;
      wireCut = 3;
      tft.setRotation(0);
      tft.drawFastVLine(50, 50, 230, wireColor1);//Red Line
      tft.drawFastVLine(100, 50, 230, wireColor2);//Blue line
      tft.drawFastVLine(150, 50, 230, BLACK);//Cuts Green Line
      tft.drawFastVLine(200, 50, 230, wireColor4);//Yellow Line
      
      if (wireCut == correctWire)
      {
        digitalWrite(12, LOW);
        success();
      }
      else if (wireCut != correctWire)
      {
        errors++;
        reset();
      }
    }//end Third Wire

    if (p.x > 780 && p.x < 800 && !(wireCut == 4))//Fourth Wire cut
    {
      wireCut = 4;
      wire_cut == true;
      tft.setRotation(0);
      tft.drawFastVLine(50, 50, 230, wireColor1);//Red Line
      tft.drawFastVLine(100, 50, 230, wireColor2);//Blue line
      tft.drawFastVLine(150, 50, 230, wireColor3);//Green Line
      tft.drawFastVLine(200, 50, 230, BLACK); //Cuts Yellow Line
    if (wireCut == correctWire)
      {
        digitalWrite(12, LOW);
        success();
      }
      else if (wireCut != correctWire)
      {
        errors++;
        reset();
      }

    }//end Fourth Wire
  }//end touch Register
}//end loop

void success()
{
  tft.fillRect(0, 0, 320, 320, BLACK);
  tft.fillRect(0, 0, 320, 320, BLACK); //Resets Screen
  tft.setRotation(3);
  tft.setTextColor(BLUE);
  tft.setTextSize(5);
  tft.setCursor(60, 100);
  tft.println("Success");
  while (true)
  {
    //Game Over
  }//end while

}

void reset()
{
  // assign a random number from 0 to 5
  int randArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
  const size_t n = sizeof(randArray) / sizeof(randArray[0]);
  for (size_t i = 0; i < n - 1; i++) {
    size_t j = random(1, n - i);
    int t = randArray[i];
    randArray[i] = randArray[j];
    randArray[j] = t;
  }

  randomSeed(analogRead(A5));
  int randomPlacement = random(0, 23);
  const int color1 = randArray[randomPlacement];
  wireColorGeneration(color1);
  determineWireColor();
  Serial.println(wire1); Serial.println(wire2);
  Serial.println(wire3); Serial.println(wire4);
  Serial.println(color1);

  //Situation with at least one wire that is Red and the last wire being Green
  if ((wire1 == 'R' | wire2 == 'R' | wire3 == 'R' | wire4 == 'R') & wire4 == 'G')
  {
    correctWire = 4;
    Serial.println("Last 1");//Cut Last Wire
  }
  //Situation with at least one wire that is Red and the last wire isn't Green and a blue wire
  else if ((wire1 == 'R' | wire2 == 'R' | wire3 == 'R' | wire4 == 'R') & wire4 != 'G')
  {
    int blueWireCount = 0;
    if (wire1 == 'B')
    {
      blueWireCount++;
    }

    if (wire2 == 'B')
    {
      blueWireCount++;
    }

    if (wire3 == 'B')
    {
      blueWireCount++;
    }

    if (wire4 == 'B')
    {
      blueWireCount++;
    }

    if (blueWireCount <= 1)
    {
      correctWire = 3;
      Serial.println("Third 2");//Cut Third Wire
    }

    else if (blueWireCount > 1)
    {
      if (wire4 == 'B')
      {
        correctWire = 4;
      }

      else if (wire3 == 'B')
      {
        correctWire = 3;
      }

      else if (wire2 == 'B')
      {
        correctWire = 2;
      }

      else
      {
        correctWire = 1;
      }
      Serial.println("Last Blue 3");//Cut Last Blue wire
    }
  }
  //Situation for no Red wires and the last wire is yellow
  else if ((wire1 != 'R' & wire2 != 'R' & wire3 != 'R' & wire4 != 'R') & wire4 == 'Y')
  {
    correctWire = 1;
    Serial.println("First 4");//Cut First Wire
  }
  //Covers all other situations
  else
  {
    int blueWireCount = 0;
    if (wire1 == 'B')
    {
      blueWireCount++;
    }

    if (wire2 == 'B')
    {
      blueWireCount++;
    }

    if (wire3 == 'B')
    {
      blueWireCount++;
    }

    if (wire4 == 'B')
    {
      blueWireCount++;
    }

    int yellowWireCount = 0;
    if (wire1 == 'Y')
    {
      yellowWireCount++;
    }

    if (wire2 == 'Y')
    {
      yellowWireCount++;
    }

    if (wire3 == 'Y')
    {
      yellowWireCount++;
    }
    if (wire4 == 'Y')
    {
      yellowWireCount++;
    }

    if (blueWireCount == 1)
    {
      correctWire = 2;
      Serial.println("Second 5");//Cut the Second Wire
    }

    else if (blueWireCount != 1)
    {
      if (yellowWireCount > 1)
      {
        correctWire = 4;
        Serial.println("Last 6");//Cut the Fourth Wire
      }

      else
      {
        correctWire = 2;
        Serial.println("Second 7");//Cut the Second Wire
      }
    }
  }
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, 320, 240, BLACK);
  tft.setTextColor(RED);
  tft.fillRect(0, 0, 320, 240, BLACK); //Resets Screen
  tft.setRotation(0);
  tft.drawFastVLine(50, 50, 230, wireColor1);//First Wire
  tft.drawCircle(50, 50, 5, WHITE);
  tft.drawCircle(50, 50, 3, WHITE);
  tft.drawCircle(50, 275, 5, WHITE);
  tft.drawCircle(50, 275, 3, WHITE);

  tft.drawFastVLine(100, 50, 230, wireColor2);//Second Wire
  tft.drawCircle(100, 50, 5, WHITE);
  tft.drawCircle(100, 50, 3, WHITE);
  tft.drawCircle(100, 275, 5, WHITE);
  tft.drawCircle(100, 275, 3, WHITE);

  tft.drawFastVLine(150, 50, 230, wireColor3);//Third Wire
  tft.drawCircle(150, 50, 5, WHITE);
  tft.drawCircle(150, 50, 3, WHITE);
  tft.drawCircle(150, 275, 5, WHITE);
  tft.drawCircle(150, 275, 3, WHITE);

  tft.drawFastVLine(200, 50, 230, wireColor4);//Fourth Wire
  tft.drawCircle(200, 50, 5, WHITE);
  tft.drawCircle(200, 50, 3, WHITE);
  tft.drawCircle(200, 275, 5, WHITE);
  tft.drawCircle(200, 275, 3, WHITE);
  tft.setRotation(3);
  tft.setCursor(90, 40);
  tft.setTextSize(2);
  pinMode(13, OUTPUT);
  wire_cut = false;
  wireCut = 0;
  return;
}

void wireColorGeneration(int number) {
  switch (number) {
    case 1: // First combo
      wire1 = 'R';
      wire2 = 'B';
      wire3 = 'Y';
      wire4 = 'G';
      break;
    case 2: //Second Combo
      wire1 = 'B';
      wire2 = 'R';
      wire3 = 'G';
      wire4 = 'G';
      break;
    case 3: //Third Combo
      wire1 = 'B';
      wire2 = 'R';
      wire3 = 'B';
      wire4 = 'Y';
      break;
    case 4: //Fourth Combo
      wire1 = 'B';
      wire2 = 'B';
      wire3 = 'R';
      wire4 = 'G';
      break;
    case 5: // Fifth Combo
      wire1 = 'R';
      wire2 = 'B';
      wire3 = 'B';
      wire4 = 'G';
      break;
    case 6: // Sixth Combo
      wire1 = 'R';
      wire2 = 'B';
      wire3 = 'G';
      wire4 = 'B';
      break;
    case 7: // Seventh combo
      wire1 = 'B';
      wire2 = 'B';
      wire3 = 'R';
      wire4 = 'B';
      break;
    case 8: //Eigth Combo
      wire1 = 'G';
      wire2 = 'B';
      wire3 = 'B';
      wire4 = 'R';
      break;
    case 9: //Ninth Combo
      wire1 = 'G';
      wire2 = 'B';
      wire3 = 'B';
      wire4 = 'Y';
      break;
    case 10: //Tenth Combo
      wire1 = 'Y';
      wire2 = 'Y';
      wire3 = 'G';
      wire4 = 'Y';
      break;
    case 11: // 11th Combo
      wire1 = 'B';
      wire2 = 'G';
      wire3 = 'Y';
      wire4 = 'Y';
      break;
    case 12: // 12th Combo
      wire1 = 'G';
      wire2 = 'G';
      wire3 = 'B';
      wire4 = 'Y';
      break;
    case 13: // 13th combo
      wire1 = 'B';
      wire2 = 'G';
      wire3 = 'G';
      wire4 = 'G';
      break;
    case 14: //14th Combo
      wire1 = 'Y';
      wire2 = 'B';
      wire3 = 'G';
      wire4 = 'G';
      break;
    case 15: //15 Combo
      wire1 = 'Y';
      wire2 = 'Y';
      wire3 = 'Y';
      wire4 = 'B';
      break;
    case 16: //16th Combo
      wire1 = 'G';
      wire2 = 'B';
      wire3 = 'G';
      wire4 = 'B';
      break;
    case 17: // 17th Combo
      wire1 = 'Y';
      wire2 = 'Y';
      wire3 = 'G';
      wire4 = 'G';
      break;
    case 18: // 18th Combo
      wire1 = 'Y';
      wire2 = 'B';
      wire3 = 'Y';
      wire4 = 'B';
      break;
    case 19: // 19th combo
      wire1 = 'Y';
      wire2 = 'G';
      wire3 = 'Y';
      wire4 = 'Y';
      break;
    case 20: //20th Combo
      wire1 = 'Y';
      wire2 = 'Y';
      wire3 = 'Y';
      wire4 = 'Y';
      break;
    case 21: //21st Combo
      wire1 = 'B';
      wire2 = 'B';
      wire3 = 'G';
      wire4 = 'B';
      break;
    case 22: //22nd Combo
      wire1 = 'Y';
      wire2 = 'G';
      wire3 = 'G';
      wire4 = 'G';
      break;
    case 23: //23rd Combo
      wire1 = 'G';
      wire2 = 'B';
      wire3 = 'B';
      wire4 = 'G';
      break;
    case 24: //24th Combo
      wire1 = 'G';
      wire2 = 'Y';
      wire3 = 'B';
      wire4 = 'B';
      break;

  }
}

void determineWireColor()
{
  if (wire1 == 'R')
  {
    wireColor1 = RED;
  }

  else if (wire1 == 'B')
  {
    wireColor1 = BLUE;
  }

  else if (wire1 == 'G')
  {
    wireColor1 = GREEN;
  }

  else if (wire1 == 'Y')
  {
    wireColor1 = YELLOW;
  }

  if (wire2 == 'R')
  {
    wireColor2 = RED;
  }

  else if (wire2 == 'B')
  {
    wireColor2 = BLUE;
  }

  else if (wire2 == 'G')
  {
    wireColor2 = GREEN;
  }

  else if (wire2 == 'Y')
  {
    wireColor2 = YELLOW;
  }

  if (wire3 == 'R')
  {
    wireColor3 = RED;
  }

  else if (wire3 == 'B')
  {
    wireColor3 = BLUE;
  }

  else if (wire3 == 'G')
  {
    wireColor3 = GREEN;
  }

  else if (wire3 == 'Y')
  {
    wireColor3 = YELLOW;
  }

  if (wire4 == 'R')
  {
    wireColor4 = RED;
  }

  else if (wire4 == 'B')
  {
    wireColor4 = BLUE;
  }

  else if (wire4 == 'G')
  {
    wireColor4 = GREEN;
  }

  else if (wire4 == 'Y')
  {
    wireColor4 = YELLOW;
  }
}

