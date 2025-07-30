# ADSR module with voltage controllable Sustain level and Release time.

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/SimpleADSR/ADSR_Front.png)

I wasn't using the CV-Features of my VCADSR module at all so I cut out all of the unnecessary CV-parts. At the end I was still left with a pretty big schematic, so I started to reduce it. The finished module now has the following features:

Attack:		Sets the Attack time.

Decay:		Sets the Decay time.

Sustain:	Sets the Sustain level. This can be controlled externally with a control voltage on the SUS input.

Release:	Sets the Release time. This can be controlled externall with a control voltage on the CV input.

GATE:		Gate input, main input to use the module, goes into the decay and sustain phases while this input is high.

TRG:		Trigger input, the module starts the Attack phase regardless in which phase the module currently is.

EOC:		End-of-cycle output, this output becomes high at the end of the release phase.

OUT:		ADSR output.

LOOP:		Loop switch, re-triggers the module at the end of the release phase. Same effect as connecting EOC to GATE or TRG.
  

Hacks:

Slew Rate Limiter: While the module is in the sustain phase, an external signal can be applied to the SUS input. A slewed version of this signal is output at the OUT jack. The "DECAY" knob controls the slew rate.

![](https://raw.githubusercontent.com/Fihdi/Eurorack/refs/heads/main/Misc/ADSR_RL.jpeg)
 
The module that you see in the picture above is using circuit boards that were fabricated by PCBWay. They have excellent build quality and I can recommend using their service to build your own projects! I used the "Black" option for the soldermask and the surface has very deep and glossy black finish, which looks amazing, they also have an option for a "Matte Black" finish which I will try next time. Thank you PCBWay for sponsoring this module. 
