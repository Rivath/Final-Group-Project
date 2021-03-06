//Mark Sullivan

int initialize = 0;
int needPosition;//1 of 5 direction randomly calculated originally
int knobPosition;//1 of 5 directions calculated each loop
int knobPositionRead;//value read
int switchPosition;//current position of switch
int startingSwitchPosition;//initial position of switch
int positionIndex;
int thresh1 = 131;
int thresh2 = 336;
int thresh3 = 495;
int thresh4 = 721;
boolean KnobsSidePassed = false;
String lightsArray[20] = {"10001100", "01010010", "01001010", "01100001",
                          "00010101", "00001011", "11000100", "10000101",
                          "00000111", "01000110", "11010000", "01100010",
                          "00101100", "10100100", "00100110", "01100100",
                          "00010110", "01000101", "10100001", "10101000"
                         };
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  randomSeed(analogRead(0));
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

}

void loop() {
  knobPositionRead = analogRead(0);
  switchPosition = digitalRead(13);
  
  if (switchPosition != startingSwitchPosition) {
    startingSwitchPosition = !startingSwitchPosition;
    winConKnob(knobPositionRead);
    if (knobPosition == needPosition) {
      Serial.println("PASSSSSSSSSS");
      for (int i = 0; i < 8; i++){ //turn on all LEDS
      KnobsSidePassed = true;
        digitalWrite(i + 2, HIGH);
      //player passed
      }
    }
    else {
      Serial.println("FAILLLLLLLLL");
      errorCount++;
      //player failed
    }
  }
}

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


