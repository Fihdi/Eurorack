//Inputs
#define CLK 4 //External Clock Pin
#define CLKI_RATE A0 //Internal Clock Rate voltage
#define SHIFT A5 
#define SHIFT_BUTTON 9
#define RST 10 //Reset Button Button 
#define L1_IN A7 //Voltage for Length of first Euclid Rhythm
#define L2_IN A2 //Voltage for Length of second Euclid Rhythm
#define S1_IN A3 //Voltage for Amount of steps of the first Euclid Rhythm
#define S2_IN A1 //Voltage for Amount of steps of the second Euclid Rhythm

//Outputs
#define CLKO 2   //Internal Clock Output
#define OUT1 8   //Euclid. Ryhthm 1 Output
#define OUT2 7   //Euclid. Ryhthm 2 Output

int timeout = 2000; //Timeout in milliseconds it takes for the internal clock to take over. After X without an external Clock, the internal clock takes over. 
int counter = 0;
int shift = 0;

//Interrupt flags
bool shiftStatus = false; //Combines the status of the Shift button and the Shift Input, honestly I should have just combined these signals on the PCB itself and not waste a Pin. meh.
volatile bool CLKtriggerInterrupted = false;
volatile bool RSTtriggerInterrupted = false;
volatile bool SHIFTtriggerInterrupted = false;

//Interrupt conditions
volatile bool MODEtriggered = false;
volatile bool CLKtriggered = false;
volatile bool SHIFTtriggered = false;

const int minDelay = 20;     // Minimum delay in milliseconds (sets the maximum frequency) 
const int maxDelay = 2000;   // Maximum delay in milliseconds (sets the minimum frequency)

bool rhythm1[16]; // Maximum length of 16
bool rhythm2[16]; // Maximum length of 16




void setup() {

  //No INPUT_PULLUP needed because of the external 10k resistors.
  
  pinMode(CLK, INPUT_PULLUP);
  pinMode(CLKI_RATE, INPUT);
  pinMode(SHIFT, INPUT_PULLUP);
  pinMode(SHIFT_BUTTON, INPUT);
  pinMode(RST, INPUT);
  pinMode(L1_IN, INPUT);
  pinMode(S1_IN, INPUT);
  pinMode(L2_IN, INPUT);
  pinMode(S2_IN, INPUT);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(CLKO, OUTPUT);

}

void loop() {

  checkClock();
  updateEuclid();
  writeOutputs();
  //Inputs have pullup resistors instead of pulldown, thus the trigger conditioning is inverted from e.g.: my Sequencer. 

}

void updateEuclid(){

}

void checkReset() {
  RSTtriggered = (digitalRead(RST) == LOW) && (RSTtriggerInterrupted == false);

  if (RSTtriggered) {
    RSTtriggerInterrupted = true;
    //Resetted, reset the counter
    counter = 0;
    //Should the program also reset the mode then de-comment the line beneath.
    //mode=1;
  }

  if ((digitalRead(RST) == HIGH) && (RSTtriggerInterrupted == true)) {
    RSTtriggerInterrupted = false;  //Reset  flag
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
    CLKtriggerInterrupted = false;  //Reset Clock flag
  }
}

void checkShift() {
    SHIFTStatus = digitalRead(SHIFT)==LOW || digitalRead(SHIFT_BUTTON)==LOW;

    SHIFTtriggered = SHIFTStatus && (SHIFTtriggerInterrupted == false);

  if (SHIFTtriggered) {
    SHIFTtriggerInterrupted = true;


  }

  if ((digitalRead(CLK) == HIGH) && (SHIFTtriggerInterrupted == true)) {
    SHIFTtriggerInterrupted = false;  //Reset Shift flag
  }
}

void writeOutputs() {

}
