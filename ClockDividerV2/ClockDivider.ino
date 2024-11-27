//Inputs
#define CLK 8
#define MODE 6
#define RST 7

//Outputs
#define OUT1 A2
#define OUT2 12
#define OUT3 9
#define OUT4 10
#define OUT5 5

int counter = 0;
int mode = 0;
int MODE_AMT = 3;

//Interrupt flags
volatile bool CLKtriggerInterrupted = false;
volatile bool RSTtriggerInterrupted = false;
volatile bool MODEtriggerInterrupted = false;

//Interrupt conditions
volatile bool MODEtriggered = false;
volatile bool CLKtriggered = false;
volatile bool RSTtriggered = false;

//Previous Output states
bool OUT1State = true;
bool OUT2State = true;
bool OUT3State = true;
bool OUT4State = true;
bool OUT5State = true;

void setup() {
  //No INPUT_PULLUP needed because of the external 10k resistors.

  pinMode(CLK, INPUT);
  pinMode(MODE, INPUT);
  pinMode(RST, INPUT);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(OUT5, OUTPUT);
}

void loop() {
  //Inputs have pullup resistors instead of pulldown, thus the trigger conditioning is inverted from e.g.: my Sequencer.

  checkClock();
  checkReset();
  checkMode();
}

void checkReset() {
  RSTtriggered = (digitalRead(RST) == LOW) && (RSTtriggerInterrupted == false);

  if (RSTtriggered) {
    RSTtriggerInterrupted = true;
    //Reset the Output states.
    OUT1State = true;
    OUT2State = true;
    OUT3State = true;
    OUT4State = true;
    OUT5State = true;

    //Reset the Outputs themselves
    digitalWrite(OUT1, LOW);
    digitalWrite(OUT2, LOW);
    digitalWrite(OUT3, LOW);
    digitalWrite(OUT4, LOW);
    digitalWrite(OUT5, LOW);

    //Reset the counter
    counter = 0;
    //Should the program also reset the mode then de-comment the line beneath.
    //mode=1;
  }

  if ((digitalRead(RST) == HIGH) && (RSTtriggerInterrupted == true)) {
    RSTtriggerInterrupted = false;  //Reset trigger flag
  }
}

void checkClock() {

  CLKtriggered = (digitalRead(CLK) == LOW) && (CLKtriggerInterrupted == false);

  // CLKtriggered = true;
  if (CLKtriggered) {
    CLKtriggerInterrupted = true;
    //Increase the counter by 1
    counter++;
    writeOutputs();
  }

  if ((digitalRead(CLK) == HIGH) && (CLKtriggerInterrupted == true)) {
    CLKtriggerInterrupted = false;  //Reset trigger flag
  }
}

void checkMode() {

  MODEtriggered = (digitalRead(MODE) == LOW) && (MODEtriggerInterrupted == false);

  if (MODEtriggered) {
    MODEtriggerInterrupted = true;
    //Change the mode, modulo MODE_AMT to prevent the mode counter to get invalid values.
    mode = (mode + 1) % MODE_AMT;
  }

  if ((digitalRead(MODE) == HIGH) && (MODEtriggerInterrupted == true)) {
    MODEtriggerInterrupted = false;  //Reset trigger flag
  }
}

void writeOutputs() {
  //Determines and writes the output states depending on the counter value, the mode and the previous output states.
  if (mode == 2) {
    //Harmonic mode
    if (counter % 1 == 0) {
      digitalWrite(OUT1, OUT1State);
      OUT1State = !OUT1State;
    }

    if (counter % 2 == 0) {
      digitalWrite(OUT2, OUT2State);
      OUT2State = !OUT2State;
    }

    if (counter % 3 == 0) {
      digitalWrite(OUT3, OUT3State);
      OUT3State = !OUT3State;
    }

    if (counter % 4 == 0) {
      digitalWrite(OUT4, OUT4State);
      OUT4State = !OUT4State;
    }

    if (counter % 5 == 0) {
      //   digitalWrite(OUT5, OUT5State);
      OUT5State = !OUT5State;
    }
  }
  if (mode == 1) {
    //Prime Number mode
    if (counter % 1 == 0) {
      digitalWrite(OUT1, OUT1State);
      OUT1State = !OUT1State;
    }

    if (counter % 2 == 0) {
      digitalWrite(OUT2, OUT2State);
      OUT2State = !OUT2State;
    }

    if (counter % 3 == 0) {
      digitalWrite(OUT3, OUT3State);
      OUT3State = !OUT3State;
    }

    if (counter % 5 == 0) {
      digitalWrite(OUT4, OUT4State);
      OUT4State = !OUT4State;
    }

    if (counter % 7 == 0) {
      digitalWrite(OUT5, OUT5State);
      OUT5State = !OUT5State;
    }
  }
  if (mode == 0) {
    //Binary Counting mode
    if (counter % 1 == 0) {
      digitalWrite(OUT1, OUT1State);
      OUT1State = !OUT1State;
    }

    if (counter % 2 == 0) {
      digitalWrite(OUT2, OUT2State);
      OUT2State = !OUT2State;
    }

    if (counter % 4 == 0) {
      digitalWrite(OUT3, OUT3State);
      OUT3State = !OUT3State;
    }

    if (counter % 8 == 0) {
      digitalWrite(OUT4, OUT4State);
      OUT4State = !OUT4State;
    }

    if (counter % 16 == 0) {
      digitalWrite(OUT5, OUT5State);
      OUT5State = !OUT5State;
    }
  }
  //if(mode = 3){} //Add your own modes
}
