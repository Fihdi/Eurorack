// Title: delayline
// Description: Delays a pinged osc signal
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisyDuino.h"

DaisyHardware hw;

size_t num_channels;

// Set max delay time to 0.75 of samplerate.
#define MAX_DELAY static_cast<size_t>(48000 * 0.75f)

// Helper Modules

// Declare a DelayLine of MAX_DELAY number of floats.
static DelayLine<float, MAX_DELAY> del;

void MyCallback(float **in, float **out, size_t size) {
  float sig_in, env_out, feedback, del_out, sig_out;
  for (size_t i = 0; i < size; i++) {
    // Read from delay line
    sig_in = in[0][i];
    del_out = del.Read();
    // Calculate output and feedback
    sig_out = del_out + sig_in;
    feedback = (del_out * 0.75f) + sig_in;

    // Write to the delay
    del.Write(feedback);

    for (size_t chn = 0; chn < num_channels; chn++) {
      out[chn][i] = sig_out;
    }
  }
}

void setup() {
  float sample_rate;
  // Initialize for Daisy pod at 48kHz
  hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  num_channels = hw.num_channels;
  sample_rate = DAISY.get_samplerate();

  del.Init();
  del.SetDelay(sample_rate * 0.75f); //Make this clockable.

  DAISY.begin(MyCallback);
}

void loop() {}
