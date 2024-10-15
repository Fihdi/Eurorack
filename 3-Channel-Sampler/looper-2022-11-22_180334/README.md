![Sampler](https://raw.githubusercontent.com/Fihdi/Eurorack/main/3-Channel-Sampler/looper-2022-11-22_180334/Sampler-Front.png)

3 Channel Sampler that pairs well with the A, B and C outputs of my sequencer

This module sounds really bad because I used ISD1820 ICs for the sampling, which are low quality. I made a new Project "SimpleSampler" that uses a Daisy Seed to store and play Samples.

Known Issue: The solder points for the buttons are too close to the hole.
When the push button is stuck through the PCB and tightend with a washer, the washer will make contact with both solder points and the button is effectively always pressed.
I have put some small sheets of isolating rubber underneath the buttons to prevent this issue.

The summing amplifier should have a gain of two since the ISD1820 are running on 5V and are rather quiet compared to normal eurorack levels.

Sources and Inspiration:

https://bastl-instruments.com/support/tutorials/omsynth-project-3
https://www.youtube.com/watch?v=epjCe3o5BPA
