#include "daisy_seed.h"
#include "../PTAL/hardwares/Hardware.h"
#include "./Osc.h"
#include "./Lpg.h"
#include "./DecayEnvelope.h"


using namespace daisy;

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
Hardware      hardware;
Osc           osc;
Lpg           lpg;
DecayEnvelope env;

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
static void AudioCallback (
  daisy::AudioHandle::InputBuffer  in,
  daisy::AudioHandle::OutputBuffer out,
  size_t size) {

  for (size_t i = 0; i < size; ++i) {
    float sample = lpg.Process(osc.Process(), env.Process());
    out[0][i] = sample;
    out[1][i] = sample;
  }
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
void UpdateSynthParams () {
  float v = hardware.cvInput.value;
  osc.SetFrequency(300.0f + v * 80.0f);
  env.SetLengthMs(300.0f - v * 90.0f);
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
int main (void) {
  // initializations
  hardware.Init();
  UpdateSynthParams();

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  // Setup audio callback
  hardware.daisy.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
  hardware.daisy.SetAudioBlockSize(16);
  hardware.daisy.StartAudio(AudioCallback);

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  while (true) {
    hardware.Update();

    hardware.led.SetPwm(env.GetValue());

    if (hardware.gatePlay->risingEdge) {
      env.Trigger();
    }

    if (hardware.cvInput.changed) {
      UpdateSynthParams();
    }
  }
}
