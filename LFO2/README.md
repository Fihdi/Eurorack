# Voltage controllable LFO 

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/LFO2/LFO2_Front.png)

The core oscillator of this circuit is the same as in my TRIVCO module, an OTA-based relaxation oscillator, so this module can be used as a VCO as well.

The triangle output is strictly above 0V while the inverted triangle output is centered to 0V. The triangle output is strictly positive so I can reliably pull it down to 0V with the RESET input.

No more shape control, only way to get a sawtooth signal is to connect its square-wave output into the RESET input.
