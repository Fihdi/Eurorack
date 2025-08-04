# 8 Channel MIDI interface module in 8hp
![](https://raw.githubusercontent.com/Fihdi/Eurorack/main/MIDI-Interface/MIDI-INT-Front.png)

MIDI Interface module with 5-Pin MIDI input and with 4 CV and 4 Gate/Trigger outputs. It lets you control other modules with MIDI Controllers or DAWs.

CV channels are mapped to MIDI CC number 20, 21, 22 and 23.

# Expansion

To expand module to 16 channels, the Pin beneath the TL072 can be used to connect two MIDI-INT Modules together. Leave the TL072 unpopulated on the second module and upload MIDI-INT-EXP.ino into the Arduino Nano.

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/MIDI-Interface/MIDI-Expansion.png)

CV channels for the expansion module are mapped to MIDI CC numbers 24, 25, 26 and 27.

Module is inspired by CircuitsAndSounds AutomationSequencer Project and is similar to an Univer Inter from Noise Engineering

https://github.com/CircuitsAndSounds/MIDIAutomationSequencer
