![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/Fold2/FoldFront.png)

A wavefolder with CV control over the amount of folds.

Main principle is the same as in Buchlas "Timbre Modulation" circuit where the input signal is split into multiple "deadbands" and then re-added and subtracted from each other to produce the folded output.

The deadbands are made using this diode structure, it can also be seen as a clamping circuit with variable clamping voltages:

An example with the upper limit being 5V and the lower limit being 3V. The output of this circuit follows the input signal as long as it is within the upper and lower limits. The output never exceeds these limits (ignoring the diode drop).

The resistor chain and buffers are there to provide the upper and lower limits for the diodes.

How to get a folded output from deadbands?


Every other band is summed together, resulting in two signals A and B. The two signals cross each other, and when B is subtracted from A, the resulting wave changes in polarity at every crossing. 
