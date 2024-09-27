//Inputs
#define CLK 4         //External Clock Pin
#define CLKI_RATE A0  //internalCLK Clock Rate voltage
#define SHIFT A5
#define SHIFT_BUTTON 9
#define RST 10    //Reset Button Button
#define L1_IN A7  //Voltage for Length of first Euclid Rhythm
#define L2_IN A2  //Voltage for Length of second Euclid Rhythm
#define S1_IN A3  //Voltage for Amount of steps of the first Euclid Rhythm
#define S2_IN A1  //Voltage for Amount of steps of the second Euclid Rhythm

//Outputs
#define CLKO 2  //internalCLK Clock Output
#define OUT1 8  //Euclid. Ryhthm 1 Output
#define OUT2 7  //Euclid. Ryhthm 2 Output

int timeout = 2000;  //Timeout in milliseconds it takes for the internalCLK clock to take over. After X without an external Clock, the internalCLK clock takes over.
bool internal = true;
int resetTime = 0;
int external = 0;

int counter = 0;
int shift = 0;

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

  //Switch back to the internalCLK Clock.
  if (millis() > resetTime) {
    internal = true;
  }

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
    //Resetted, reset the channel assignment => Reset the shifts to the default
    shift = 0;
  }

  if ((digitalRead(RST) == HIGH) && (RSTtriggerInterrupted == true)) {
    RSTtriggerInterrupted = false;  //Reset  flag
  }
}

void checkClock() {

  CLKtriggered = (digitalRead(CLK) == LOW) && (CLKtriggerInterrupted == false);

  if (CLKtriggered) {
    //External Clock Rising Edge
    CLKtriggerInterrupted = true;
    internal = false;  //internalCLK clock is disabled.
    resetTime = millis() + 2000;
    //Set the new resetTime after which the internalCLK Clock takes over.
    counter++;
  }
  if (!internal && (digitalRead(CLK) == LOW)) {
    //External Clock is held high
    resetTime = millis() + 2000;
  }
  if ((digitalRead(CLK) == HIGH) && (CLKtriggerInterrupted == true)) {
    //External Clock falling edge
    resetTime = millis() + 2000;
    CLKtriggerInterrupted = false;  //Reset Clock flag
  }
}

void checkShift() {

  bool SHIFTStatus = digitalRead(SHIFT) == LOW || digitalRead(SHIFT_BUTTON) == LOW;

  SHIFTtriggered = SHIFTStatus && (SHIFTtriggerInterrupted == false);

  if (SHIFTtriggered) {
    SHIFTtriggerInterrupted = true;

    shift = (shift + 1) % 4;
  }

  if ((digitalRead(CLK) == HIGH) && (SHIFTtriggerInterrupted == true)) {
    SHIFTtriggerInterrupted = false;  //Reset Shift flag
  }
}

void writeOutputs() {

  int outputPin1;
  int outputPin2;
  int outputPin3;

  if (shift == 0) {
    outputPin1 = OUT1;
    outputPin2 = OUT2;
    outputPin3 = CLKO;
  } else if (shift == 1) {
    outputPin1 = OUT2;
    outputPin2 = CLKO;
    outputPin3 = OUT1;
  } else if (shift == 2) {
    outputPin1 = CLKO;
    outputPin2 = OUT1;
    outputPin3 = OUT2;
  } else if (shift == 3) {
    outputPin1 = CLKO;
    outputPin2 = OUT2;
    outputPin3 = OUT1;
  }

  if (internal) {
    digitalWrite(CLKO, internalCLK.state);
  } else {
    digitalWrite(CLKO, digitalRead(CLK));
  }

  if ((internalCLK.state == HIGH) || (digitalRead(CLK) == HIGH && !internal)) {
    //OUT1
    if (rhythm1[counter % length1]) {
      digitalWrite(outputPin1, HIGH);
    } else {
      digitalWrite(outputPin1, LOW);
    }
    //OUT2
    if (rhythm2[counter % length2]) {
      digitalWrite(outputPin2, HIGH);
    } else {
      digitalWrite(outputPin2, LOW);
    }

  } else {

    digitalWrite(OUT1, LOW);
    digitalWrite(OUT2, LOW);
  }
}
