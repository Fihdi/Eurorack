#ifndef _SimpleSampler_hardware_h
#define _SimpleSampler_hardware_h


#include <stdio.h>
#include <string.h>
#include "daisy_seed.h"
#include "fatfs.h"
#include "../PTAL/core/Debouncer.h"
#include "../PTAL/core/Encoder2.h"
#include "../PTAL/core/Remapper.h"
#include "../PTAL/core/PwmLed.h"


using namespace daisy;

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
// #define USE_POKI

#ifdef USE_POKI
  #define PIN_CV_IN     17
  #define PIN_LED       14
  #define PIN_ENCODER_A 10
  #define PIN_ENCODER_B 11
  #define PIN_GATE_PLAY 15
  #define PIN_GATE_RAND 27
  #define PIN_GATE_PART 25
#else
  #define PIN_CV_IN     15
  #define PIN_LED       17
  #define PIN_ENCODER_A 19
  #define PIN_ENCODER_B 20
  #define PIN_GATE_PLAY 16
  #define PIN_GATE_RAND 18
  #define PIN_GATE_PART 21
#endif

#define GATE_COUNT 3
static const int ADC_COUNT = 1;
static const int LED_PWM_MAX = ptal::PwmLed::GetPwmSize();

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
SdmmcHandler   _sdCard;
FatFSInterface fatFsInterface;


//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
class Hardware {
private:
  int _currentGateIndex;
  int _ledPwmCounter;

public:
  DaisySeed       daisy;
  ptal::Debouncer gates[3];
  ptal::Encoder2  encoder;
  ptal::Remapper  cvInput;
  ptal::PwmLed    led;

  ptal::Debouncer *gatePlay = &gates[0];
  ptal::Debouncer *gateRand = &gates[1];
  ptal::Debouncer *gatePart = &gates[2];


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void Init () {
    daisy.Configure(); // NOTA: deprecated. This actually does nothing and can be removed
    daisy.Init();

    // Initialize analog inputs (ADC)
    AdcChannelConfig adc[ADC_COUNT];
    adc[0].InitSingle(daisy.GetPin(PIN_CV_IN));
    daisy.adc.Init(adc, ADC_COUNT);
    daisy.adc.Start();
    cvInput.SetBounds(0.0f, 1.0f);

    // Initialize encoder
    encoder.Init(daisy.GetPin(PIN_ENCODER_A), daisy.GetPin(PIN_ENCODER_B), 4);

    // Initialize input gates.
    gates[0].Init(daisy.GetPin(PIN_GATE_PLAY), 30, true, DSY_GPIO_PULLUP);
    gates[1].Init(daisy.GetPin(PIN_GATE_RAND), 30, true, DSY_GPIO_PULLUP);
    gates[2].Init(daisy.GetPin(PIN_GATE_PART), 30, true, DSY_GPIO_PULLUP);
    _currentGateIndex = 0;

    // Initialize LED
    ptal::PwmLed::SetGlobalSpeed(340);
    led.Init(daisy.GetPin(PIN_LED));
    _ledPwmCounter = 0;

    System::Delay(200);
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void InitSdCard () {
    // Initialize SD card and file system interface
    SdmmcHandler::Config sdCardHandlerConfig;
    sdCardHandlerConfig.Defaults(); // High Speed (50MHz)
    _sdCard.Init(sdCardHandlerConfig);

    // Links libdaisy i/o to fatfs driver.
    fatFsInterface.Init(FatFSInterface::Config::MEDIA_SD);

    // Mount SD Card
    f_mount(&fatFsInterface.GetSDFileSystem(), "/", 1);
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  const char* GetSdCardPath () {
    return fatFsInterface.GetSDPath();
  }


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void Update () {
    encoder.Debounce();
    cvInput.SetRawValue(daisy.adc.Get(0));

    // LED
    led.Update(_ledPwmCounter);
    if (++_ledPwmCounter > LED_PWM_MAX) _ledPwmCounter = 0;

    // Gate inputs: debounce only one per cycle
    gates[_currentGateIndex].ResetEdges();
    if (++_currentGateIndex >= GATE_COUNT) _currentGateIndex = 0;
    gates[_currentGateIndex].Debounce();
  }

};


#endif // _SimpleSampler_hardware_h
