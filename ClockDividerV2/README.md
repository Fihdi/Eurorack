![](https://raw.githubusercontent.com/Fihdi/Eurorack/main/ClockDividerV2/CD_Front.png)

An Improved Clockdivider module based on the ATMEGA328P microcontroller instead of the CD4040 binary counter IC. Instead of simply counting in binary, this module can be switched to count in binary, prime numbers and harmonically. I am also thinking about making a fourth mode: Random mode, where at each trigger, the outputs have different probabilities to turning on or off. It's also possible to analyze the incoming clock frequency and output a multiple of the input frequencies to the outputs.


A button press or a rising edge on the input "CLK" will increase the internal counter.

A button press or a rising edge on the input "MODE" will switch to another mode.

A button press or a rising edge on the input "RESET" resets the internal counter of the output states, it does not reset the mode.
