![](https://raw.githubusercontent.com/Fihdi/Eurorack/main/Euclid/Euclid_Front.png)

2 Channel Euclidean Rhythm Generator.

Has an internal clock generator which can be used as a master clock. This clock will be overwritten if an external signal on "CLK IN" is detected. If no signal is detected at "CLK IN", the internal clock is used again. 

Channel paramters such as length and steps can be changed via pots and shifted via the SHIFT input.

A rising edge at RESET resets the channels assignments.
