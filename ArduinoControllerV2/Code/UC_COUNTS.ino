/*Voltage Controlled Slew / Voltage follower
Something close to either channel 1 or channel 4 of the Make Noise Maths Module

But we are not doing Maths, we are just counting... so COUNTS it is.

Inputs:
A...Rise CV
B...Input
C...Fall CV

Outputs:
A...Out
B...Rising
C...Below
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
bool triggered = false;
float exponentialValueR; 
int mappedDeltaPwmR; 
float exponentialValueF;
int mappedDeltaPwmF;
int in;
int output;

int c;

void loop() {
in = analogRead(Bin)/4;
  exponentialValueR = pow(2.0, ((float)analogRead(Ain) / 1023.0) * 7.0);
mappedDeltaPwmR = map(exponentialValueR, 1, 128, 1, 40);
 exponentialValueF = pow(2.0, ((float)analogRead(Cin) / 1023.0) * 7.0);
mappedDeltaPwmF = map(exponentialValueF, 1, 128, 1, 40);
analogWrite(Ccv, 255);
c=0;
while(output < in){
//Rising

in = analogRead(Bin)/4;

if(in <= output){
  analogWrite(Bcv, 0);
  break;
}  
  analogWrite(Bcv, 255);
  
  //Increase the value of the output until it is equal to the input
  exponentialValueR = pow(2.0, ((float)analogRead(Ain) / 1023.0) * 7.0);
mappedDeltaPwmR = map(exponentialValueR, 1, 128, 1, 40);
 exponentialValueF = pow(2.0, ((float)analogRead(Cin) / 1023.0) * 7.0);
mappedDeltaPwmF = map(exponentialValueF, 1, 128, 1, 40);
//increase the voltage if it does not lead to an overshoot on the next loop 

if(c%10==0){
if((output+mappedDeltaPwmR)<in){
  output+=mappedDeltaPwmR;
  analogWrite(Acv, output);
}else{
  output = in; // otherwise set the output equal to the input
  analogWrite(Acv, output);
  break;
}
}
c=(c+1)%1024;
delay(50);
}

c=0;
exponentialValueR = pow(2.0, ((float)analogRead(Ain) / 1023.0) * 7.0);
mappedDeltaPwmR = map(exponentialValueR, 1, 128, 1, 40);
 exponentialValueF = pow(2.0, ((float)analogRead(Cin) / 1023.0) * 7.0);
mappedDeltaPwmF = map(exponentialValueF, 1, 128, 1, 40);
analogWrite(Ccv, 0);
while(output > in){
 //Falling

in = analogRead(Bin)/4;
analogWrite(Bcv, 0);

  if(in > output){
    analogWrite(Bcv, 255);
  break;
}
 
  //decrease the value of the output until it is equal to the input
  exponentialValueR = pow(2.0, ((float)analogRead(Ain) / 1023.0) * 7.0);
mappedDeltaPwmR = map(exponentialValueR, 1, 128, 1, 40);
 exponentialValueF = pow(2.0, ((float)analogRead(Cin) / 1023.0) * 7.0);
mappedDeltaPwmF = map(exponentialValueF, 1, 128, 1, 40);
if(c%10==0){
//only decrease the voltage if it does not lead to an undershoot on the next decrease 
if((output-mappedDeltaPwmF)>in){
  output-=mappedDeltaPwmF;
  analogWrite(Acv, output);
}else{
  output = in;
analogWrite(Acv, output);  
  break; // the output is close to the output, so the loop breaks.
}
}
c=(c+1)%1024;
delay(50);
}

}