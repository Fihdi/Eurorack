/* Euclidean Rhythm Generator
Inputs:
A ... Master Clock Rate CV
B ... Length CV
C ... Steps CV

Outputs:
Acv...Master Output Clock 
Bcv...Master Clock divided by 4
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

const int minDelay = 20;     // Minimum delay in milliseconds (sets the maximum frequency) 
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

// Euclidean rhythm array
bool rhythm[16]; // Maximum length of 16
int euclideanIndex = 0;

void setup() {

Serial.begin(9600);

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

updateOutputs();

}

void loop() {
  updateOutputs();
}

void updateOutputs() {

  // Get the current time
  
  unsigned long currentMillis = millis();

  master.delayTime = map(analogRead(master.inputPin), 0, 1023, minDelay, maxDelay); // Set the frequency of the master
  divided.delayTime = master.delayTime * 4; // Set the frequency to 1/4 of the master

//Write Master Clock and advance the euclidean output
  if (currentMillis - master.previousMillis >= master.delayTime) {
    master.previousMillis = currentMillis;
    master.state = !master.state;
    digitalWrite(master.outputPin, master.state);
    
    // Read in Parameters for the Euclid Rhythm
    int rhythmLength = map(analogRead(Bin), 0, 1023, 4, 16);
    int rhythmOnsets = min(map(analogRead(Cin), 0, 1023, 1, rhythmLength - 1), rhythmLength - 1);

    generateEuclid(rhythmLength, rhythmOnsets);

    //Write Euclid Output but only if the master state changed into the high state (Rising edge)
if(master.state == HIGH){	  
    if (rhythm[euclideanIndex % rhythmLength])  {
      digitalWrite(Ccv, HIGH);
    } else {
      digitalWrite(Ccv, LOW);
    }
    euclideanIndex++;
  }else{
digitalWrite(Ccv, LOW);
}
  }
  
//Write the divided clock
  if (currentMillis - divided.previousMillis >= divided.delayTime) {
    divided.previousMillis = currentMillis;
    divided.state = !divided.state;
    digitalWrite(divided.outputPin, divided.state);
  }
}

void generateEuclid(int length, int onsets){

    int bucket = 0;
  // Fill the track with the rhythm
  for (int i = 0; i < length; i++) {
    bucket += onsets;
    if (bucket >= length) {
      bucket -= length;
      rhythm[i] = 1;
    } else {
      rhythm[i] = 0;
    }
  }

}
