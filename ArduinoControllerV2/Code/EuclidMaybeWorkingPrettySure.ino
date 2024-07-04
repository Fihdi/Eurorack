/* V2 Euclidean Rhythm Generator

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
bool rhythm[16]; // Maximum length of 16
int euclideanIndex = 0;

int findlength(unsigned int bnry) {
  bool lengthfound = false;
  int length = 1; // no number can have a length of zero - single 0 has a length of one, but no 1s for the sytem to count
  for (int q = 32; q >= 0; q--) {
    int r = bitRead(bnry, q);
    if (r == 1 && lengthfound == false) {
      length = q + 1;
      lengthfound = true;
    }
  }
  return length;
}

//-----2--------Function to concatenate two binary numbers bitwise----------------------//
unsigned int ConcatBin(unsigned int bina, unsigned int binb) {
  int binb_len = findlength(binb);
  unsigned int sum = (bina << binb_len);
  sum = sum | binb;
  return sum;
}

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

updateOutput();

}

void loop() {
}

void updateOutput() {

  Serial.println("In method");
/*
  int length = 13;

  uint16_t result = euclid(13, 5);

  result = result << (16-length);

  for (int i = 15; i >= 0; i--) {
            rhythm[i] = (result & (1 << i));
    }

Serial.println(result);


Serial.print("[");
for(int x = 15; x>=0; x--){

if(rhythm[x]){
Serial.print("1");
}else{
Serial.print("0");
}
}
Serial.println("]");

*/
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

	//Only advance the euclidean Rhythm when the master clock goes high (rising edge)
	
    //generateEuclideanRhythm(rhythmOnsets, rhythmLength, euclideanRhythm);
    

   uint16_t result = euclid(rhythmLength, rhythmOnsets);

  result = result << (16-rhythmLength);

  Serial.println(result);

  for (int i = 15; i >= 0; i--) {
            rhythm[i] = (result & (1 << i));
    }

Serial.print("[");
for(int x = 0; x<16; x++){

if(rhythm[x]){
Serial.print("x");
}else{
Serial.print(".");
}
}
Serial.println("]");

    if (rhythm[(15 - euclideanIndex) % rhythmLength]) {
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
unsigned int euclid(int n, int k) { // inputs: n=total, k=beats
  int pauses = n - k;
  int pulses = k;
  int steps = n;
  int per_pulse = pauses / k;
  int remainder = pauses % pulses;
  unsigned int workbeat[n];
  unsigned int outbeat;
  int workbeat_count = n;
  int a;
  int b;
  int trim_count;

  for (a = 0; a < n; a++) { // Populate workbeat with unsorted pulses and pauses
    if (a < pulses) {
      workbeat[a] = 1;
    }
    else {
      workbeat[a] = 0;
    }
  }

  if (per_pulse > 0 && remainder < 2) { // Handle easy cases where there is no or only one remainer
    for (a = 0; a < pulses; a++) {
      for (b = workbeat_count - 1; b > workbeat_count - per_pulse - 1; b--) {
        workbeat[a] = ConcatBin(workbeat[a], workbeat[b]);
      }
      workbeat_count = workbeat_count - per_pulse;
    }

    outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count
    for (a = 0; a < workbeat_count; a++) {
      outbeat = ConcatBin(outbeat, workbeat[a]);
    }

    return outbeat;
  }

  else {
    if (pulses == 0) {
      pulses = 1;  // Prevent crashes when k=0 and n goes from 0 to 1
    }
    int groupa = pulses;
    int groupb = pauses;
    int iteration = 0;
    if (groupb <= 1) {
    }

    while (groupb > 1) { //main recursive loop
      
      if (groupa > groupb) { // more Group A than Group B
        int a_remainder = groupa - groupb; // what will be left of groupa once groupB is interleaved
        trim_count = 0;
        for (a = 0; a < groupa - a_remainder; a++) { //count through the matching sets of A, ignoring remaindered
          workbeat[a] = ConcatBin(workbeat[a], workbeat[workbeat_count - 1 - a]);
          trim_count++;
        }
        workbeat_count = workbeat_count - trim_count;

        groupa = groupb;
        groupb = a_remainder;
      }

      else if (groupb > groupa) { // More Group B than Group A
        int b_remainder = groupb - groupa; // what will be left of group once group A is interleaved
        trim_count = 0;
        for (a = workbeat_count - 1; a >= groupa + b_remainder; a--) { //count from right back through the Bs
          workbeat[workbeat_count - a - 1] = ConcatBin(workbeat[workbeat_count - a - 1], workbeat[a]);

          trim_count++;
        }
        workbeat_count = workbeat_count - trim_count;
        groupb = b_remainder;
      }

      else if (groupa == groupb) { // groupa = groupb
        trim_count = 0;
        for (a = 0; a < groupa; a++) {
          workbeat[a] = ConcatBin(workbeat[a], workbeat[workbeat_count - 1 - a]);
          trim_count++;
        }
        workbeat_count = workbeat_count - trim_count;
        groupb = 0;
      }

      else {
        //Serial.println("ERROR");
      }
      iteration++;
    }

    outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count
    for (a = 0; a < workbeat_count; a++) {
      outbeat = ConcatBin(outbeat, workbeat[a]);
    }
    return outbeat;
  }
}
