![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/DoubleDelay/DelayFront.png)

Simple delay module with two identical PT2399 circuits either in series or separate. Parameters for Level, Delay Time, and Feedback amount can be set with knobs.
This module features no CV.

Ideas for a new module:

I was thinking about a "SingleDelay" module (in contrast to the "DoubleDelay") where
the three parameters for level, time and feedback can be voltage controlled.

For Level CV, the PT2399 output is fed back through a VCA.
For Time CV, a 470 to 1k resistor with an NPN transistor for biasing can be used.
For Feedback CV, like with level, the PT2399 output is fed back through a separate VCA back to the Input summing amplifier for the PT2399 input.

Another feature idea would be to only let the delayed sound through. 
This can be achieved by adding an attenuation knob for the input signal on the output summing stage. 
Instead of a knob a switch could be used to bypass the signal instead of attenuation.
