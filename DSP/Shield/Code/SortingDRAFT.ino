//This program sorts an array of 20 elements. The numbers in the array represent the musical notes in Hz, so the program can be seen as sorting musical notes.
//Using an external clock forwards the sorting algorithm.
//The currently sorted note is played using the tone() function.
//Outputs
#define CLKO 2  //Internal Clock Output
int counter = 0;
int array[] = {110,123,146,293,164,174,196,220,246,261,329,349,392,440,494,523,587,659,698,783};
int length = 20;
int current = 0;

//Interrupt flags 
volatile bool CLKtriggerInterrupted = false;

//Interrupt conditions
volatile bool CLKtriggered = false;

const int minDelay = 20;    // Minimum delay in milliseconds (sets the maximum frequency)
const int maxDelay = 2000;  // Maximum delay in milliseconds (sets the minimum frequency)

bool rhythm1[16];  // Maximum length of 16
bool rhythm2[16];  // Maximum length of 16

struct Clock {
  unsigned long previousMillis;  // Variable to store the current state and the last toggle time for each clock
  int delayTime;
  int state;
};

Clock internal;

void setup() {
  pinMode(CLKO, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
  shuffle();
  internal.delayTime = 50;
}

void loop() {

  updateInternalClock();


  //Inputs have pullup resistors instead of pulldown, thus the trigger conditioning is inverted from e.g.: my Sequencer.
}

void updateInternalClock() {

  unsigned long currentMillis = millis();

// internal.delayTime = map(analogRead(CLKI_RATE), 0, 1023, minDelay, maxDelay);  // Set the frequency of the internal

  //Write internal Clock
  if (currentMillis - internal.previousMillis >= internal.delayTime) {
    internal.previousMillis = currentMillis;
    internal.state = !internal.state;
    digitalWrite(LED_BUILTIN, internal.state);
    //The state changed, if state is HIGH now that means the clock just had a rising Edge, update the counter.
    if(internal.state == HIGH){
      gnome();
        tone(CLKO, array[current]/2);
    }else{
      //Uncomment to silence the  
     //   tone(CLKO, 0);
    } 
  }
}


void shuffle() {
    int n = sizeof(array) / sizeof(array[0]);
  
    for (int i = n - 1; i > 0; --i) {
        // Generate random index between 0 and i
        int j = random(i);
        
        // Swap elements
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}
void gnome(){

       if (current == 0 || array[current] >= array[current-1]){
           current = current + 1;
       }else{
           //Swap
          int temp = array[current];
           array[current] = array[current-1];
           array[current-1] = temp;

           current = current - 1;
       }
}
