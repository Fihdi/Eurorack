//Code by Fihdi 1st July 2024

const int outputPins[] = { 9, 10, 7, 5, 6, 23, 24, 8};  // Define the output pins //A0 is pin 23 //A1 is pin 24
const int analogPin = 17; //Analog Pin
const int STOPPin = 1; // Stop Pin


byte sequence[] = { 0b00000001, 0b00000010, 0b00000100, 0b00001000,
                    0b00010000, 0b00100000, 0b01000000, 0b10000000 };

int index = 0; //Index for mapping


void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(outputPins[i], OUTPUT);  // Set output pins as OUTPUT
  }
  pinMode(analogPin, INPUT);
}

void loop() {

if(digitalRead(STOPPin)!=HIGH){

	index = analogRead(analogPin)/180;

	writeToOutputs(sequence[index]);
}
delay(10);
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