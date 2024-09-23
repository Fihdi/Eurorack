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
int mode = 1;

//Interrupt flags
volatile bool CLKtriggerInterrupted = false;
volatile bool RSTtriggerInterrupted = false;
volatile bool MODEtriggerInterrupted = false;

//Interrupt conditions
volatile bool MODEtriggered = false;
volatile bool CLKtriggered = false;
volatile bool RSTtriggered = false;

//Previous Output states
bool OUT1State = false;
bool OUT2State = false;
bool OUT3State = false;
bool OUT4State = false;
bool OUT5State = false;

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
  writeOutputs();
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
    RSTtriggerInterrupted = false;  //Reset trigger flag
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

void checkMode() {

  MODEtriggered = (digitalRead(MODE) == LOW) && (MODEtriggerInterrupted == false);

  if (MODEtriggered) {
    MODEtriggerInterrupted = true;
    //Change the mode, modulo 3 to prevent the mode counter to get invalid values. 
    mode = (mode + 1) % 3;
  }

  if ((digitalRead(MODE) == HIGH) && (MODEtriggerInterrupted == true)) {
    MODEtriggerInterrupted = false;  //Reset trigger flag
  }
}

void writeOutputs() {
  //Determines and writes the output states depending on the counter value, the mode and the previous output states.
  if (mode == 1) {
    //Binary Counting mode
    digitalWrite(OUT1, counter % 2 == 0 ? LOW : HIGH);

    if (counter % 4 == 0) {
      digitalWrite(OUT2, OUT2State ? LOW : HIGH);
      OUT2State = !OUT2State;
    }

    if (counter % 8 == 0) {
      digitalWrite(OUT3, OUT3State ? LOW : HIGH);
      OUT3State = !OUT3State;
    }

    if (counter % 16 == 0) {
      digitalWrite(OUT4, OUT4State ? LOW : HIGH);
      OUT4State = !OUT4State;
    }

    if (counter % 32 == 0) {
      digitalWrite(OUT5, OUT5State ? LOW : HIGH);
      OUT5State = !OUT5State;
    }
  }

  if (mode == 2) {
    //Prime Number mode
    digitalWrite(OUT1, counter % 2 == 0 ? LOW : HIGH);

    if (counter % 3 == 0) {
      digitalWrite(OUT2, OUT2State ? LOW : HIGH);
      OUT2State = !OUT2State;
    }
    if (counter % 5 == 0) {
      digitalWrite(OUT3, OUT3State ? LOW : HIGH);
      OUT3State = !OUT3State;
    }
    if (counter % 7 == 0) {
      digitalWrite(OUT4, OUT4State ? LOW : HIGH);
      OUT4State = !OUT4State;
    }
    if (counter % 11 == 0) {
      digitalWrite(OUT5, OUT5State ? LOW : HIGH);
      OUT5State = !OUT5State;
    }
  }
  
  if (mode == 3) {
    //Harmonic mode
    digitalWrite(OUT1, counter % 2 == 0 ? LOW : HIGH);

    if (counter % 3 == 0) {
      digitalWrite(OUT2, OUT2State ? LOW : HIGH);
      OUT2State = !OUT2State;
    }
    if (counter % 4 == 0) {
      digitalWrite(OUT3, OUT3State ? LOW : HIGH);
      OUT3State = !OUT3State;
    }
    if (counter % 5 == 0) {
      digitalWrite(OUT4, OUT4State ? LOW : HIGH);
      OUT4State = !OUT4State;
    }
    if (counter % 6 == 0) {
      digitalWrite(OUT5, OUT5State ? LOW : HIGH);
      OUT5State = !OUT5State;
    }
  }
}
