//Mark Sullivan
int buttonIn;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(12,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonIn = digitalRead(12);
  Serial.println(buttonIn);
  if(buttonIn){//add check for sides completed
    Serial.println("Button pressed");
    //win occured
  }
}
