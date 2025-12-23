#define CLK 4 //External Clock Input
#define VCO 3 //Feedback from the VCO
#define PWM 6 //Control voltage output to adjust the VCO
#define DIV 5 //Division factor button
#define CV  A1 //External CV when the delay time is not set via the clock (freerunning)
#define MOSFET 2 //MOSFET to change RC constant (for faster response when freerunning)

#define LED1 7
#define LED2 8
#define LED4 9
#define LED8 10

//Delaytime and corresponding frequencies (divided by 2048) from the datasheet
double delaytime[28] =  {32.6, 34.4, 36.6, 38.5, 40.6, 43,    45.8, 48.1, 52.3, 56.6, 61.6, 68.1,  75.9, 81, 86.3, 92.2, 97.1, 104.3, 113.7, 124.1, 136.6, 151, 171, 196,   228, 273, 342, 500};
double hertz[28] =      {10254, 9766, 9277, 8789, 8301, 7813, 7324, 6836, 6348, 5859, 5371, 4883,  4395, 4150, 3906, 3662, 3418, 3174, 2930, 2686, 2441, 2197, 1953, 1709, 1465, 1221, 977, 635};

volatile unsigned int VCOtarget = 3000; //Target frequency that has to be measured on the VCO pin. (example: 3000)
volatile float VCOFrequency = 0.0; //measured frequency of the PT2399, compare this to VCOtarget
unsigned int controlVoltage = 64.0; // controls the VCO

unsigned int division = 1; //Division Factor
/*
1 = div by 1, 
2 = div by 2, 
3 = div by 4, 
4 = div by 8, 
5 = mult by 2, 
6 = mult by 4, 
7 = mult by 8, 
0 = NO DIVISION(CV control)
*/

bool DIVinterrupted = false;
bool DIVtriggered = false;

volatile unsigned long lastVcoTime = 0; //Last time a rising edge of the VCO occured
volatile unsigned long vcoPeriod = 0;   //PT2399 VCO period in microseconds
volatile bool VCOinterrupted = false;
volatile bool VCOtriggered = false;

bool CLKinterrupted = false;
bool CLKtriggered = false;

volatile unsigned long lastCLKtime = 0;
volatile unsigned long CLKperiod = 0;   //CLK period in milliseconds.

unsigned long lastPrint;

void setup() {

  Serial.begin(9600);
  
  //Fast PWM, speeds up the internal timers by a factor 64, hence the need to multiply/divide the millis and delay values.  
  TCCR0A = 0b00000011;
  TCCR0B = 0b00000001;
  TCCR1A = 0b00000001;
  TCCR1B = 0b00001001;
  TCCR2A = 0b00000011;
  TCCR2B = 0b00000001;

  pinMode(CLK, INPUT);
  pinMode(VCO, INPUT_PULLUP);
  pinMode(DIV, INPUT_PULLUP);
  pinMode(PWM, OUTPUT);
  pinMode(CV, INPUT);
  pinMode(MOSFET, OUTPUT);

  digitalWrite(MOSFET, HIGH);
  analogWrite(PWM, 64); //initial value 

  delay(400);
  outputDivisionLEDs();

}

