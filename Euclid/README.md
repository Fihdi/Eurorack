![](https://raw.githubusercontent.com/Fihdi/Eurorack/main/Euclid/Euclid_Front.png)

2 Channel Euclidean Rhythm Generator.

Has an internal clock generator which can be used as a master clock. This clock will be overwritten if an external signal on CLK IN is detected. If no signal is detected at CLK IN, the internal clock is used again. 

Channel parameters such as length and steps can be changed via pots and shifted via the SHIFT input.

A rising edge at RESET resets the channels assignments.

Upload the file "Bernoulli-Gates.ino" to repurpose the module as a "coin toss" module similar to Mutable Instruments Branches. Send a Gate or Trigger into the CLK IN input and the signal will be routed to either OUT1 or OUT2 depending on a probability. The probability is set by the rate knob.
