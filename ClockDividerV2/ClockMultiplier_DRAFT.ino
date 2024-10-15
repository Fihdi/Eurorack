//Output 1 is 3 times the original frequency
//Output 2 is 2 times the original frequency
//Output 3 is the same as the original frequency
//Output 4 is half the original frequency
//Output 5 is a third of the original frequency

#define CLK 4

int counter = 0;
int freq = 0;
int prevfreq = 0;

int lapse = 1000;

//Interrupt flags
volatile bool CLKtriggerInterrupted = false;
volatile bool CLKtriggered = false;

struct Clock {
  unsigned long previousMillis;  // Variable to store the current state and the last toggle time for each clock
  int delayTime;
  int state;
  int outputPin;
};

Clock internalCLK[5];

void setup() {

   pinMode(CLK, INPUT_PULLUP);
  
   pinMode(A2, OUTPUT);
   pinMode(12, OUTPUT);
   pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);
   pinMode(5, OUTPUT);

  internalCLK[0].outputPin = A2;
  internalCLK[1].outputPin = 12;
  internalCLK[2].outputPin = 9;
  internalCLK[3].outputPin = 10;
  internalCLK[4].outputPin = 5;
}

void loop() {
  checkClock();
  
  if(millis()%lapse==0){
  //New Block
  if(counter!=0){
    updateInternalClocks();
  freq = counter;
    counter=0;
  }else{
  //No Incoming signal detected -> frequency is 0Hz -> disable all clocks.
    
  digitalWrite(internalCLK[0].outputPin, LOW);
  digitalWrite(internalCLK[1].outputPin, LOW);
  digitalWrite(internalCLK[2].outputPin, LOW);
  digitalWrite(internalCLK[3].outputPin, LOW);
  digitalWrite(internalCLK[4].outputPin, LOW);
    
  }
  }
}

void updateInternalClocks(){

for(int i = 0; i< 5; i++){

 internalCLK[0].delayTime = 1000/(3*freq);  //3x frequency
  internalCLK[1].delayTime = 1000/(2*freq); //2x frequency
  internalCLK[2].delayTime = 1000/(1*freq); //1x frequency
  internalCLK[3].delayTime = 2000/(freq);   //0.5x frequency
  internalCLK[4].delayTime = 3000/(freq);   //0.33x frequency

if (millis() - internalCLK[i].previousMillis >= internalCLK[i].delayTime) {
      internalCLK[i].previousMillis = millis();
      internalCLK[i].state = !internalCLK[i].state;
    }

digitalWrite(internalCLK[i].outputPin, internalCLK[i].state);
  
}
}

void checkClock() {

  CLKtriggered = (digitalRead(CLK) == LOW) && (CLKtriggerInterrupted == false);

  if (CLKtriggered) {
    CLKtriggerInterrupted = true;
    //Increase the counter by 1
    counter++;
  }

  if ((digitalRead(CLK) == HIGH) && (CLKtriggerInterrupted == true)) {
    CLKtriggerInterrupted = false;  //Reset trigger flag
  }
}
