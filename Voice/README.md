# Full Synthesizer Voice with Envelope Generator

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/Voice/VoiceFront.png)

The Envelope generator is my original decay generator with an instant attack and voltage controllable decay time. The accent can also be controlled via CV. The decay signal can modulate the other three sections with variable strengths or be used independantly.

The VCO is a modified Version of Moritz Kleins Shapes VCO based on Synth Mongers Poor mans VCO. The CD40106 has been replaced with an Opamp based Schmitt-Trigger with positive biasing. I am thinking about replacing this section with a triangle-core, OTA based VCO.

The sub-oscillator is created by feeding the pulse output of the VCO into a D-Flipflop to create another square-wave output at half the frequency. A neat thing about the D-flipflop method is, even with short puleses as the input, the D-flipflop produces a 50% Duty Cycle output. The Sub output and the PWM signal are summed together, the strenght of the SUB output can be controlled with the SUB knob.

The VCF section used to be a AS3350 circuit. It worked, but I was not happy with the sound of it, I was hoping for squelchy, acid like sounds, which I couldnt get out of it the way I wanted. I decided to adapt the existing VCF PCB so it can be slotted into the backside of the module.

The VCA is the VCA-3 by Rene Schmitz, the same that I use for my percussion modules like Hat, Bell or Fold, it is an incredibly reliable circuit. It is DC biased so modulating CV is possible. 

When building this module, normalize the Square wave output to the IN jack of the VCF and normalize the LP output to the IN of the VCA.

I am thinking about making another version of this Module featuring a synch-able LFO instead of the envelope and maybe adding a Noise OUT as well. 
