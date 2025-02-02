//3 Channel Euclid generator. Needs an external clock, the RATE knob controls the amount of steps of the 3rd channel.

//Inputs
#define CLK 19         //External Clock Pin
#define SHIFT 4
#define SHIFT_BUTTON 10
#define RST 9    //Reset Button Button
#define L1_IN A7  //Voltage for Length of first Euclid Rhythm
#define L2_IN A2  //Voltage for Length of second Euclid Rhythm
#define S1_IN A3  //Voltage for Amount of steps of the first Euclid Rhythm
#define S2_IN A1  //Voltage for Amount of steps of the second Euclid Rhythm
#define S3_IN A0  //Voltage for Amount of steps of the third Euclid Rhythm

//Outputs
#define OUT3 2  //Euclid. Ryhthm 3 Output
#define OUT1 8  //Euclid. Ryhthm 1 Output
#define OUT2 7  //Euclid. Ryhthm 2 Output

int counter = 0;
int shift = 0;

int outputPin1 = OUT1;
int outputPin2 = OUT2;
int outputPin3 = OUT3;

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

volatile int length1 = 32;
volatile int length2 = 32;
int length3 = 16;
volatile int steps1 = 8;
volatile int steps2 = 8;
volatile int steps3 = 8;

bool rhythm1[32];  // Maximum length of 32
bool rhythm2[32];  // Maximum length of 32
bool rhythm3[16]; // fixed length

void setup() {

  //No INPUT_PULLUP needed because of the external 10k resistors.

  pinMode(CLK, INPUT_PULLUP);
  pinMode(SHIFT, INPUT_PULLUP);
  pinMode(SHIFT_BUTTON, INPUT);
  pinMode(RST, INPUT);
  pinMode(L1_IN, INPUT);
  pinMode(S1_IN, INPUT);
  pinMode(L2_IN, INPUT);
  pinMode(S2_IN, INPUT);
  pinMode(S3_IN, INPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);

}

void loop() {
  updateEuclid();
  checkClock();
  checkReset();
  checkShift();
  //Inputs have pullup resistors instead of pulldown, thus the trigger conditioning is inverted from e.g.: my Sequencer.
}

void updateEuclid() {
  //Update parameters
  length1 = map(analogRead(L1_IN), 0, 1023, 2, 32);
  length2 = map(analogRead(L2_IN), 0, 1023, 2, 32);
  //length3 = 16;
  steps1 = map(analogRead(S1_IN), 0, 1023, 1, length1);
  steps2 = map(analogRead(S2_IN), 0, 1023, 1, length2);
  steps3 = map(analogRead(S3_IN), 0, 1023, 1, length3);
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

  //Generate the third Rhythm
int bucket3 = 0;

  for (int i = 0; i < length3; i++) {
    bucket3 += steps3;
    if (bucket3 >= length3) {
      bucket3 -= length3;
      rhythm3[i] = 1;
    } else {
      rhythm3[i] = 0;
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

    counter++;
      //Write the outputs directly after the counter increased, this prevents pulses from firing after the transition from LOW to HIGH on the output
      // OUT1
      digitalWrite(outputPin1, rhythm1[counter % length1] ? HIGH : LOW);
      // OUT2
      digitalWrite(outputPin2, rhythm2[counter % length2] ? HIGH : LOW);
      // OUT3
      digitalWrite(outputPin3, rhythm3[counter % length3] ? HIGH : LOW);
  }
  if ((digitalRead(CLK) == HIGH) && (CLKtriggerInterrupted == true)) {
    // External Clock Falling Edge
    CLKtriggerInterrupted = false; // Reset Clock flag

      digitalWrite(outputPin3, LOW);
      digitalWrite(outputPin1, LOW);
      digitalWrite(outputPin2, LOW);
  }
}

void checkShift() {

  bool SHIFTStatus = digitalRead(SHIFT) == LOW || digitalRead(SHIFT_BUTTON) == LOW;

  SHIFTtriggered = SHIFTStatus && (SHIFTtriggerInterrupted == false);

  if (SHIFTtriggered) {
    SHIFTtriggerInterrupted = true;

    shift = (shift + 1) % 6;
  }

  //Swap the channels 
   if (shift == 0) {
    outputPin1 = OUT1;
    outputPin2 = OUT2;
    outputPin3 = OUT3;
  } else if (shift == 1) {
    outputPin1 = OUT2;
    outputPin2 = OUT1;
    outputPin3 = OUT3;
  } else if (shift == 2) {
    outputPin1 = OUT2;
    outputPin2 = OUT3;
    outputPin3 = OUT1;
  } else if (shift == 3) {
    outputPin1 = OUT1;
    outputPin2 = OUT3;
    outputPin3 = OUT2;
  } else if (shift == 4) {
    outputPin1 = OUT3;
    outputPin2 = OUT2;
    outputPin3 = OUT1;
  } else if (shift == 5) {
    outputPin1 = OUT3;
    outputPin2 = OUT1;
    outputPin3 = OUT2;
  } 

  if ((digitalRead(CLK) == HIGH) && (SHIFTtriggerInterrupted == true)) {
    SHIFTtriggerInterrupted = false;  //Reset Shift flag
  }
}
