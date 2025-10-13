/*

Code by Fihdi

Matrix Keypad values:

13 = no key

col 1, 2, 3, 4
_________________
| 1 | 2 | 3 | 4 | row1 
_________________
| 5 | 6 | 7 | 8 | row2
_________________
| 9 | 10| 11| 12| row3
_________________

Scale:
col 1, 2, 3, 4
_________________
| C | C#| D | D#| row1 
_________________
| E | F | F#| G | row2
_________________
| G#| A | A#| B | row3
_________________

*/


#include "TCA9555.h"

#define TRG 9  //TRG Input Pin
#define CVIN A1   //CV Input Pin
#define ARP 0   //ARP Input Pin

#define CVOUT 10   //CV Output Pin
#define AUD 11  //Audio Output Pin

//Matrix Keypad Pins
#define ROW1 6
#define ROW2 7
#define ROW3 8
#define COL1 3
#define COL2 2
#define COL3 4
#define COL4 5

//LED Pins 
TCA9535 TCA(0x20);
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED7 7
#define LED8 8
#define LED9 9
#define LED10 10
#define LED11 11
#define LED12 12

#define LEDUP 13
#define LEDDWN 14
#define LEDUND 15

int leds[12] = { LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10, LED11, LED12 };

int noteInOctave = 0;
int currentSemitone = 0; //currently played note
int currentOctave = 0; //currently played octave BASED on CV only (MUST NOT BE increased when the arp is going over 2 octaves)
int lastquantized = 0;

//It takes (1023/5)*(1/12) = 17.05 bits to change one semitone, thus we can set the hysteresis to 14.
int hysteresis = 14;

bool scale[12] = {
  true,   // C
  false,  // C#
  true,   // D
  false,  // D#
  true,   // E
  true,   // F
  false,  // F#
  true,   // G
  false,  // G#
  true,   // A
  false,  // A#
  true    // B
};

int arpScale[48] = {};
int arpIndex = 0;      // current position in the arpeggio
int arpNoteAmount = 0; //Amount of notes in the scales
int arpMode = 1; // 1 = UP 2 Octaves, 2 = DOWN 2 Octaves, 3 = UP and DOWN 2 octaves.

volatile bool TRGInterrupted = false;
volatile bool TRGtriggered = false;

volatile bool ARPInterrupted = false;
volatile bool ARPtriggered = false;

volatile bool keypadInterrupted;

volatile int lastCV;

void setup() {
  Wire.begin();
  TCA.begin();
  Wire.setClock(50000);

//Increase PWM Frequency
  TCCR0A = 0b00000011;
  TCCR0B = 0b00000001;
  TCCR1A = 0b00000001;
  TCCR1B = 0b00001001;
  TCCR2A = 0b00000011;
  TCCR2B = 0b00000001;

  Serial.begin(9600);

  pinMode(ROW1, INPUT);
  pinMode(ROW2, INPUT);
  pinMode(ROW3, INPUT);

  pinMode(COL1, OUTPUT);
  pinMode(COL2, OUTPUT);
  pinMode(COL3, OUTPUT);
  pinMode(COL4, OUTPUT);

  pinMode(TRG, INPUT_PULLUP);
  pinMode(CVIN, INPUT);
  pinMode(CVOUT, OUTPUT);
  pinMode(AUD, OUTPUT);

  for (int i = 0; i < 12; i++) {
    TCA.pinMode1(leds[i], OUTPUT);
    TCA.write1(leds[i], HIGH);
    delay(700);
  }
  TCA.pinMode1(LEDUP, OUTPUT);
  TCA.pinMode1(LEDDWN, OUTPUT);
  TCA.pinMode1(LEDUND, OUTPUT);
  TCA.pinMode1(ARP, INPUT);

  buildARPScale();
}

// Scale: true = note is allowed in the scale
// Index 0 = C, 1 = C#, ..., 11 = B

//find the nearest allowed semitone
int quantizeToScale(int semitone) {
  int octave = semitone / 12;
  noteInOctave = semitone % 12;
  // Serial.println(noteInOctave);

  // Search outward for closest allowed note in the scale
  for (int offset = 0; offset < 12; offset++) {
    // Check upward
    int up = (noteInOctave + offset) % 12;
    if (scale[up]) {
      //  lastquantized = octave * 12 + up;
      return octave * 12 + up;
    }
    // Check downward
    int down = (noteInOctave - offset + 12) % 12;
    if (scale[down]) {
      // lastquantized = octave * 12 + down;
      return octave * 12 + down;
    }
  }
  // Fallback: return unquantized semitone if no scale notes found (shouldn’t happen)
  return semitone;
}

