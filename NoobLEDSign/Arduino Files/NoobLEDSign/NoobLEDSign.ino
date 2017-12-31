int pinN = 5;
int pinO1 = 6;
int pinO2 = 7;
int pinB = 8;
int CommonledState = LOW;  
int NledState = LOW;  
int O1ledState = LOW;  
int O2ledState = LOW;  
int BledState = LOW;  


int encoderPin1 = 2;
int encoderPin2 = 3;
int encoderSwitchPin = 4; //push button switch
int prevState = 0;
int currentState = 0;
int estados=0;
int i=0;


int delayTime = 1000;
int delayTimeActive = 0;


volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;


unsigned long previousMillis = 0; 

byte sequence[19][4]{{1,0,0,0},
                  {0,1,0,0},
                  {0,0,1,0},
                  {0,0,0,1},
                  {0,0,1,0},
                  {0,1,0,0},
                  {1,0,0,0},
                  {1,1,0,0},                 
                  {1,1,1,0},
                  {1,1,1,1},
                  {0,1,1,1},
                  {0,0,1,1},
                  {0,0,0,1},
                  {0,0,1,1},
                  {0,1,1,1},
                  {1,1,1,1},
                  {0,0,0,0},
                  {1,1,1,1},
                  {0,0,0,0},
};


void setup() {
  Serial.begin (9600);
  pinMode(pinN, OUTPUT);
  pinMode(pinO1, OUTPUT);
  pinMode(pinO2, OUTPUT);
  pinMode(pinB, OUTPUT);

  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(encoderSwitchPin, INPUT);
  digitalWrite(encoderSwitchPin, HIGH); //turn pullup resistor on
   
  
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
}

// the loop function runs over and over again forever
void loop() {
  prevState = currentState;
  currentState = digitalRead(encoderSwitchPin);
  if (prevState==0 && currentState==1){
    estados++;
    encoderValue = 0;
    if (estados>4)
      estados=0;
  }
  Serial.println(estados);
  delayTimeActive = 1000+encoderValue*10;
  if (delayTimeActive > 5000)
    delayTimeActive = 5000;
  if (delayTimeActive < 100)
    delayTimeActive=100;
  switch (estados) {
    case 0:
      blinkLeds();
      break;
    case 1:
      allON();
      break;
    case 2:
      randomLeds();
      break;
    case 3:
      sequenceLeds();
      break;
  }
}

void updateEncoder(){
int MSB = digitalRead(encoderPin1); //MSB = most significant bit
int LSB = digitalRead(encoderPin2); //LSB = least significant bit

int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number 
int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value 
if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) 
  encoderValue ++; 
if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) 
  encoderValue --; 
lastEncoded = encoded; //store this value for next time 
}

 
void blinkLeds(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= delayTimeActive) {
    previousMillis = currentMillis;
    if (CommonledState == LOW) {
      CommonledState = HIGH;
      } 
    else {
      CommonledState = LOW;
      }
    digitalWrite(pinN, CommonledState);
    digitalWrite(pinO1, CommonledState);
    digitalWrite(pinO2, CommonledState);
    digitalWrite(pinB, CommonledState);
  }
  
}

void randomLeds(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= delayTimeActive) {
    previousMillis = currentMillis;

    digitalWrite(pinN, random(2));
    digitalWrite(pinO1, random(2));
    digitalWrite(pinO2, random(2));
    digitalWrite(pinB, random(2));
  }
  
}

void allON(){
  digitalWrite(pinN, HIGH);
  digitalWrite(pinO1, HIGH); 
  digitalWrite(pinO2, HIGH); 
  digitalWrite(pinB, HIGH); 
}


void sequenceLeds(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= delayTimeActive) {
    previousMillis = currentMillis;
    
    digitalWrite(pinN, sequence[i][0]);
    digitalWrite(pinO1, sequence[i][1]);
    digitalWrite(pinO2, sequence[i][2]);
    digitalWrite(pinB, sequence[i][3]);
    i++;
    if (i>18)
      i=0;
  }
  
}
