# Full Synthesizer Voice with Envelope Generator

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/Voice/VoiceFront.png)

The Envelope generator is my original decay generator with an instant attack and voltage controllable decay time. The accent can also be controlled via CV. The decay signal can modulate the other three sections with variable strengths or be used independantly.

The VCO is a triangle-core, OTA based VCO, same as in the TRIVCO. The sub-oscillator is created by feeding the pulse output of the VCO into a 4040 counter IC to create another square-wave output at half the frequency. The Sub output and the PWM signal are summed together, the strenght of the SUB output can be controlled with the SUB knob. Turning "PWM" all the way to the max/min turns the original square wave signal off by setting the duty cycle to 0% or 100%. 

The VCF section used to be a AS3350 circuit. It worked, but I was not happy with the sound of it, I was hoping for squelchy, acid like sounds, which I couldnt get out of it the way I wanted. I decided to adapt the existing VCF PCB so it can be slotted into the backside of the module.

The VCA is the VCA-3 by Rene Schmitz, the same that I use for my percussion modules like Hat, Bell or Fold, it is an incredibly reliable circuit. It is DC biased so modulating CV is possible. 

When building this module, normalize the Square wave output to the IN jack of the VCF and normalize the LP output to the IN of the VCA.

I am thinking about making another version of this Module featuring a synch-able LFO instead of the envelope and maybe adding a Noise OUT as well. 

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/Misc/Voice_RL.jpeg)

The module that you see in the picture above is using circuit boards that were fabricated by PCBWay. This is the biggest set of PCBs in terms of size, being 20 hp in width, that I ordered and they all worked flawlessly the first time! They have excellent build quality and I can recommend using their service to build your own projects! I used the "Black" option for the soldermask and the surface has very deep and glossy black finish, which looks amazing, they also have an option for a "Matte Black" finish which I will try next time. Thank you PCBWay for sponsoring this module. 