void loop() {
checkKeypadTriggered();
checkInputCVChange();
checkTRG();
checkARP();
outputLeds();
delay(10);  // Optional smoothing delay
}

void checkInputCVChange(){
int raw = analogRead(CVIN);     // 0–1023
  //Only update the CV and AUD if the CV Input(Or offset knob) has changed
  if (((lastCV - raw) >= hysteresis)||((raw - lastCV) >= hysteresis)) {
    writeCVandAUD();
  }
}

void writeCVandAUD() {

  int raw = analogRead(CVIN);     // 0–1023

    float voltage = raw * (5.0 / 1023.0);
  // Convert to semitone number (0–60): 12 semitones per volt
  int semitone = round(voltage * 12.0);

    lastCV = raw;
    //value decreased
    currentSemitone = quantizeToScale(semitone);
    currentOctave = currentSemitone/12;
    
  int frequency = 440 * pow(2.0, (currentSemitone - 49.0 - 4.0 /*offset for so the base tone is C*/) / 12.0);
  tone(AUD, (int)frequency);
  float quantizedVoltage = currentSemitone / 12.0;
  int pwmValue = map(quantizedVoltage * 100, 0, 500, 0, 255);
  pwmValue = constrain(pwmValue, 0, 255);
  analogWrite(CVOUT, pwmValue);
  
}

void outputLeds() {
  //Scale LEDs
  for (int i = 0; i < 12; i++) {
    if (scale[i]) {
      if( (currentSemitone % 12) != i){
        TCA.write1(leds[i], HIGH);  //Write LED HIGH if it is in the scale and NOT the current note being played.
      }else{
TCA.write1(leds[i], LOW);
      }
    }else{
      TCA.write1(leds[i], LOW);
    }
    
  }
  //ARP Mode 
    if(arpMode==1){
      //UP 2 Octaves
      TCA.write1(LEDUP, LOW);
      TCA.write1(LEDDWN, HIGH);
      TCA.write1(LEDUND, HIGH);
}
if(arpMode==2){
      //DOWN 2 Octaves
      TCA.write1(LEDUP, HIGH);
      TCA.write1(LEDDWN, LOW);
      TCA.write1(LEDUND, HIGH);
}
if(arpMode==3){
      //UP and DOWN 2 Octaves
      TCA.write1(LEDUP, HIGH);
      TCA.write1(LEDDWN, HIGH);
      TCA.write1(LEDUND, LOW);
}
if(arpMode==4){
      //UP 1 Octave
      TCA.write1(LEDUP, HIGH);
      TCA.write1(LEDDWN, LOW);
      TCA.write1(LEDUND, LOW);
}
if(arpMode==5){
      //DOWN 1 Octave
      TCA.write1(LEDUP, LOW);
      TCA.write1(LEDDWN, HIGH);
      TCA.write1(LEDUND, LOW);
}
if(arpMode==0){
      //UP and DOWN Octave
      TCA.write1(LEDUP, LOW);
      TCA.write1(LEDDWN, LOW);
      TCA.write1(LEDUND, HIGH);
}

}


