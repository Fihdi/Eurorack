#include "daisy_seed.h"
#include "../PTAL/hardwares/Hardware.h"
#include "./MemoryManager.h"
#include "./SamplePlayer.h"
#include "./FilesScanner.h"
#include "./debug.h"
#include "./constants.h"


using namespace daisy;


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
Hardware      hardware;
MemoryManager memoryManager;
SamplePlayer  samplePlayer;
FilesScanner  filesScanner;



//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
static void AudioCallback (
  daisy::AudioHandle::InputBuffer  in,
  daisy::AudioHandle::OutputBuffer out,
  size_t size) {

  // Clear buffer
  memset(out[0], 0, size * sizeof(float));
  memset(out[1], 0, size * sizeof(float));

  // Process sample player
  samplePlayer.ProcessBlock(out[0], out[1], size);
}


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
int main (void) {
  // initializations
  hardware.Init();
  debug.Init(&hardware);
  hardware.InitSdCard();
  memoryManager.Init();
  samplePlayer.Init();
  filesScanner.Init(&hardware);
  filesScanner.ScanDirectory(&memoryManager, &samplePlayer);


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  // Setup audio callback
  hardware.daisy.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
  hardware.daisy.SetAudioBlockSize(AUDIO_BLOCK_SIZE);
  hardware.daisy.StartAudio(AudioCallback);


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  while (true) {
    hardware.Update();

    if (hardware.cvInput.changed) {
      samplePlayer.SetSpeed(0.5f + hardware.cvInput.value * 1.5f);
    }

    if (hardware.encoder.scrolled) {
      if (hardware.encoder.increment ==  1) samplePlayer.NextSample();
      if (hardware.encoder.increment == -1) samplePlayer.PrevSample();
    }

    if (hardware.gatePlay->risingEdge) samplePlayer.TriggerSample();

    // TODO: gateRand, gatePart

    hardware.led.SetSolid(samplePlayer.IsPlaying());
  }
}
