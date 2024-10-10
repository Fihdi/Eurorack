![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/AXOR/AXOR-Front.PNG)

AXOR (Analogue XOR)

Improved stand-alone module of my analogue XOR circuit for CV intermangling, rectification or ring modulation purposes. Incoming signals can be in the audio or in the low frequency range, including DC voltages. These signals are passed through VCAs before going to the AXOR circuit, these VCAs can also be bypassed via switches.

The core of this circuit is a subtractor followed by a full wave rectifier, the transfer function for the CV OUT is thus:



with IN1(t) and IN2(t) being the input amplitudes at a moment of time.

Example truth table: IN1 and IN2 are square wave with a range of 0V - 5V:

