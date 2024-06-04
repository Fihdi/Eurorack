![Sequencer](https://raw.githubusercontent.com/Fihdi/Eurorack/main/CV_Trigger_Sequencer/DrumSequencer/SEQ-Front.png)

IN1 PIN MUST NOT BE POPULATED it is connected to the RESET Pin on the ATMega328P, unfortunately this pin needs to be either left unpopulated or the trace needs to be scratched and routed to another Pin. Alternatevely you could disable the RESET function with changed fuse settings (somehow)

No Rotary-Switch for the amount of steps.

Pulse / Trigger Outputs A, B and C have been made for the Sampler Module, but they can be used to trigger anything (Envelopes, Sample and Hold, etc)

Ideas:
One modifaction I will definitely try is to route one of the inputs (IN1 to 3) to an analog input pin of the ATMega328P. This way the stage which is active is CV-controllable. You can also programm an onboard LFO where the rate of this LFO can be CV-controlled.

Tap-tempo with an input or the push-button.
