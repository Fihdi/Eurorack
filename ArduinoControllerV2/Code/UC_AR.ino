/*Attack-Release mode

Inputs:
A ...Attack CV
B ...Trigger
C ...Release CV

Outputs:
Acv...A/R Output
Bcv...Random output voltage when the end of the A/R cycle
Ccv...End of cycle gate output
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
int deltaPwm  = 5;        // PWM duty cycle change when Releasing
int dir = 1;                // PWM duty cycle direction, 1 for increasing, -1 for decreasing

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

int t = 0;
void loop() {
  // calculate exponential mapping of input value to deltaPwm range
  float exponentialValueA = pow(2.0, ((float)analogRead(Ain) / 1023.0) * 7.0);
  int mappedDeltaPwmA = map(exponentialValueA, 1, 128, 1, 40);

  float exponentialValueR = pow(2.0, ((float)analogRead(Cin) / 1023.0) * 7.0);
  int mappedDeltaPwmR = map(exponentialValueR, 1, 128, 1, 40);
  // update PWM duty cycle value
  //mappedDeltaPwmR = 3;
  //mappedDeltaPwmA = 3;
  
 pwmValue += deltaPwm * dir;

  if (pwmValue > 254) {
    dir = -1;
    analogWrite(Ccv, 0); 
  } else if (pwmValue <= 0) {
    dir = 1;  
    analogWrite(Bcv, rand()*255);
    analogWrite(Ccv, 255);        
  }
  
  // update deltaPwm based on input value
  if(dir==-1){
    deltaPwm = mappedDeltaPwmA;
  }else{
deltaPwm = mappedDeltaPwmR;    
  }
  // update deltaPwm based on input value
  // output PWM signal
  analogWrite(Acv, (int)pwmValue);
  
  t=(t+1)%20000;
  delay(100);
}