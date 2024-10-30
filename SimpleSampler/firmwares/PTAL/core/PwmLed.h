// Copyright 2024 Cedric Stoquer.
//
// Author: Cedric Stoquer
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// Soft-PWM LED driver


#ifndef _PTAL_PwmLed_h
#define _PTAL_PwmLed_h

#include "daisy_core.h"
#include "per/gpio.h"


namespace ptal {

static const int LED_MAX_PWM = 64;

int _pwmLed_globalSpeed;


class PwmLed {
  private:

    enum Mode {
      MODE_SOLID,
      MODE_PWM,
      MODE_BLINK,
      MODE_BLINK_PWM,
      MODE_PULSE,
      MODE_TRIANGLE,
      MODE_FLASH,
    };

    struct LedState {
      Mode mode;
      bool state;
      int  pwm;
      int  speed;
      int  counter;
      int  min;
      int  max;
      int  direction;
    };


    daisy::GPIO _pin;
    int         _pwmThreshold;     // width of the PWM (64 levels)
    Mode        _mode;             // animation mode
    bool        _state;            // hardware state of the LED (ON or OFF)
    int         _animationSpeed;   // how many update required to increment one frame
    int         _animationCounter; // counter to control the duration of one frame
    int         _animationMin;     // min intensity of animation
    int         _animationMax;     // max intensity of animation
    int         _lfoDirection;
    LedState    _ledState;



    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _updatePwm (int pwmCounter) {
      bool value = pwmCounter < _pwmThreshold;
      if (_state == value) return;
      _pin.Write(value);
      _state = value;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _setAnimationMinMax (int min, int max) {
      _animationMin = min < max ? min : max;
      _animationMax = max > min ? max : min;
      if (_animationMin < 0) _animationMin = 0;
      if (_animationMax > LED_MAX_PWM) _animationMax = LED_MAX_PWM;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _saveState () {
      _ledState.mode      = _mode;
      _ledState.state     = _state;
      _ledState.pwm       = _pwmThreshold;
      _ledState.speed     = _animationSpeed;
      _ledState.counter   = _animationCounter;
      _ledState.min       = _animationMin;
      _ledState.max       = _animationMax;
      _ledState.direction = _lfoDirection;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _restoreState () {
      if (_ledState.state != _state) _pin.Write(_ledState.state);
      _mode             = _ledState.mode;
      _state            = _ledState.state;
      _pwmThreshold     = _ledState.pwm;
      _animationSpeed   = _ledState.speed;
      _animationCounter = _ledState.counter;
      _animationMin     = _ledState.min;
      _animationMax     = _ledState.max;
      _lfoDirection     = _ledState.direction;
    }


  public:
    enum AnimSpeed {
      SLOW,
      MID,
      FAST,
    };

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    static int GetPwmSize () {
      return LED_MAX_PWM;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    static void SetGlobalSpeed (int value) {
      _pwmLed_globalSpeed = value;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void Init (daisy::Pin pin) {
      _pin.Init(pin, GPIO::Mode::OUTPUT);
      _mode  = Mode::MODE_SOLID;
      _state = false;
      _pin.Write(false);
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetPwm (float value) {
      int pwmValue = (int)(value * value * (float)LED_MAX_PWM);
      SetPwm(pwmValue);
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetPwm (int value) {
      if (value <= 0)           { SetSolid(false); return; }
      if (value >= LED_MAX_PWM) { SetSolid(true);  return; }

      if (_mode == Mode::MODE_FLASH) {
        _ledState.mode    = Mode::MODE_PWM;
        _ledState.state   = false;
        _ledState.pwm     = value;
        _ledState.counter = 0;
        return;
      }

      _pwmThreshold = value;
      _mode = Mode::MODE_PWM;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetSolid (bool value) {
      if (_mode == Mode::MODE_FLASH) {
        _ledState.mode  = Mode::MODE_SOLID;
        _ledState.state = value;
        return;
      }

      _mode = Mode::MODE_SOLID;
      if (_state == value) return;
      _state = value;
      _pin.Write(value);
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetBlink (AnimSpeed speed = AnimSpeed::MID) {
      int animationSpeed;

      switch (speed) {
        case AnimSpeed::SLOW: animationSpeed = 121; break;
        case AnimSpeed::MID:  animationSpeed =  65; break;
        case AnimSpeed::FAST: animationSpeed =  32; break;
      }

      SetBlink(animationSpeed);
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetBlink (int speed = 64) {
      speed *= _pwmLed_globalSpeed;

      if (_mode == Mode::MODE_FLASH) {
        _ledState.mode    = Mode::MODE_BLINK;
        _ledState.speed   = speed;
        _ledState.counter = speed;
        return;
      }

      _mode = Mode::MODE_BLINK;
      _animationSpeed   = speed;
      _animationCounter = speed;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetBlinkDim (AnimSpeed speed = AnimSpeed::MID, int min = 0, int max = LED_MAX_PWM) {
      int animationSpeed;

      switch (speed) {
        case AnimSpeed::SLOW: animationSpeed = 121 * _pwmLed_globalSpeed; break;
        case AnimSpeed::MID:  animationSpeed =  65 * _pwmLed_globalSpeed; break;
        case AnimSpeed::FAST: animationSpeed =  32 * _pwmLed_globalSpeed; break;
      }

      if (_mode == Mode::MODE_FLASH) {
        _ledState.mode    = Mode::MODE_BLINK_PWM;
        _ledState.speed   = animationSpeed;
        _ledState.counter = animationSpeed;
        _ledState.pwm     = min < max ? min : max;
        _ledState.min     = min;
        _ledState.max     = max;
        return;
      }

      _mode = Mode::MODE_BLINK_PWM;
      _animationSpeed   = animationSpeed;
      _animationCounter = animationSpeed;
      _setAnimationMinMax(min, max);
      _pwmThreshold = min < max ? min : max;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetPulse (AnimSpeed speed = AnimSpeed::MID) {
      int animationSpeed;

      switch (speed) {
        case AnimSpeed::SLOW: animationSpeed = LED_MAX_PWM * 8; break;
        case AnimSpeed::MID:  animationSpeed = LED_MAX_PWM * 3;  break;
        case AnimSpeed::FAST: animationSpeed = LED_MAX_PWM;      break;
      }

      if (_mode == Mode::MODE_FLASH) {
        _ledState.mode    = Mode::MODE_PULSE;
        _ledState.pwm     = LED_MAX_PWM;
        _ledState.speed   = animationSpeed;
        _ledState.counter = animationSpeed;
        return;
      }

      _mode = Mode::MODE_PULSE;
      _animationSpeed   = animationSpeed;
      _animationCounter = animationSpeed;
      _pwmThreshold     = LED_MAX_PWM;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetTriangle (int speed, int min = 0, int max = LED_MAX_PWM) {
      int animationSpeed = speed * LED_MAX_PWM;

      if (_mode == Mode::MODE_FLASH) {
        _ledState.mode      = Mode::MODE_TRIANGLE;
        _ledState.pwm       = min;
        _ledState.speed     = animationSpeed;
        _ledState.counter   = animationSpeed;
        _ledState.min       = min;
        _ledState.max       = max;
        _ledState.direction = 1;
        return;
      }

      _mode = Mode::MODE_TRIANGLE;
      _setAnimationMinMax(min, max);

      _animationSpeed   = animationSpeed;
      _animationCounter = animationSpeed;
      _pwmThreshold     = _animationMin;
      _lfoDirection     = 1;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetFlash () {
      _animationMax = 8; // using max to store the flash count

      if (_mode == Mode::MODE_FLASH) {
        return;
      }

      _saveState();
      _mode = Mode::MODE_FLASH;
      _animationSpeed   = 48 * _pwmLed_globalSpeed;
      _animationCounter = _animationSpeed;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void Update (int pwmCounter) {
      switch (_mode) {
        //--------------------------------------------------------------------------
        // SOLID: the LED stay ON or OFF. We don't need to update anything
        case Mode::MODE_SOLID:
          break;


        //--------------------------------------------------------------------------
        // PWM: dimmed value without animation
        case Mode::MODE_PWM:
          _updatePwm(pwmCounter);
          break;


        //--------------------------------------------------------------------------
        // BLINK: LED blink between ON and OFF
        case Mode::MODE_BLINK:
          if (--_animationCounter == 0) {
            _animationCounter = _animationSpeed;
            _state = !_state;
            _pin.Write(_state);
          }
          break;


        //--------------------------------------------------------------------------
        // BLINK_PWM: LED blink between 2 discrete values
        case Mode::MODE_BLINK_PWM:
          if (--_animationCounter == 0) {
            _animationCounter = _animationSpeed;
            if (_pwmThreshold <= _animationMin) {
              _pwmThreshold = _animationMax;
            } else {
              _pwmThreshold = _animationMin;
            }
          }
          _updatePwm(pwmCounter);
          break;


        //--------------------------------------------------------------------------
        // PULSE: A single pulse decaying until it become solid OFF
        case Mode::MODE_PULSE:
          if (--_animationCounter == 0) {
            if (_pwmThreshold <= 1) {
              // end of pulse: set to a solid OFF state
              SetSolid(false);
              return;
            } else {
              // dim pulse pwm by one
              _pwmThreshold -= 1;
              _animationCounter = _animationSpeed;
            }
          }
          _updatePwm(pwmCounter);
          break;


        //--------------------------------------------------------------------------
        // TRIANGLE: triangle LFO between 2 values
        case Mode::MODE_TRIANGLE:
          if (--_animationCounter == 0) {
            _animationCounter = _animationSpeed;
            _pwmThreshold += _lfoDirection;
            if (_pwmThreshold <= _animationMin) {
              _lfoDirection = 1;
            } else if (_pwmThreshold >= _animationMax) {
              _lfoDirection = -1;
            }
          }
          _updatePwm(pwmCounter);
          break;


        //--------------------------------------------------------------------------
        // FLASH: blink for a short time then go back to previous animation
        case Mode::MODE_FLASH:
          if (--_animationCounter == 0) {
            if (--_animationMax <= 0) {
              _restoreState();
            } else {
              _animationCounter = _animationSpeed;
              _state = !_state;
              _pin.Write(_state);
            }
          }
          break;

      }


    }
};


} // namespace ptal

#endif // _PTAL_PwmLed_h
