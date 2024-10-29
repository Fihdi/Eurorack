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
// Analog input manager and value remapper


#ifndef _PTAL_Remapper_h
#define _PTAL_Remapper_h

namespace ptal {

#define REMAPPER_MAX_ZONES_COUNT 8
#define REMAPPER_RAW_EPSILON 16
#define REMAPPER_GET_DELTA(a, b) (a < b ? b - a : a - b)

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
struct remapperZone {
    float minX;
    float maxX;
    float minY;
    float maxY;
    float rangeX;
    float rangeY;
    bool  isDeadZone;
    int   index;
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
class Remapper {
  private:
    // float _min   = 0.0f;
    // float _max   = 1.0f;
    // float _range = 1.0f;
    remapperZone   _zones[REMAPPER_MAX_ZONES_COUNT];
    int            _zoneCount = 0;
    unsigned short _rawInput;

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _setZone (int index, float minX, float maxX, float minY, float maxY, bool isDeadZone) {
      _zones[index].minX       = minX;
      _zones[index].maxX       = maxX;
      _zones[index].minY       = minY;
      _zones[index].maxY       = maxY;
      _zones[index].isDeadZone = isDeadZone;
      _zones[index].rangeX     = maxX - minX;
      _zones[index].rangeY     = maxY - minY;
      _zones[index].index      = index;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _updateUpperBounds (remapperZone *zone, float maxX, float maxY) {
      zone->maxX   = maxX;
      zone->maxY   = maxY;
      zone->rangeX = maxX - zone->minX;
      zone->rangeY = maxY - zone->minY;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _setValueUsingZone (remapperZone *zone, float x) {
      isInDeadZone = zone->isDeadZone;
      zoneIndex    = zone->index;
      if (zone->isDeadZone) {
        value = zone->minY; // it is same as maxY
        return;
      }
      value = zone->minY + zone->rangeY * (x - zone->minX) / zone->rangeX;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void _updateValue () {
      // not initialized
      // if (_zoneCount == 0) {
      //   value = raw;
      //   return;
      // }

      // only one zone
      if (_zoneCount == 1) {
        _setValueUsingZone(&_zones[0], raw);
        return;
      }

      // find zone
      remapperZone* zone = &_zones[0];
      for (int i = 1; i < _zoneCount; i++) {
        // if (x >= zone.minX && x <= zone.maxX) break;
        if (raw <= zone->maxX) break;
        zone = &_zones[i];
      }

      // map in this zone
      _setValueUsingZone(zone, raw);
    }


  public:
    float value        = 0.0f;
    float raw          = 0.0f;
    bool  changed      = false;
    bool  isInDeadZone = false;
    int   zoneIndex    = 0;

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void SetBounds (float min, float max) {
      _setZone(0, 0.0f, 1.0f, min, max, false);
      _zoneCount = 1;
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void AddDeadZone (float x, float y, float width) {
      width /= 2;
      remapperZone* prev = &_zones[_zoneCount - 1];
      // TODO: check and resolve dead-zones collisions
      float lo = x - width;
      float hi = x + width;
      _setZone(_zoneCount++, lo, hi, y, y, true);
      _setZone(_zoneCount++, hi, prev->maxX, y, prev->maxY, false);
      _updateUpperBounds(prev, lo, y);
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    void AddBreakPoint (float x, float y) {
      remapperZone* prev = &_zones[_zoneCount - 1];
      _setZone(_zoneCount++, x, prev->maxX, y, prev->maxY, false);
      _updateUpperBounds(prev, x, y);
    }

    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    /**
     * remap an input value in range [0~1] to the bounds of the remaper
    */
    void SetRawValue (unsigned short input) {
      // TODO: add low-pass filter on input to filter noise
      // _rawInput = (_rawInput * 3.0 + input) / 4.0;

      // Check delta with previous rawInput to check if changed
      // if (REMAPPER_GET_DELTA(input, _rawInput) < REMAPPER_RAW_EPSILON) {
      //   return;
      // }

      _rawInput = input;
      changed   = true;

      raw = (float) input / 65536.0f;

      _updateValue();
    }


    //▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    float SetValue (float x) {
      raw = x;
      _updateValue();
      return value;
    }
};


} // namespace ptal

#endif // _PTAL_Remapper_h