int readMatrixKeypad() {
  //Check Column 1:
  digitalWrite(COL1, HIGH);
  digitalWrite(COL2, LOW);
  digitalWrite(COL3, LOW);
  digitalWrite(COL4, LOW);

  //Read in the rows
  if (digitalRead(ROW1) == 1) { /*1-1*/
    return 1;
  }
  if (digitalRead(ROW2) == 1) { /*1-2*/
    return 5;
  }
  if (digitalRead(ROW3) == 1) { /*1-3*/
    return 9;
  }
  //Check Column 2:
  digitalWrite(COL1, LOW);
  digitalWrite(COL2, HIGH);
  digitalWrite(COL3, LOW);
  digitalWrite(COL4, LOW);

  if (digitalRead(ROW1) == 1) { /*2-1*/
    return 2;
  }
  if (digitalRead(ROW2) == 1) { /*2-2*/
    return 6;
  }
  if (digitalRead(ROW3) == 1) { /*2-3*/
    return 10;
  }

  //Check Column 3:
  digitalWrite(COL1, LOW);
  digitalWrite(COL2, LOW);
  digitalWrite(COL3, HIGH);
  digitalWrite(COL4, LOW);

  if (digitalRead(ROW1) == 1) { /*3-1*/
    return 3;
  }
  if (digitalRead(ROW2) == 1) { /*3-2*/
    return 7;
  }
  if (digitalRead(ROW3) == 1) { /*3-3*/
    return 11;
  }

  //Check Column 4:
  digitalWrite(COL1, LOW);
  digitalWrite(COL2, LOW);
  digitalWrite(COL3, LOW);
  digitalWrite(COL4, HIGH);

  if (digitalRead(ROW1) == 1) { /*4-1*/
    return 4;
  }
  if (digitalRead(ROW2) == 1) { /*4-2*/
    return 8;
  }
  if (digitalRead(ROW3) == 1) { /*4-3*/
    return 12;
  }

  //No key pressed
  return 13;
}

void checkKeypadTriggered() {
  int keypad = readMatrixKeypad();

  bool keypadtriggered = (keypad != 13) && (keypadInterrupted == false); //A key was pressed AND we are not in the ISR yet 

  if (keypadtriggered) {
    Serial.println(keypad);
    keypadInterrupted = true;

    scale[keypad-1] = !scale[keypad-1];  //Invert the state for the pressed key.
    if(countNotes() == 0){
      scale[keypad-1] = !scale[keypad-1];  //Undo the Inverting if it resulted in all of the notes being off.
    }
        buildARPScale();

  }

  if ((keypad == 13) && (keypadInterrupted == true)) {
    keypadInterrupted = false;  //Reset  flag
  }
}

void checkARP() {
ARPtriggered = (TCA.read1(ARP) == LOW) && (ARPInterrupted == false);

if(ARPtriggered){
ARPInterrupted = true;

arpMode = (arpMode+1) % 6;
buildARPScale();
outputLeds();
}


if ((digitalRead(ARP) == HIGH) && (ARPInterrupted == true)) {
    ARPInterrupted = false;  //Reset trigger flag
  }

}

//Count the number of active keys in the scale
int countNotes(){
  int count = 0;
  for (int i = 0; i < 12; i++) {
    if(scale[i]){
      count++;
    }
 }
 return count;
}

void checkTRG() {

  TRGtriggered = (digitalRead(TRG) == LOW) && (TRGInterrupted == false);

  if (TRGtriggered) {
    //Triggered!
    TRGInterrupted = true;

    if(arpMode==1){
      //UP 2 Octaves
      Serial.println(arpIndex);
      Serial.println(arpNoteAmount);
      arpIndex = (arpIndex + 1) % (arpNoteAmount*2); //Increase/Cycle the index for the amount of notes in the scale
    }
    if(arpMode==2){
      //DOWN 2 Octaves
      arpIndex++;
      arpIndex = arpIndex % (arpNoteAmount*2); //Increase/Cycle the index for the amount of notes in the scale
    }
      if(arpMode==3){
      //UP AND DOWN 2 Octaves
      arpIndex++;
      arpIndex = arpIndex % ((arpNoteAmount*4)-2); //Increase/Cycle the index for the amount of notes in the scale
    }
    if(arpMode==4){
      //UP 1 Octaves
      arpIndex++;
      arpIndex = arpIndex % ((arpNoteAmount)); //Increase/Cycle the index for the amount of notes in the scale
    }
    if(arpMode==5){
      // DOWN 1 Octaves
      arpIndex++;
      arpIndex = arpIndex % ((arpNoteAmount)); //Increase/Cycle the index for the amount of notes in the scale
    }
    if(arpMode==0){
      //UP AND DOWN  1 Octaves
      arpIndex++;
      arpIndex = arpIndex % ((arpNoteAmount*2)-2); //Increase/Cycle the index for the amount of notes in the scale
    }
    
    currentSemitone = currentOctave*12 + arpScale[arpIndex];
    //OUTPUT CV and AUDIO
    int frequency = 440 * pow(2.0, (currentSemitone - 49.0 - 4.0 /*offset for so the base tone is C*/) / 12.0);
  tone(AUD, (int)frequency);
  float quantizedVoltage = currentSemitone / 12.0;
  int pwmValue = map(quantizedVoltage * 100, 0, 500, 0, 255);
  pwmValue = constrain(pwmValue, 0, 255);
  analogWrite(CVOUT, pwmValue);
    
    outputLeds();
  }

  if ((digitalRead(TRG) == HIGH) && (TRGInterrupted == true)) {
    TRGInterrupted = false;  //Reset trigger flag
  }
}

