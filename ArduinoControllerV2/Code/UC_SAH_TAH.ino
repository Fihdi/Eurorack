/*Sample and Hold / Track and Hold
Inputs:
A...Input to be sampled
B...Trigger
C...Mode

Outputs:
A...Sample and hold / Track and hold output
B...Same as input
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

int mode = 0; //0 for SAH, else for TAH

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
bool triggered = false;

void loop() {
  mode = analogRead(Cin)>50?1:0;
if(mode!=0){
  //Track and hold
if(analogRead(Bin)>50){ 
  analogWrite(Acv,analogRead(Ain)/4);      
}
}else{ 
  //Sample and hold
  if(analogRead(Bin)>50){
    analogWrite(Bcv, 255);
    if(!triggered){
      triggered = true;
    analogWrite(Acv, analogRead(Ain)/4);
    }   
  }
}

if(analogRead(Bin)<50){
    triggered = false;
    analogWrite(Bcv, 0);
}

}