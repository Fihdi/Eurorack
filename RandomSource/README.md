![](https://raw.githubusercontent.com/Fihdi/Eurorack/main/RandomSource/Random_Front.png)

Improved Version of SAH-TAH, contains a noise generator, sample and hold, burst, slew and bounce circuit. Signal routing is done via manual switches.  

At its core SAH-TAH has two core components: a noise generator and sample/track and hold. Random also has the same core, but with two extra features, a comparator and a slewing circuit.

SAH/TAH changes the SAH output from sample and hold to track and hold, the switch bypasses the gate to trigger converter and feeds a 10V signal to the JFET and diode, opening it and tracking the signal from IN or NOISE while TRG is held high. When TRG falls low, the last value of IN or NOISE is held at the SAH output.

The comparator has a reference voltage that can be set with the REF knob. The input to the comparator can be set as NOISE or SAH. When the input signal to the comparator is less than the reference voltage, GATES will output 10V. This can be used to output random gates. BURST doesnt seems to work.

SLEW increases the time it takes for the SLEW OUT output to catch up to the SAH output by varying the RC time constant. The BOUNCE knob adds a delay to the feedback signal with a second RC circuit, causing the SAH SLEW output to over- and undershoot the sampled valued. The MODE switch can select between the first and second RC circuit. This can be used to output smooth random or chaotic random voltages when set to IN/NOISE switch is set to NOISE.
