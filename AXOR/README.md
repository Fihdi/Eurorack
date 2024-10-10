![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/AXOR/AXOR-Front.PNG)

AXOR (Analogue XOR)

Improved stand-alone module of my analogue XOR circuit for CV intermangling, rectification or ring modulation purposes. XORing two square wave signals together results in an ringmodulation-like effect.

Incoming signals can be in the audio or in the low frequency range, including DC voltages. The signals are passed through VCAs before going to the AXOR circuit, these VCAs can also be bypassed via switches.

The core of this circuit is a subtractor followed by a full wave rectifier, the transfer function G for the CV OUT output is thus:

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/AXOR/Formula.PNG)

Example: IN1 and IN2 are square wave in the range of 0V - 5V:

![](https://github.com/Fihdi/Eurorack/blob/main/AXOR/TTable.png)

The resulting truth table is the same as that of an XOR gate.
