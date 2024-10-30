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
// Rotary encoder debouncer


#ifndef _PTAL_Encoder2_h
#define _PTAL_Encoder2_h

#include "daisy_core.h"
#include "per/gpio.h"

using namespace daisy;

namespace ptal {

class Encoder2 {
private:
  daisy::GPIO _pinA;
  daisy::GPIO _pinB;

  uint8_t  _debounceA;
  uint8_t  _debounceB;
  uint32_t _lastUpdate;
  uint32_t _throttle;

  bool     _hasBounds;
  bool     _isCircular;
  int      _min;
  int      _max;

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void _enforceBounds () {
    if (_hasBounds) {
      if (_isCircular) {
        if (value < _min) value = _max;
        if (value > _max) value = _min;
      } else {
        if (value < _min) value = _min;
        if (value > _max) value = _max;
      }
    }
  }


public:
  int  increment;
  int  value;
  bool scrolled;


  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void Init (daisy::Pin pinA, daisy::Pin pinB, int throttle = 0) {
    _pinA.Init(pinA, daisy::GPIO::Mode::INPUT, daisy::GPIO::Pull::PULLUP);
    _pinB.Init(pinB, daisy::GPIO::Mode::INPUT, daisy::GPIO::Pull::PULLUP);

    value       = 0;
    increment   = 0;
    scrolled    = false;
    _debounceA  = 0xff;
    _debounceB  = 0xff;
    _hasBounds  = false;
    _isCircular = false;
    _lastUpdate = 0;
    _throttle   = throttle;
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void ResetScroll (int offset = 0) {
    value = offset;
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void Debounce () {
    increment = 0;
    scrolled  = false;

    // Throttle update speed
    if (_throttle > 0) {
      uint32_t now = System::GetNow();
      if(now - _lastUpdate < _throttle) return;
      _lastUpdate  = now;
    }

    // Shift Button states to debounce
    _debounceA = (_debounceA << 1) | _pinA.Read();
    _debounceB = (_debounceB << 1) | _pinB.Read();

    // infer increment direction
    if ((_debounceA & 0x03) == 0x02 && (_debounceB & 0x03) == 0x00) {
        increment = 1;
        scrolled  = true;
    } else if ((_debounceB & 0x03) == 0x02 && (_debounceA & 0x03) == 0x00) {
        increment = -1;
        scrolled  = true;
    }
    value += increment;
    _enforceBounds();
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void SetBounds (int min, int max) {
    _hasBounds = true;
    _min = min;
    _max = max;
    _enforceBounds();
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void SetBounds (int min, int max, int v) {
    _hasBounds = true;
    _min = min;
    _max = max;
    value = v;
    _enforceBounds();
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void SetCircular (bool value) {
    _isCircular = value;
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void SetCircularBounds (int min, int max, int v) {
    SetBounds(min, max, v);
    _isCircular = true;
  }

  //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  void SetValue (int v) {
    value = v;
    _enforceBounds();
  }
};


} // namespace ptal

#endif // _PTAL_Encoder2_h
