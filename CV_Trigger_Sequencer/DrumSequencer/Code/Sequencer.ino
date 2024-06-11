//Code by Fihdi 1st July 2024

const int outputPins[] = { 9, 10, 7, 5, 6, 23, 24, 8};  // Define the output pins //A0 is pin 23 //A1 is pin 24
const int CLKtriggerPin = 0; //Clock Input Pin, when this Pin has a rising edge, the sequencer will jump to the next step.
const int DIRPin = 4; //Direction Pin, while this pin is HIGH, the sequencer runs in reverse
const int RSTtriggerPin = 12;  // Reset Trigger, sets the step counter back to 0.
const int STOPPin = 1; //Stop Pin, while this pin is HIGH, the sequencer will stop. Clock, Reset and Direction Pins will have no effects.
const int pushButtonPin = 17;

//Trigger flags for Rising edge detection
volatile bool CLKTriggerInterrupted = false;
volatile bool RSTtriggerInterrupted = false;
volatile bool CLKtriggered = false;
volatile bool RSTtriggered = false; 

byte sequence[] = { 0b00000001, 0b00000010, 0b00000100, 0b00001000,
                    0b00010000, 0b00100000, 0b01000000, 0b10000000 };
int step = 0;  // Step counter

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(outputPins[i], OUTPUT);  // Set output pins as OUTPUT
  }

  pinMode(STOPPin, INPUT);
  pinMode(DIRPin, INPUT);
  pinMode(CLKtriggerPin, INPUT);
  pinMode(RSTtriggerPin, INPUT);    
  pinMode(pushButtonPin, INPUT);    
}

void loop() {

if(digitalRead(STOPPin)!=HIGH){
//Only proceed as long as the STOPPin is low
  
  CLKtriggered = (digitalRead(CLKtriggerPin) == HIGH || digitalRead(pushButtonPin) == HIGH ) && (CLKTriggerInterrupted == false);
  
  if (CLKtriggered) {
    //CLK Triggered
    CLKTriggerInterrupted = true;
    writeToOutputs(sequence[step]);  // Get the byte data for the current step

    if (digitalRead(DIRPin) == 0) {
      //Increment step if the direction Pin is LOW
      step = (step + 1) % 8;
    } else {
      //Decrement step if the direction Pin is HIGH
      step = (step - 1 + 8) % 8;
    }
  }

if((digitalRead(CLKtriggerPin) == LOW || digitalRead(pushButtonPin) == LOW )&& (CLKTriggerInterrupted == true)){
CLKTriggerInterrupted = false; //Reset trigger flag 
}
  
  RSTtriggered = (digitalRead(RSTtriggerPin) == HIGH) && (RSTtriggerInterrupted == false);

  if (RSTtriggered) {
    //Resetted
    RSTtriggerInterrupted=true;
    step = 0;
    writeToOutputs(sequence[step]);  // Get the byte data for the current step
  }

if((digitalRead(RSTtriggerPin) == LOW)&& (RSTtriggerInterrupted == true)){
RSTtriggerInterrupted = false; //Reset trigger flag 
}
}
}

void writeToOutputs(byte outputByte) {
//Turns "outputByte" into 8 bits and those 8 bits are mapped to the outputs.
if(outputByte & (1 << 0)){
digitalWrite(outputPins[0], HIGH);
}else{
  digitalWrite(outputPins[0], LOW);
}

if(outputByte & (1 << 1)){
digitalWrite(outputPins[1], HIGH);
}else{
  digitalWrite(outputPins[1], LOW);
}

if(outputByte & (1 << 2)){
digitalWrite(outputPins[2], HIGH);
}else{
  digitalWrite(outputPins[2], LOW);
}

if(outputByte & (1 << 3)){
digitalWrite(outputPins[3], HIGH);
}else{
  digitalWrite(outputPins[3], LOW);
}

if(outputByte & (1 << 4)){
  digitalWrite(outputPins[4], HIGH);
}else{
   digitalWrite(outputPins[4], LOW);
}

if(outputByte & (1 << 5)){
analogWrite(A1, 255);
}else{
  analogWrite(A1, 0);
}

if(outputByte & (1 << 6)){
analogWrite(A0, 255);
}else{
  analogWrite(A0, 0);
}

if(outputByte & (1 << 7)){
digitalWrite(outputPins[7], HIGH);
}else{
  digitalWrite(outputPins[7], LOW);
}

}
