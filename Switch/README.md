![Switch](https://raw.githubusercontent.com/Fihdi/Eurorack/main/Switch/Switch-Front.png)

Two Voltage Controlled Switches with analog logic outputs. 

Issue: The comparators for A>B and B>A should compare the inputs before the switches, not after.

Circuit based on Rene Schmitz, they made it possible to switch positive and negative Voltages through a CD4066 chip, which should only operate at 0V to Vcc.

The working principle is: The analog switches, when closed, act as a short circuits inbetween the two resistors of an inverting amplifier opamp circuit,
which sits at 0V since the non-inverting input is connected to ground. Because there is close to 0V across a short circuit and one end of the switch is always connected to 0V, 
there will be a total of 0V across the switch when it is closed, but still relays the current in both directions to complete the non-inverting amplifier.

When the switch is open, two diodes clamp the voltage from going above approx. +/- 400mV which the CD4066 can handle. 

The only reason I made this circuit this way instead of just buying analog switch ICs form Vishay that can handle up to +/- 15V without the need for black magic is because I haove 70 4066 chips that were socketed in old servers.

The SAH-TAH Module can also be made into a signal switch by replacing the capacitor with a resistor in the sample and hold circuit. This could be further simplified by removing the input and output buffers and the trigger circuit entirely, leaving only a JFET and a few pull down reistors.

https://www.schmitzbits.de/switch.html
