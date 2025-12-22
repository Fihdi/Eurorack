# 10hp Delay Module 

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/SimpleDelaySMD/SimpleDelayIRL.jpeg)

This module is based on the famous PT2399 delay IC and uses an Arduino Nano to control its delay time. The Arduino controls the delay time by RC-filtering its PWM output and sending that signal to an NPN current sink. For the feedback, the VCO output of the PT2399 goes to a high speed binary counter, divided by 2048 and fed back to the Arduino Nano for evaluation.

If the VCO frequency is higher than the target frequency, the duty cycle of the PWM output is decreased, and increased if the VCO frequency is lower than the target.

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/SimpleDelaySMD/TimesFrequencyTable.png)

To evaluate the target frequency is by first measuring the time of the Clock signal and then interpolating the delay time from the delay time / frequency table. The delay time for interpolation is multiplied or divided based on the division factor, which is set by the DIV button and shown by the LEDs next to it.

It is also possible to set the delay time freely by hand and CV instead of an external Clock signal.

Feedback can be controlled by CV or by hand. For this the WET signal is fed back to the input mixer through a VCA.

The final output is set by a crossfader between the original input and the WET signal.
