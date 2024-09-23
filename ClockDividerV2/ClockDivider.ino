//Inputs
#define CLK 11
#define MODE 9
#define RST 10

/Outputs
#define OUT1 A2
#define OUT2 12
#define OUT3 9
#define OUT4 10
#define OUT5 5

int counter = 0;
int mode = 1;

//Trigger flags
volatile bool CLKtriggerInterrupted = false;
volatile bool RSTtriggerInterrupted = false;
volatile bool MODEtriggerInterrupted = false;

volatile bool MODEtriggered = false;
volatile bool CLKtriggered = false;
volatile bool RSTtriggered = false;

//previous output states
bool OUT1State = false;
bool OUT2State = false;
bool OUT3State = false;
bool OUT4State = false;
bool OUT5State = false;

void setup() {

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
  checkClock();
  checkReset();
  checkMode();

  writeOutputs();
}

void checkReset() {
  RSTtriggered = (digitalRead(RST) == HIGH) && (RSTtriggerInterrupted == false);

  if (RSTtriggered) {
    //Reset triggered
    RSTtriggerInterrupted = true;
    counter = 0;  //Reset counter
  }

  if ((digitalRead(RST) == LOW) && (RSTtriggerInterrupted == true)) {
    RSTtriggerInterrupted = false;  //Reset trigger flag
  }
}

void checkClock() {

  CLKtriggered = (digitalRead(CLK) == HIGH) && (CLKtriggerInterrupted == false);

  if (CLKtriggered) {
    //Clock triggered
    CLKtriggerInterrupted = true;
    counter++;  //Increase counter
  }

  if ((digitalRead(CLK) == LOW) && (CLKtriggerInterrupted == true)) {
    CLKtriggerInterrupted = false;  //Reset clock flag
  }
}

void checkMode() {

  MODEtriggered = (digitalRead(MODE) == HIGH) && (MODEtriggerInterrupted == false);

  if (MODEtriggered) {
    //Mode switched
    MODEtriggerInterrupted = true;
    mode = (mode + 1) % 3; //Increase the mode counter
  }

  if ((digitalRead(MODE) == LOW) && (MODEtriggerInterrupted == true)) {
    MODEtriggerInterrupted = false;  //Reset mode flag
  }
}

void writeOutputs() {
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

  //if(mode == 4, 5, 6 etc){} make your own modes.
}
