#define Acv 11
#define Aaud 10

const int melody[] PROGMEM = {1000, 8, 760, 8, 2000, 8, 1200, 16, 200, 8, 500, 16, 50, 8, 200, 8};
int tempo = 108;
int mode = 0; //0 for CV, else for audio

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
   playSequence();
}
void playSequence(){
for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = pgm_read_word_near(melody+thisNote + 1);
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    if(mode!=0){
    tone(Aaud, pgm_read_word_near(melody+thisNote), noteDuration * 0.9);
    delay(noteDuration);
    noTone(Aaud);
    }else{
      analogWrite(Acv, pgm_read_word_near(melody+thisNote)/100);
      delay(noteDuration);
    }    
    
  }  
}