![VCADSR](https://raw.githubusercontent.com/Fihdi/Eurorack/main/VCADSR/VCADSR-Front.png)

Voltage controlled Attack Decay Sustain Release Module.

Manual Trigger, Loop and x4 Switch. Rates for ADR and the level for Sustain can be controlled manually or by CV with onboard attenuaters
Tested this on breadboard and it works nicely.

Basic schematic inspired by Rene Schmitz. An RS flipflop and analog switches control the charge and discharge paths for the capacitor.
The way I made the circuit voltage controlled is by using current mirrors as VCRs, their job is to limit the current, and thus how fast the capacitor charges (or discharges). 
The "Q" output of the flipflop is responsible for the Attack. Q and the PNP current mirror is the sole path that the capacitor charges up. Since the control current of the mirror is linear, the current to the cap is constant, and the voltage charges linearly instead of logarithmically.

The two NPN current mirrors are used to drop the capacitors voltage, again linearly. One current mirror is used for the decay phase, the other for the release phase.

The PNP transistor before the decay current mirror cuts off the current flow when the voltage reaches Vsus + 500mV. The capacitor is thus held at this voltage and the circuit is in the sustain phase until the input gate dissapears. 

Once the input gate dissapears the current mirror for the release phase discharges the capacitor until 0V.

MOSFETS are used as AND gates to extract the phases of the circuit and indicated with LEDs.

The PCB is the only 4 layer PCB of my modules because the circuit is quiet big (or I am just terrible at routing), either way I would preferr the board(s) to be 2 layer and THT-only.

https://www.schmitzbits.de/adsr.html
