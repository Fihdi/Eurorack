![](https://raw.githubusercontent.com/Fihdi/Eurorack/main/utility/Utility-Front.png)

MISSING POWER and POWER ROUTED WRONG

Inverse VCO: Turns a frequency into a voltage

-No input hysteresis

Slew Rate Limiter:
-I don't know why but it does not slew when I built it, or at the very least it does not slew very noticably. Maybe just a bigger cap is needed.

Attenuverter:
-No issues but some LEDs would be nice, same for the analog XOR

AXOR (Analog XOR)

A differential amplifier followed by a full wave rectifier.

The transfer function of the CV output is: Vout = |(Vin1 - Vin2)| 

When two square waves of equal amplitude are sent into the inputs, the output is (IN1 XOR IN2), this gives a kind of ringmodulator sound.

Sending a single triangle wave into IN1 or IN2 will rectify it and thus double the freuqency.

Sending two LFOs into the inputs produces a really weird (and fun) signal.

AUD output is Vout followed by a highpass to center it around 0V.
