//Send gates or triggers into the CLK IN input.
//The gate (or trigger) will randomly be routed to either OUT1 or OUT2 depending on a probability.
//This probability is set by the potentiometer for the amount of steps at the first Euclid Generator.

//Inputs
#define CLK 4         //External Clock Pin
#define S1_IN A3  //Voltage for Amount of steps of the first Euclid Rhythm

//Outputs
#define CLKO 2  //Internal Clock Output
#define OUT1 8  //Euclid. Ryhthm 1 Output
#define OUT2 7  //Euclid. Ryhthm 2 Output

int ref = 0;
//Interrupt flags
volatile bool CLKtriggerInterrupted = false;
volatile bool CLKtriggered = false;


void setup() {

  //No INPUT_PULLUP needed because of the external 10k resistors.
  Serial.begin(9600);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(S1_IN, INPUT);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(CLKO, OUTPUT);

}

void loop() {

  checkClock();
  //Inputs have pullup resistors instead of pulldown, thus the trigger conditioning is inverted from e.g.: my Sequencer.
}

void checkClock() {

  CLKtriggered = (digitalRead(CLK) == LOW) && (CLKtriggerInterrupted == false);

  if (CLKtriggered) {
    CLKtriggerInterrupted = true;

    //Triggered
   digitalWrite(CLKO, HIGH);

writeOutputs();

  }

  if ((digitalRead(CLK) == HIGH) && (CLKtriggerInterrupted == true)) {
    CLKtriggerInterrupted = false;  //Reset Clock flag

    digitalWrite(CLKO, LOW);
    digitalWrite(OUT1, LOW);
    digitalWrite(OUT2, LOW);
  }
}


void writeOutputs() {
//New refom number
  ref = random(1024);
  Serial.println(ref);
//Output Bernoulli Gates

if(ref > analogRead(S1_IN)){
  digitalWrite(OUT1, HIGH);
  digitalWrite(OUT2, LOW);
}else{
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, HIGH);
}

}
