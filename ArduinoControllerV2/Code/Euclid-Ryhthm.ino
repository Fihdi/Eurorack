/*ClockModule

Inputs:
A ... Master Clock Rate CV
B ... Length CV
C ... Steps CV

Outputs:
Acv...Output Clock 
Bcv...Output Clock 2
Ccv...Euclidean Rythm
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

const int minDelay = 60;     // Minimum delay in milliseconds (sets the maximum frequency) 
const int maxDelay = 2000;   // Maximum delay in milliseconds (sets the minimum frequency)

struct Clock {
  unsigned long previousMillis; // Variable to store the current state and the last toggle time for each clock
  int delayTime;
  int state;
  int outputPin;
  int inputPin;
};
Clock master;
Clock divided;

int shiftCounter = 0;

volatile bool shiftInterrupt = false;
volatile bool shifted = false;

// Euclidean rhythm array
bool euclideanRhythm[24]; // Maximum length of 24
int euclideanIndex = 0;

void setup() {
  master.inputPin = Ain;
  master.outputPin = Acv;
  master.previousMillis = 0;
  master.state = LOW;

  divided.outputPin = Bcv;
  divided.previousMillis = 0;
  divided.state = LOW;

  pinMode(master.outputPin, OUTPUT);
  pinMode(divided.outputPin, OUTPUT);
  pinMode(Ccv, OUTPUT);

  generateEuclideanRhythm(4, 8, euclideanRhythm); // Initial values
}

void loop() {
  updateOutput();
}

void updateOutput() {
  // Get the current time
  unsigned long currentMillis = millis();

  master.delayTime = map(analogRead(master.inputPin), 0, 1023, minDelay, maxDelay); // Set the frequency of the master
  
  divided.delayTime = master.delayTime * 4; // Set the frequency to 1/4 of the master

  if (currentMillis - master.previousMillis >= master.delayTime) {
    master.previousMillis = currentMillis;
    master.state = !master.state;
    digitalWrite(master.outputPin, master.state);
    
    // Advance the Euclidean rhythm
    int rhythmLength = map(analogRead(Bin), 0, 1023, 4, 24);
    int rhythmOnsets = min(map(analogRead(Cin), 0, 1023, 1, rhythmLength - 1), rhythmLength - 1);

    generateEuclideanRhythm(rhythmOnsets, rhythmLength, euclideanRhythm);

    if (euclideanRhythm[euclideanIndex]) {
      digitalWrite(Ccv, HIGH);
    } else {
      digitalWrite(Ccv, LOW);
    }
    euclideanIndex = (euclideanIndex + 1) % rhythmLength;
  }

  if (currentMillis - divided.previousMillis >= divided.delayTime) {
    divided.previousMillis = currentMillis;
    divided.state = !divided.state;
    digitalWrite(divided.outputPin, divided.state);
  }
}

// Function to generate a Euclidean rhythm
void generateEuclideanRhythm(int pulses, int steps, bool* rhythm) {
  int counts[steps];
  int remainders[steps];
  int divisor = steps - pulses;
  int level = 0;

  remainders[0] = pulses;

  while (true) {
    counts[level] = divisor / remainders[level];
    remainders[level + 1] = divisor % remainders[level];
    divisor = remainders[level];
    level++;
    if (remainders[level] <= 1) {
      break;
    }
  }

  counts[level] = divisor;

  int k = 0;
  int j = 0;
  for (int i = 0; i < level + 1; i++) {
    for (int l = 0; l < counts[i]; l++) {
      rhythm[k++] = (i == 0);
    }
    if (k < steps) {
      j = i + 1;
      rhythm[k++] = (j < level && remainders[j] != 0);
    }
  }
}
