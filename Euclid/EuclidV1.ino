//Inputs
#define CLK 19         //External Clock Pin
#define CLKI_RATE A0  //internalCLK Clock Rate voltage
#define SHIFT 4
#define SHIFT_BUTTON 10
#define RST 9    //Reset Button Button
#define L1_IN A7  //Voltage for Length of first Euclid Rhythm
#define L2_IN A2  //Voltage for Length of second Euclid Rhythm
#define S1_IN A3  //Voltage for Amount of steps of the first Euclid Rhythm
#define S2_IN A1  //Voltage for Amount of steps of the second Euclid Rhythm

//Outputs
#define CLKO 2  //internalCLK Clock Output
#define OUT1 8  //Euclid. Ryhthm 1 Output
#define OUT2 7  //Euclid. Ryhthm 2 Output

bool internal = true;

int counter = 0;
int shift = 0;

int outputPin1 = OUT1;
int outputPin2 = OUT2;

//Interrupt flags
bool shiftStatus = false;  //Combines the status of the Shift button and the Shift Input, honestly I should have just combined these signals on the PCB itself and not waste a Pin. meh.
volatile bool CLKtriggerInterrupted = false;
volatile bool CLKFallingEdge = false;
volatile bool RSTtriggerInterrupted = false;
volatile bool SHIFTtriggerInterrupted = false;

//Interrupt conditions
volatile bool RSTtriggered = false;
volatile bool CLKtriggered = false;
volatile bool SHIFTtriggered = false;

volatile int length1 = 16;
volatile int length2 = 16;
volatile int steps1 = 8;
volatile int steps2 = 8;

const int minDelay = 20;    // Minimum delay in milliseconds (sets the maximum frequency)
const int maxDelay = 2000;  // Maximum delay in milliseconds (sets the minimum frequency)

bool rhythm1[16];  // Maximum length of 16
bool rhythm2[16];  // Maximum length of 16

struct Clock {
  unsigned long previousMillis;  // Variable to store the current state and the last toggle time for each clock
  int delayTime;
  int state;
};

Clock internalCLK;

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

  internalCLK.delayTime = 100;
}

void loop() {

  updateinternalCLKClock();
  checkClock();
  checkReset();
  checkShift();
  updateEuclid();
  writeOutputs();
  //Inputs have pullup resistors instead of pulldown, thus the trigger conditioning is inverted from e.g.: my Sequencer.
}

void updateinternalCLKClock() {

  if (internal) {
    unsigned long currentMillis = millis();

    internalCLK.delayTime = map(analogRead(CLKI_RATE), 0, 1023, minDelay, maxDelay);  // Set the frequency of the internalCLK

    //Write internalCLK Clock state, the output is written in writeOutputs();
    if (currentMillis - internalCLK.previousMillis >= internalCLK.delayTime) {
      internalCLK.previousMillis = currentMillis;
      internalCLK.state = !internalCLK.state;
      //The state changed, if state is HIGH now that means the clock just had a rising Edge, update the counter.
      if (internalCLK.state == HIGH) {
        counter++;
      }
    }
  }
}

void updateEuclid() {
  //Update parameters
  length1 = map(analogRead(L1_IN), 0, 1023, 2, 16);
  length2 = map(analogRead(L2_IN), 0, 1023, 2, 16);
  steps1 = map(analogRead(S1_IN), 0, 1023, 1, length1);
  steps2 = map(analogRead(S2_IN), 0, 1023, 1, length2);
  //Generate first Rhythm
  int bucket1 = 0;

  for (int i = 0; i < length1; i++) {
    bucket1 += steps1;
    if (bucket1 >= length1) {
      bucket1 -= length1;
      rhythm1[i] = 1;
    } else {
      rhythm1[i] = 0;
    }
  }
  //Generate second Rhythm
  int bucket2 = 0;

  for (int i = 0; i < length2; i++) {
    bucket2 += steps2;
    if (bucket2 >= length2) {
      bucket2 -= length2;
      rhythm2[i] = 1;
    } else {
      rhythm2[i] = 0;
    }
  }
}

void checkReset() {
  RSTtriggered = (digitalRead(RST) == LOW) && (RSTtriggerInterrupted == false);

  if (RSTtriggered) {
    RSTtriggerInterrupted = true;
    //Reset the channel assignment and reset the counter
    shift = 0;
    counter = 0;
    //Activate the internal clock
    internal = true;
  }

  if ((digitalRead(RST) == HIGH) && (RSTtriggerInterrupted == true)) {
    RSTtriggerInterrupted = false;  //Reset  flag
  }
}

void checkClock() {
  CLKtriggered = (digitalRead(CLK) == LOW) && (CLKtriggerInterrupted == false);

  if (CLKtriggered) {
    // External Clock Rising Edge
    CLKtriggerInterrupted = true;
      // Switching from internal to external clock
      internal = false;       // Disable the internal clock and the WriteOutputs function.
    counter++;
      //Write the outputs directly after the counter increased, this prevents pulses from firing after the transition from LOW to HIGH on the output.
      //CLK OUT
      digitalWrite(CLKO, HIGH);
      // OUT1
      digitalWrite(outputPin1, rhythm1[counter % length1] ? HIGH : LOW);
      // OUT2
      digitalWrite(outputPin2, rhythm2[counter % length2] ? HIGH : LOW);
    
  }
  if ((digitalRead(CLK) == HIGH) && (CLKtriggerInterrupted == true)) {
    // External Clock Falling Edge
    CLKtriggerInterrupted = false; // Reset Clock flag

      digitalWrite(CLKO, LOW);
      digitalWrite(outputPin1, LOW);
      digitalWrite(outputPin2, LOW);
  }
}

void checkShift() {

  bool SHIFTStatus = digitalRead(SHIFT) == LOW || digitalRead(SHIFT_BUTTON) == LOW;

  SHIFTtriggered = SHIFTStatus && (SHIFTtriggerInterrupted == false);

  if (SHIFTtriggered) {
    SHIFTtriggerInterrupted = true;

    shift = (shift + 1) % 4;
  }

  //Swap the channels 1 and 2, you can add more modes if you want to shift the clock out as well
   if (shift == 0) {
    outputPin1 = OUT1;
    outputPin2 = OUT2;
  } else if (shift == 1) {
    outputPin1 = OUT2;
    outputPin2 = OUT1;
  }

  if ((digitalRead(CLK) == HIGH) && (SHIFTtriggerInterrupted == true)) {
    SHIFTtriggerInterrupted = false;  //Reset Shift flag
  }
}

void writeOutputs() {
//Only writes output when the internal clock is active, otherwise the outputs are written in the "CheckClock" function.
  if (internal) {
    digitalWrite(CLKO, internalCLK.state);
    if (internalCLK.state == HIGH) {
      // OUT1
      digitalWrite(outputPin1, rhythm1[counter % length1] ? HIGH : LOW);
      // OUT2
      digitalWrite(outputPin2, rhythm2[counter % length2] ? HIGH : LOW);
    } else {
      digitalWrite(outputPin1, LOW);
      digitalWrite(outputPin2, LOW);
    }
  } 
}
