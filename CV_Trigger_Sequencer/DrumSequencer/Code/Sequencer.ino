//Example code, not correct yet.

const int outputPins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Define the output pins
const int CLKtriggerPin = 11; // Pin to detect rising edge
const int DIRPin = 13;
const int RSTtriggerPin = 10; // Pin to detect rising edge
const int STOPPin = 12;

volatile bool CLKtriggered = false; // Flag to indicate trigger
volatile bool RSTtriggered = false; // Flag to indicate trigger

byte sequence[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, // Example sequence, replace with your own
                   0b00010000, 0b00100000, 0b01000000, 0b10000000};
int step = 0; // Step counter

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(outputPins[i], OUTPUT); // Set output pins as OUTPUT
  }

  pinMode(STOPPin, INPUT);
  pinMode(DIRPin, INPUT);
  pinMode(CLKtriggerPin, INPUT_PULLUP); // Set trigger pin as INPUT with internal pull-up resistor
  pinMode(RSTtriggerPin, INPUT_PULLUP); // Set trigger pin as INPUT with internal pull-up resistor
  
  
  attachInterrupt(digitalPinToInterrupt(CLKtriggerPin), CLKtriggerStep, RISING); // Attach interrupt for rising edge 
  attachInterrupt(digitalPinToInterrupt(RSTtriggerPin), RSTtriggerStep, RISING); // Attach interrupt for rising edge
  
}

void loop() {

    if(analogRead(STOPPin)<10){
      
  if (CLKtriggered) {
    writeToOutputs(sequence[step]);// Get the byte data for the current step
    
    if(digitalRead(DIRPin)==LOW){
    step = (step + 1) % 8; 
    }else{
    step = (step - 1) % 8;   
    }
    CLKtriggered = false; // Reset trigger flag
  }

if (RSTtriggered) {
  step = 0;
    writeToOutputs(sequence[step]);// Get the byte data for the current step
    RSTtriggered = false; // Reset trigger flag
  }
}
}

void writeToOutputs(byte outputByte){
for (int i = 0; i < 8; i++) {
      if (outputByte & (1 << i)) {
        digitalWrite(outputPins[i], HIGH); // If the i-th bit is set, turn the corresponding output pin HIGH
      } else {
        digitalWrite(outputPins[i], LOW); // If the i-th bit is not set, turn the corresponding output pin LOW
      }
    }
}
void CLKtriggerStep() {
  CLKtriggered = true; // Set trigger flag on rising edge
}

void RSTtriggerStep() {
  RSTtriggered = true; // Set trigger flag on rising edge
}
