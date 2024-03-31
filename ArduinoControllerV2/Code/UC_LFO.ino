/*LFO Mode
Input voltages are proocessed in exponential fashion.
Triangle LFO with an End of Rise Gate, inverted output and fold capabilites
Also features a reset input that can be used in combination 
with the EOR to turn the triangle into a sawtooth

Inputs:
A...Frequency CV
B...Reset Trigger
C...Fold Threshold
Outputs:
A...Triangle
B...End of Rise Gate
C...A inverted
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

long pwmValue = 0;          // PWM duty cycle value
int deltaPwm = 5;          // PWM duty cycle change per cycle
int triggered = 0;
int fold = 0;
int dir = 1;               // PWM duty cycle direction, 1 for increasing, -1 for decreasing

void setup() {
  //Fast PWM for Channel C
TCCR0A = 0b00000011;
TCCR0B = 0b00000001;

//Fast PWM for Channel B
TCCR1A = 0b00000001; //or 0b00000011;
TCCR1B = 0b00001001;

//Fast PWM for Channel A
TCCR2A = 0b00000011;
TCCR2B = 0b00000001;

pinMode(Acv, OUTPUT);
pinMode(Ain, INPUT);
pinMode(Trg, INPUT);
pinMode(Mode, INPUT);
}

void loop() {

  // calculate exponential mapping of input value to deltaPwm range
  float exponentialValue = pow(2.0, ((float)analogRead(Ain) / 1023.0) * 7.0);
  int mappedDeltaPwm = map(exponentialValue, 1, 128, 1, 40);

  // update PWM duty cycle value
  pwmValue += deltaPwm * dir;

  if (pwmValue > 253) {
    dir = -1;
    analogWrite(Bcv, 255);    
  } else if (pwmValue <= 0) {
    analogWrite(Bcv, 0);
    dir = 1;
  }
  // update deltaPwm based on input value
  deltaPwm = mappedDeltaPwm;
  // output PWM signal
  fold = 254-(analogRead(Cin)/8);
  if(pwmValue > fold){
    analogWrite(Acv, fold-abs(fold-pwmValue));
    analogWrite(Ccv, 255-(int)(fold-abs(fold-pwmValue)));
  }else{
  analogWrite(Acv, (int)pwmValue);
  analogWrite(Ccv, 255-(int)pwmValue);    
  }
  
  if(analogRead(Bin)>50){
    if(!triggered){
      triggered = true;
    pwmValue = 0;
    }
    }
if(analogRead(Bin)<50){
    triggered = false;
}
  
  delay(100);

}