#ifndef FanController_h
#define FanController_h

#include <math.h>
#include <stdio.h>

#include "RgbLed.h"
#include "Shared.h"
#include "Triac.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"

class FanController {
 public:
  FanController(ZwiftData *zwiftData, RgbLed *heartRateLed, RgbLed *powerLed,
                RgbLed *combinedLed, Triac *triac, uint16_t maxHr,
                uint16_t ftp);
  void updateFan();

 private:
  void toFanPercent(float drivingPercent);
  ZwiftData *_zwiftData;
  RgbLed *_heartRateLed;
  RgbLed *_powerLed;
  RgbLed *_combinedLed;
  Triac *_triac;
  float _fanSlope;
  float _fanYInt;
  uint16_t _maxHr;
  uint16_t _ftp;
  float _fanPercent;
};

#endif
