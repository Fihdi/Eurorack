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
// Digital input debouncer


#ifndef _PTAL_Debouncer_h
#define _PTAL_Debouncer_h

#include "daisy_core.h"
#include "per/gpio.h"

namespace ptal {

class Debouncer {
  private:
    daisy::GPIO _pin;
    int         _state;
    int         _debounceCounter;
    bool        _isDebouncing;
    int         _debounceLength;
    bool        _inverted;

  public:

    bool risingEdge;
    bool fallingEdge;
    bool value;

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void Init (
      daisy::Pin        pin,
      int               debounceLength, // = 8,
      bool              invertedInput = true,
      daisy::GPIO::Pull pull = daisy::GPIO::Pull::PULLUP
    ) {
      _pin.Init(pin, daisy::GPIO::Mode::INPUT, pull);

      _debounceLength  = debounceLength;
      _inverted        = invertedInput;
      _state           = invertedInput ? 1 : 0;
      _debounceCounter = 0;
      _isDebouncing    = false;
      risingEdge       = false;
      fallingEdge      = false;
      value            = false;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void ResetEdges () {
      risingEdge  = false;
      fallingEdge = false;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void Debounce () {
      risingEdge  = false;
      fallingEdge = false;

      int pinValue = _pin.Read();

      if (_state != pinValue) {
        _state = pinValue;
        _isDebouncing = true;
        _debounceCounter = 0;
      } else if (_isDebouncing) {
        if (++_debounceCounter >= _debounceLength) {
          _isDebouncing = false;
          _debounceCounter = 0;
          value = (pinValue > 0) != _inverted;
          if (value) {
            risingEdge = true;
          } else {
            fallingEdge = true;
          }
        }
      }
    }
};


} // namespace ptal

#endif // _PTAL_Debouncer_h