void buildARPScale(){
//Builds an array of "offsets" used for the Arpeggiator
//Example: the indexes for C major are (starting at 0): 0, 2, 4, 5, 7, 9, 11 for the first octave and 12, 14, 16, 17, 19, 21, 23 for the second octave(add 12 to every value).
//Using this array, we jump to the first note in the scale in the active octave and then output: currentoctave + offset
arpNoteAmount = 0;
if(arpMode == 1){
  //Mode UP 2 Octaves
for (int i = 0; i < 12; i++) {
  if(scale[i]){
    arpScale[arpNoteAmount] = i;
    arpNoteAmount++;
  }
}
for (int i = 0; i < arpNoteAmount; i++) {
    arpScale[i+arpNoteAmount] = arpScale[i] +12;
}

}
if(arpMode == 2){
  int temp;
  //Mode DOWN 2 Octaves
  //Same as UP 2 Octaves with a flipped array.
for (int i = 0; i < 12; i++) {
  if(scale[i]){
    arpScale[arpNoteAmount] = i;
    arpNoteAmount++;
  }
}
for (int i = 0; i < arpNoteAmount; i++) {
    arpScale[i+arpNoteAmount] = arpScale[i] +12;
}

//Flip
for (int i = 0; i < arpNoteAmount; i++) {
  temp = arpScale[i];
  arpScale[i] = arpScale[arpNoteAmount*2 - 1 - i];
  arpScale[arpNoteAmount*2 - 1 - i] = temp;
}
}
if(arpMode == 3){
 //Mode UP and DOWN 2 Octaves

 //Build the same array as in UP 2 Octaves
for (int i = 0; i < 12; i++) {
  if(scale[i]){
    arpScale[arpNoteAmount] = i;
    arpNoteAmount++;
  }
}
for (int i = 0; i < arpNoteAmount; i++) {
    arpScale[i+arpNoteAmount] = arpScale[i] +12;
}
//Attach the DOWN parts
for (int i = 0; i < arpNoteAmount*2; i++) {
    arpScale[arpNoteAmount*2 + i] = arpScale[arpNoteAmount*2 - 2 - i];
}
}
if(arpMode == 4){
 //Mode UP 1 Octaves
 for (int i = 0; i < 12; i++) {
  if(scale[i]){
    arpScale[arpNoteAmount] = i;
    arpNoteAmount++;
  }
}
}
if(arpMode == 5){
 //Mode DOWN 1 Octaves
 int temp;
  //Mode DOWN 2 Octaves
  //Same as UP 2 Octaves with a flipped array.
for (int i = 0; i < 12; i++) {
  if(scale[i]){
    arpScale[arpNoteAmount] = i;
    arpNoteAmount++;
  }
}
//Flip
for (int i = 0; i < arpNoteAmount / 2; i++) {
    int temp = arpScale[i];
    arpScale[i] = arpScale[arpNoteAmount - 1 - i];
    arpScale[arpNoteAmount - 1 - i] = temp;
  }
}
if(arpMode == 0){
 //Mode UP and DOWN 1 Octaves
arpNoteAmount = 0;
int temp;
//UP AND DOWN 1 oct
for (int i = 0; i < 12; i++) {
  if(scale[i]){
    arpScale[arpNoteAmount] = i;
    arpNoteAmount++;
  }
}
//Attach the DOWN parts
for (int i = 0; i < arpNoteAmount; i++) {
    arpScale[arpNoteAmount + i] = arpScale[arpNoteAmount -2 - i];
}

}
/*Helper loop for debugging
Serial.println(arpNoteAmount);
for (int i = 0; i < 48; i++) {
  Serial.print(arpScale[i]);
  Serial.print(", ");
}
*/
}
