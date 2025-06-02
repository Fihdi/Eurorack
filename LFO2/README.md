![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/LFO2/LFO2_Front.png)

The core oscillator of this circuit is the same as in my TRIVCO module, an OTA-based relaxation oscillator, so this module can be used as a VCO as well.

The triangle output is strictly above 0V while the inverted triangle output is centered to 0V. The triangle output is strictly positive so I can reliably pull it down to 0V with the RESET input.

No more Shape control, only way to get a sawtooth signal is to connect its square-wave output into the RESET input.

The IC used for the STOP feature is quiet expensive, so if you want to omit that, just short pins 1 and 8 on the switch IC.

10/10/2024: I built this module for real but it does not work as well as I had hoped, will have to do some debugging later. Build the original LFO for now.

01.06.2025: Debugged this circuit and tested it with the rest of the synth, works fine now.
