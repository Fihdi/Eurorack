//Send gates or triggers into the CLK IN input.
//The gate (or trigger) will randomly be routed to either OUT1 or OUT2 depending on a probability.
//This probability is set by the rate potentiometer

//Inputs
#define CLK 4         //External Clock Pin
#define CLKI_RATE A0  //Voltage for Amount of steps of the first Euclid Rhythm

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
  pinMode(CLK, INPUT_PULLUP);
  pinMode(CLKI_RATE, INPUT);

  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(CLKO, OUTPUT);

}

void loop() {

  checkClock();
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
    //Reset the Outputs
    digitalWrite(CLKO, LOW);
    digitalWrite(OUT1, LOW);
    digitalWrite(OUT2, LOW);
  }
}

void writeOutputs() {
//New random number
  ref = random(1024);
//Output Bernoulli Gates

if(ref > analogRead(CLKI_RATE)){
  digitalWrite(OUT1, HIGH);
  digitalWrite(OUT2, LOW);
}else{
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, HIGH);
}

}
