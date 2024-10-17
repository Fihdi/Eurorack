![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/SimpleSampler/SamplesFront.png)

UNTESTED! An improved Sampler module based on the Daisy Seed by Electrosmith.

The sample can be selected via a rotary encoder and the playback speed can be changed via a knob or an external CV. A Gate signal to the RANDOM input will select a random sample.

Depending on the PART/ENTIRE switch, the sample will either play in its entirity or only play while the gate signal is active.

A few notes on the circuit design choices:

Almost everyone uses MCP6002 OpAmps to sum and feed control voltages to microcrontrollers. The reasons for these are:

- safety: The MCP6002 can only output at max 3.3V, which is the maximum what the pins can handle, so the OpAmp will never try to apply a higher voltage than what is safe.
- Easy summing: The summing of control voltages can be done with a single Opamp by adding a negative voltage of around -10V to the external voltage. Since its a summing amplifier with only one stage, the -10 volts will be flipped to a positive voltage on the other side, which is safe for the microcontroller and (again) it cannot go over the safe 3.3V.
-This summing approach does not work so well with a lot of JFET input Opamps (like the TL074) because of the "phase reversal" mode that can happen when the opamps pin is lower than around -8V.

The reason I still went with the TL074 is to safe an IC in the schematic. The safety aspect can be achieved with protection diodes and resistors as well. The audio output of the Daisy Seed also needs to be amplified to regular Eurorack levels, which is not possible with the MCP6002. 
