# 10hp Delay Module 

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/SimpleDelaySMD/SimpleDelayIRL.jpeg)

This module is based on the famous PT2399 delay IC and uses an Arduino Nano to control its delay time with a frequency control system. The Arduino controls the delay time with an NPN current sink connected to pin 6 of the PT2399. More current flowing out of pin 6 results in a smaller delay time and a higher VCO frequency. For the control system, the VCO output of the PT2399 goes to a high speed frequency divider(divided by 2048) and fed back to the Arduino Nano for evaluation.

If the VCO frequency is higher than the target frequency, the current is decreased, and increased if the VCO frequency is lower than the target. To evaluate the target frequency, the time of the external clock signal is measured and then interpolated from the delay time / frequency table. The delay time for interpolation is multiplied or divided based on the division factor, which is set by the DIV button and shown by the LEDs next to it.

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/SimpleDelaySMD/TimesFrequencyTable.png)

It is also possible to set the delay time freely by hand and CV instead of an external Clock signal.

Audio feedback (the amount of repeats) can be controlled by CV or by hand. For this the WET signal is fed back to the input mixer through a VCA.

The final output is set by a crossfader between the original input and the WET signal.
