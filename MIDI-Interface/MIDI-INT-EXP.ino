#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
Encoder myEnc(5, 6);

int oldPosition1 = -999;
int newPosition1 = -999;
bool enc_switch_in = 0;
bool oldEncSwitch = 0;
byte scroll = 0;

#define NUM_OUTPUTS 8
#define ENCSW 8

#define CV1 11
#define CV2 10
#define CV3 9
#define CV4 3

#define GT1 7
#define GT2 2
#define GT3 A1
#define GT4 A2

byte outputPins[NUM_OUTPUTS] = { GT1, GT2, GT3, GT4, CV1, CV2, CV3, CV4 };
boolean outputStates[NUM_OUTPUTS] = { false };  // Initialize array of boolean variables for each output // false = gate, true = trig
unsigned long trigTimerCLK = 0;
unsigned long trigTimers[NUM_OUTPUTS] = { 0 };

void setup() {

  //Increase PWM Frequency
  TCCR0A = 0b00000011;
  TCCR0B = 0b00000001;
  TCCR1A = 0b00000001;
  TCCR1B = 0b00001001;
  TCCR2A = 0b00000011;
  TCCR2B = 0b00000001;

  // Initiate MIDI communications, listen to channel 1
  MIDI.begin(1);

  //       Connect the handleNoteOn function to the library,
  //     so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

  // Do the same for NoteOffs
  MIDI.setHandleNoteOff(handleNoteOff);

  MIDI.setHandleControlChange(handleControlChange);

  pinMode(ENCSW, INPUT_PULLUP);  //Encoder Button

  for (int i = 0; i < NUM_OUTPUTS; i++) {  //SET TO OUTPUTS AND LOW
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], LOW);
  }
}

void loop() {

  hardwareCheck();
  midiCheck();
}

void flashLED(int pin, int times) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(100);
    digitalWrite(pin, LOW);
    delay(100);
  }

  digitalWrite(outputPins[scroll - 1], LOW);
  digitalWrite(outputPins[scroll], HIGH);
}

void hardwareCheck() {
  oldEncSwitch = enc_switch_in;
  enc_switch_in = !digitalRead(ENCSW);
  newPosition1 = myEnc.read();
  
  if ((newPosition1 + 2) / 2 < oldPosition1 / 2) {
    oldPosition1 = newPosition1;
    scroll = scroll + 1;
    digitalWrite(outputPins[scroll - 1], LOW);
    digitalWrite(outputPins[scroll], HIGH);
  }
  else if ((newPosition1 - 2) / 2 > oldPosition1 / 2) {
    oldPosition1 = newPosition1;
    scroll = scroll - 1;

    digitalWrite(outputPins[scroll + 1], LOW);
    digitalWrite(outputPins[scroll], HIGH);
  }

  if (scroll >= NUM_OUTPUTS) {
    scroll = 0;
  }
  else if (scroll < 0) {
    scroll = NUM_OUTPUTS - 1;
  }
  
  if (oldEncSwitch == 0 && enc_switch_in == 1) {
    outputStates[scroll] = !outputStates[scroll];
    
    if (outputStates[scroll] == false) {  //FLASH ONCE FOR GATE
      flashLED(outputPins[scroll], 1);
    }
    else {  //FLASH TWICE FOR TRIG
      flashLED(outputPins[scroll], 2);
    }
  }
}

void midiCheck() {
  MIDI.read();
  MIDI.setInputChannel(1);
  
  for (int i = 0; i < NUM_OUTPUTS; i++) {
    if ((trigTimers[i] > 0) && (millis() - trigTimers[i] > 50)) {
      digitalWrite(outputPins[i], LOW);  // Set trigger low after 50 msec
      trigTimers[i] = 0;
    }
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  // Do whatever you want when a note is pressed.
  // Try to keep your callbacks short (no delays ect)
  // otherwise it would slow down the loop() and have a bad impact
  // on real-time performance.

  switch (pitch) {

    case 9:
      if (outputStates[0] == false) {
        digitalWrite(outputPins[0], HIGH);
      }

      else {
        digitalWrite(outputPins[0], HIGH);
        trigTimers[0] = millis();
      }
      break;


    case 10:
      if (outputStates[1] == false) {
        digitalWrite(outputPins[1], HIGH);
      }

      else {
        digitalWrite(outputPins[1], HIGH);
        trigTimers[1] = millis();
      }
      break;


    case 11:
      if (outputStates[2] == false) {
        digitalWrite(outputPins[2], HIGH);
      }

      else {
        digitalWrite(outputPins[2], HIGH);
        trigTimers[2] = millis();
      }
      break;


    case 12:
      if (outputStates[3] == false) {
        digitalWrite(outputPins[3], HIGH);
      }

      else {
        digitalWrite(outputPins[3], HIGH);
        trigTimers[3] = millis();
      }
      break;


    case 13:
      if (outputStates[4] == false) {
        digitalWrite(outputPins[4], HIGH);
      }

      else {
        digitalWrite(outputPins[4], HIGH);
        trigTimers[4] = millis();
      }
      break;


    case 14:
      if (outputStates[5] == false) {
        digitalWrite(outputPins[5], HIGH);
      }

      else {
        digitalWrite(outputPins[5], HIGH);
        trigTimers[5] = millis();
      }
      break;


    case 15:
      if (outputStates[6] == false) {
        digitalWrite(outputPins[6], HIGH);
      }

      else {
        digitalWrite(outputPins[6], HIGH);
        trigTimers[6] = millis();
      }
      break;

    
    case 16:
      if (outputStates[7] == false) {
        digitalWrite(outputPins[7], HIGH);
      }
      break;
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  // Do something when the note is released.
  // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.

  switch (pitch) {

    case 9:
      if (outputStates[0] == false) {
        digitalWrite(outputPins[0], LOW);
      }
      break;

    case 10:
      if (outputStates[1] == false) {
        digitalWrite(outputPins[1], LOW);
      }
      break;

    case 11:
      if (outputStates[2] == false) {
        digitalWrite(outputPins[2], LOW);
      }
      break;

    case 12:
      if (outputStates[3] == false) {
        digitalWrite(outputPins[3], LOW);
      }
      break;

    case 13:
      if (outputStates[4] == false) {
        digitalWrite(outputPins[4], LOW);
      }
      break;

    case 14:
      if (outputStates[5] == false) {
        digitalWrite(outputPins[5], LOW);
      }
      break;

    case 15:
      if (outputStates[6] == false) {
        digitalWrite(outputPins[6], LOW);
      }
      break;


    case 16:
      if (outputStates[7] == false) {
        digitalWrite(outputPins[7], LOW);
      }
      break;
  }
}

void handleControlChange(byte channel, byte number, byte value) {

  if (number == 24) {
    analogWrite(CV4, value);
  }

  else if (number == 25) {
    analogWrite(CV1, value);
  }

  else if (number == 26) {
    analogWrite(CV2, value);
  }

  else if (number == 27) {
    analogWrite(CV3, value);
  }
}
