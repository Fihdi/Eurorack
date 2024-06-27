/*ClockModule

Inputs:
A ...Shift Trigger input when the level is above 800. Otherwise Rate CV for the Master clock.
B ...Rate CV for Clock 2.
C ...Rate CV for Clock 3.

Outputs:
Acv...Output Clock 1
Bcv...Output Clock 2
Ccv...Output Clock 3
*/

#define Acv 11
#define Bcv 9
#define Ccv 5

#define Aaud 10
#define Baud 6
#define Caud 3

#define Ain A1
#define Bin A2
#define Cin A3

const int minDelay = 50;     // Minimum delay in milliseconds (sets the maximum frequency)
const int maxDelay = 5000;   // Maximum delay in milliseconds (sets the minimum frequency)
const int shiftPin = A1;

const int clock0OriginalInputPin = A1;
const int clock1OriginalInputPin = A2;
const int clock2OriginalInputPin = A3;

struct Clock {
  unsigned long previousMillis; // Variable to store the current state and the last toggle time for each clock
  int delayTime;
  int state;
  int outputPin;
  int inputPin;
};

Clock clocks[3]; // Three output Clocks and one aux. Clock for shifting the parameters

int shiftCounter = 0;

volatile bool shiftInterrupt = false;
volatile bool shifted = false;

void setup() {
  clocks[0].inputPin = clock0OriginalInputPin;
  clocks[0].outputPin = Acv;

  clocks[1].inputPin = clock1OriginalInputPin;
  clocks[1].outputPin = Bcv;

  clocks[2].inputPin = clock2OriginalInputPin;
  clocks[2].outputPin = Ccv;

  for (int i = 0; i < 3; i++) {
    pinMode(clocks[i].outputPin, OUTPUT);
    clocks[i].previousMillis = 0;
    clocks[i].state = LOW;
  }
}

void loop() {
  // Check for Shift
  shifted = (analogRead(shiftPin) > 800) && (shiftInterrupt == false);

  if (shifted) {
    shiftInterrupt = true;
    shiftCounter = (shiftCounter + 1) % 6;

    if (shiftCounter == 0) {
      clocks[0].inputPin = clock0OriginalInputPin;
      clocks[1].inputPin = clock1OriginalInputPin;
      clocks[2].inputPin = clock2OriginalInputPin;
    } else if (shiftCounter == 1) {
      clocks[0].inputPin = clock2OriginalInputPin;
      clocks[1].inputPin = clock0OriginalInputPin;
      clocks[2].inputPin = clock1OriginalInputPin;
    } else if (shiftCounter == 2) {
      clocks[0].inputPin = clock1OriginalInputPin;
      clocks[1].inputPin = clock2OriginalInputPin;
      clocks[2].inputPin = clock0OriginalInputPin;
    } else if (shiftCounter == 3) {
      clocks[0].inputPin = clock1OriginalInputPin;
      clocks[1].inputPin = clock0OriginalInputPin;
      clocks[2].inputPin = clock2OriginalInputPin;
    } else if (shiftCounter == 4) {
      clocks[0].inputPin = clock2OriginalInputPin;
      clocks[1].inputPin = clock1OriginalInputPin;
      clocks[2].inputPin = clock0OriginalInputPin;
    } else if (shiftCounter == 5) {
      clocks[0].inputPin = clock0OriginalInputPin;
      clocks[1].inputPin = clock2OriginalInputPin;
      clocks[2].inputPin = clock1OriginalInputPin;
    }
  }

  if ((analogRead(shiftPin) < 800) && (shiftInterrupt == true)) {
    shiftInterrupt = false; // Reset Shift flag
  }

  updateClocks();
}

void updateClocks() {
  // Get the current time
  unsigned long currentMillis = millis();

  for (int i = 0; i < 3; i++) {
    clocks[i].delayTime = map(analogRead(clocks[i].inputPin), 0, 1023, minDelay, maxDelay);

    if (i == 0) {
      if (currentMillis - clocks[i].previousMillis >= clocks[i].delayTime) {
        clocks[i].previousMillis = currentMillis;
        clocks[i].state = !clocks[i].state;
        digitalWrite(clocks[i].outputPin, clocks[i].state);
      }
    } else {
      if (clocks[i - 1].state == LOW) {
        if (currentMillis - clocks[i].previousMillis >= clocks[i].delayTime) {
          clocks[i].previousMillis = currentMillis;
          clocks[i].state = !clocks[i].state;
          digitalWrite(clocks[i].outputPin, clocks[i].state);
        }
      } else {
        digitalWrite(clocks[i].outputPin, LOW);
        clocks[i].state = LOW;
      }
    }
  }
}
