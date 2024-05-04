/*Attack-Release mode

Inputs:
A ...CV

Outputs:
A...VCO
B...Sub Oscillator
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

#define Trg 12
#define Mode 13

void setup() {  
Serial.begin(9600);

pinMode(Acv, OUTPUT);
pinMode(Bcv, OUTPUT);
pinMode(Ccv, OUTPUT);

pinMode(Aaud, OUTPUT);
pinMode(Baud, OUTPUT);
pinMode(Caud, OUTPUT);

pinMode(Ain, INPUT);
pinMode(Bin, INPUT);
pinMode(Cin, INPUT);

pinMode(Trg, INPUT);
pinMode(Mode, INPUT);
//Fast PWM for Channel C
TCCR0A = 0b00000011;
TCCR0B = 0b00000001;

//Fast PWM for Channel B
TCCR1A = 0b00000001; //or 0b00000011;
TCCR1B = 0b00001001;

//Fast PWM for Channel A
TCCR2A = 0b00000011;
TCCR2B = 0b00000001;

}

//Exponential Conversion from CV to frequency:
double frequency = 61;
int base = 0;
void loop() {
  if(analogRead(Bin)>7){
   // base = 4*pow(2.0,(double)((-1/2)*(pow(analogRead(Bin)/200)-4,2)));
  //  frequency = 61.735*pow(base, analogRead(Ain)/100);
  }else{
    frequency = 61.735*pow(2, (double)analogRead(Ain)/70); 
  }
  
  tone(Aaud, (int)frequency/8);
  tone(Baud, (int)frequency/4);  
}
