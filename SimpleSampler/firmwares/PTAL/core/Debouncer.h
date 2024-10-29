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
    dsy_gpio _pin;
    int      _state;
    int      _debounceCounter;
    bool     _isDebouncing;
    int      _debounceLength;
    bool     _inverted;

  public:

    bool risingEdge;
    bool fallingEdge;
    bool value;

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void Init (
      dsy_gpio_pin  pin,
      int           debounceLength, // = 8,
      bool          invertedInput, //  = true,
      dsy_gpio_pull pull //            = DSY_GPIO_PULLUP
    ) {
      _pin.pin  = pin;
      _pin.mode = DSY_GPIO_MODE_INPUT;
      _pin.pull = pull;
      dsy_gpio_init(&_pin);

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

      int pinValue = dsy_gpio_read(&_pin);

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
