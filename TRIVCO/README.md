![TRIVCO](https://raw.githubusercontent.com/Fihdi/Eurorack/main/TRIVCO/VCO-Front.png)

It is a beefed up relaxation oscillator using an inverting schmitt trigger. See: https://en.wikipedia.org/wiki/Relaxation_oscillator. The charging resistor has been replaced by an LM13700 trans-conductance amplifier (OTA), acting as a CCCS. The control voltages are summed through two inverting amplifiers with unity gain and sent to an exponential current converter that controls the OTA's output current (And thus the frequency). Tuning the exponential current converter is crucial to get proper Volt per Octave tracking.

The capacitor voltage is buffered and DC offset is removed (triangle/saw output), that voltage is compared to either ground or to an external voltage leveled to approx. 5V and buffered once again (pwm output).

The SYNCH input features a schmitt trigger input with small hysteresis (~100mV) to prevent spontaneous syncing from noise. For signals substantialy bigger than 100mV this schmitt trigger acts as a zero-crossing comparator; Whose output is fed to the schmitt trigger junction of the oscillator, achieving synchronisation by averaging the two schmitt triggers outputs together.

Minor issues:
-No trimpots for 1V/Oct tracking

-Triangle signal after the buffer is shifted up/down by the schmitt trigger for synchronisation -> comparator compares this signal to ground -> duty cylce is not 50% at default. This should be easily fixable by adding a decoupling cap in series to R14, or by bypassing the comparator alltogether and connecting the 100k resistor directly to the SYNCH input

-LED is mostly usless excpet when using the module as an LFO.
