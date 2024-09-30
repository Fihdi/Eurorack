#define CLK 4

int counter = 0;
int freq = 2;
int prevfreq = 0;

int lapse = 1000;

//Interrupt flags
volatile bool CLKtriggerInterrupted = false;
volatile bool CLKtriggered = false;

struct Clock {
  unsigned long previousMillis;  // Variable to store the current state and the last toggle time for each clock
  int delayTime;
  int state;
};

Clock internalCLK;


void setup() {
  Serial.begin(9600);
   pinMode(CLK, INPUT_PULLUP);
   pinMode(8, OUTPUT);
}

void loop() {
  checkClock();
  if(millis()%lapse==0){
  //New Block
  if(counter!=0 && counter!=1){
  freq = counter;
  Serial.println(counter);
  }
  counter=0;
  }
//Generate internal Clock with twice the frequency 
  internalCLK.delayTime = 1000/(2*freq);
    //Write internalCLK Clock state, the output is written in writeOutputs();
    if (millis() - internalCLK.previousMillis >= internalCLK.delayTime) {
      internalCLK.previousMillis = millis();
      internalCLK.state = !internalCLK.state;

      if(internalCLK.state == HIGH){
        Use counter to 
        //counter++;
      }
    }

digitalWrite(8, internalCLK.state);


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
