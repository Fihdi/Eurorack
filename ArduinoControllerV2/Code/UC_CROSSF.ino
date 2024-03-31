/*Dual VCA Crossfader
A crossfader can be used to fade between two signals.
The fading between the the signals on INPUT A and C are done by INPUT B, which acts as the CV input.

The principle behind the code is the same as feeding both signals into seperate VCAs.
Using a CV and an inverted version of that CV on those two VCAs, and summing their outputs, is how
voltage controlled crossfaders were done in analog synthesizers.
The code was written in a way to reflect that.

Inputs:
A...Signal 1
B...CV
C... Signal 2

Outputs:
A...VCA1
B...Crossfader output
B...VCA2 output
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

  double cvAmount = ((double)analogRead(A2))/1024.0; //CHANGE TO KOMMASTELLENZAHLEN NICHT GANZZAHLEN OMG
  double cvInv = 0.8-cvAmount;
  
  double a = analogRead(A1)/4.0;
double c = analogRead(A3)/4.0;

  Serial.println((cvAmount*((double)analogRead(A1)/4.0)));
    analogWrite(Acv,(cvAmount*((double)analogRead(A1)/4.0))); // VCA1 = CV*Signal1
    analogWrite(Ccv,(cvInv*((double)analogRead(A3)/4.0))); //VCA2 = (1-CV)*Signal2

    
  analogWrite(Bcv, (cvInv*((double)analogRead(A3)/4.0)) + (cvAmount*((double)analogRead(A1)/4.0))); //Crossfader output = CV*Signal1+(1-CV)*Signal2
  
}