//Code by Fihdi 1st July 2024

const int outputPins[] = { 9, 10, 7, 5, 6, 23, 24, 8};  // Define the output pins //A0 is pin 23 //A1 is pin 24

const int CLKtriggerPin = 0;                           // Clock Input (Enable)
const int STEPPin = 4; //Step Input Pin
const int STOPPin = 1;


volatile bool CLKTriggerInterrupted = false;  // Flag to indicate trigger
volatile bool RSTtriggerInterrupted = false;
volatile bool CLKtriggered = false;
volatile bool RSTtriggered = false;  // Flag to indicate trigger

byte sequence[] = { 0b00000001, 0b00000010, 0b00000100, 0b00001000,  // Example sequence, replace with your own
                    0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b00000000 };
					
					
int step = 0;  // Step counter

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(outputPins[i], OUTPUT);  // Set output pins as OUTPUT
  }

  pinMode(STEPPin, INPUT);
  pinMode(CLKtriggerPin, INPUT);
}

void loop() {
//As long as STOPPin is held HIGH, nothing should happen

if(digitalRead(STOPPin)!=HIGH){

  CLKtriggered = (digitalRead(CLKtriggerPin) == HIGH) && (CLKTriggerInterrupted == false); //Check for Trigger-Condition 
  
  if (CLKtriggered) {
    CLKTriggerInterrupted = true; //Set Interrupt flag
	step = (step + 1)%8;
	
  }

if((digitalRead(CLKtriggerPin) == LOW )&& (CLKTriggerInterrupted == true)){
CLKTriggerInterrupted = false; //Reset Interrupt Flag

}

if(digitalRead(STEPPin)==HIGH){
	writeToOutputs(sequence[step]);
}else{
	writeToOutputs(sequence[8]);
}

}
}

void writeToOutputs(byte outputByte) {

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
analogWrite(A0, 255);
}else{
  analogWrite(A0, 0);
}

if(outputByte & (1 << 6)){
analogWrite(A1, 255);
}else{
  analogWrite(A1, 0);
}

if(outputByte & (1 << 7)){
digitalWrite(outputPins[7], HIGH);
}else{
  digitalWrite(outputPins[7], LOW);
}

}