void loop() {

  checkDIV();
  checkCLK();
  checkVCO();
  static unsigned long lastUpdate = 0;

  // Update control loop at ~1kHz AND when division is not zero.
  // When division is zero, the delaytime can be freely set by CV, no need for a control loop.
  if ((micros() - lastUpdate > 64000L ) && division!=0) {
    
    lastUpdate = micros();
    if (vcoPeriod > 0) {
      VCOFrequency = 1000000.0 / vcoPeriod;
      VCOFrequency = VCOFrequency*64L;

    if(VCOtarget > VCOFrequency){
      controlVoltage = constrain((controlVoltage+1), 0, 90);
    }
    if(VCOtarget < VCOFrequency){
      controlVoltage = constrain((controlVoltage-1), 1, 90);
    }
      analogWrite(PWM, controlVoltage);
    }

  }

  if(division==0){
    analogWrite(PWM, map(analogRead(CV),0,1023,1,90)); //Write the incoming TimeCV value to the output (within 1 to 90 range)
  }

if (millis() - lastPrint > 64000L) {
  //Update BPM if CLK has been measured AND the BPM has changed
  if(CLKperiod > 0){
      switch(division){
      case 1: 
      VCOtarget = interpolate(CLKperiod); //Set the VCOtarget for the PT2399 as it is
      break;
      case 2:   //Divide by 2
      VCOtarget = interpolate(CLKperiod*2); //Set the VCOtarget for the PT2399
      break;
      case 3:   //Divide by 4
      VCOtarget = interpolate(CLKperiod*4); //Set the VCOtarget for the PT2399
      break;
      case 4:   //Divide by 8
      VCOtarget = interpolate(CLKperiod*8); //Set the VCOtarget for the PT2399
      break;
      case 5:   //mult by 2
      VCOtarget = interpolate(CLKperiod/2); //Set the VCOtarget for the PT2399
      break;
      case 6:   //mult by 4
      VCOtarget = interpolate(CLKperiod/4); //Set the VCOtarget for the PT2399
      break;
      case 7:   //mult by 8
      VCOtarget = interpolate(CLKperiod/8); //Set the VCOtarget for the PT2399
      break;
      case 0:   
//CV control
      break;
      }
  }
    lastPrint = millis();
    printDebug();
   //Serial.println(constrain(analogRead(CV)/8,1,90));
}
}
void adjustCV(){

}
void printDebug(){
    Serial.print(" Division Factor: ");
    Serial.print(division);
    Serial.print(" VCOtarget PT2399 : ");
    Serial.print(VCOtarget);
    Serial.print(" Pt2399 Measured: ");
    Serial.print(VCOFrequency);
    Serial.print(" CV: ");
    Serial.print(controlVoltage);
    Serial.print(" DelayTime CLK: ");

    Serial.println(CLKperiod);
}

float interpolate(int period) {
  //Translates a given delaytime to the expected frequency
  //period in ms!
  //Search for lower and upper bound indexes
  int lowindex = 0;
  int highindex = 0;

  for(int i = 0; i<28; i++){
    if(period < delaytime[i]){
      lowindex = i;
      highindex = i-1;
      break;
    }
  }
  //Setup for the points for linear interpolation
  int x1 = delaytime[lowindex];
  int y1 = hertz[lowindex];
  int x2 = delaytime[highindex];
  int y2 = hertz[highindex];


  //Interpolate
    if (x2 - x1 == 0) {
    return hertz[lowindex]; //Prevent divide by 0
  }

  //  float y = y1 + ((x - x1) * (y2 - y1)) / (x2 - x1);
  // Perform the linear interpolation
  return y1 + ((period - x1) * (y2 - y1)) / (x2 - x1);
 
}


void checkVCO(){

  if((digitalRead(VCO) == LOW) && (VCOinterrupted == false)){
VCOinterrupted = true;
//VCO ISR
 vcoPeriod = micros() - lastVcoTime;
  lastVcoTime = micros();
}

if ((digitalRead(VCO) == HIGH) && (VCOinterrupted == true)) {
    VCOinterrupted = false;  //Reset flag
  }
}

void checkCLK() {
	CLKtriggered = (digitalRead(CLK) == LOW) && (CLKinterrupted == false);
	
if(CLKtriggered){
CLKinterrupted = true;
//External Clock ISR
  CLKperiod = millis()/64L - lastCLKtime;
  lastCLKtime = millis()/64L;
}

if ((digitalRead(CLK) == HIGH) && (CLKinterrupted == true)) {
    CLKinterrupted = false;  //Reset flag
  }
}

void checkDIV() {
DIVtriggered = (digitalRead(DIV) == LOW) && (DIVinterrupted == false);

if(DIVtriggered){
DIVinterrupted = true;
//Division Button ISR
division = (division+1)%8;
outputDivisionLEDs();

if(division==0){
digitalWrite(MOSFET, LOW); //In CV mode, switch the MOSFET and the large capacitor OFF for faster response.
 }else{
  digitalWrite(MOSFET, HIGH); //Switch the MOSFET and large capacitor ON for slow response, to prevent over and undershooting in the control loop.
 }

}

if ((digitalRead(DIV) == HIGH) && (DIVinterrupted == true)) {
    DIVinterrupted = false;  //Reset trigger flag
  }
}

void outputDivisionLEDs(){
switch(division){
      case 1: 
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED8, LOW);
      break;
      case 2:   //Divide by 2
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED8, LOW);
      break;
      case 3:   //Divide by 4
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED8, LOW);
      break;
      case 4:   //Divide by 8
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED8, HIGH);
      break;
      case 5:   //mult by 2
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED8, HIGH);
      break;
      case 6:   //mult by 4
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED8, HIGH);
      break;
      case 7:   //mult by 8
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED8, LOW);
      break;
      case 0:
       digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED8, LOW);
//CV control
      break;
      }

}