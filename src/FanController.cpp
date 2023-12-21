#include "FanController.h"

FanController::FanController(ZwiftData *zwiftData, RgbLed *heartRateLed,
                             RgbLed *powerLed, RgbLed *combinedLed,
                             Triac *triac, uint16_t maxHr, uint16_t ftp) {
  _zwiftData = zwiftData;
  _heartRateLed = heartRateLed;
  _powerLed = powerLed;
  _combinedLed = combinedLed;
  _triac = triac;
  _fanSlope = (1 - 0.3) / (0.8 - 0.5);
  _fanYInt = 1 - _fanSlope * 0.8;
  _maxHr = maxHr;
  _ftp = ftp;
}

void FanController::updateFan() {
  _zwiftData->hr;
  float hrPercent = _zwiftData->hr / (float)_maxHr;
  float powerPercent = _zwiftData->power / (float)_ftp;
  float drivingPercent = hrPercent > powerPercent ? hrPercent : powerPercent;

  _heartRateLed->setLedZone(hrPercent);
  _powerLed->setLedZone(powerPercent);
  _combinedLed->setLedZone(drivingPercent);
  toFanPercent(drivingPercent);
}

void FanController::toFanPercent(float drivingPercent) {
  if (drivingPercent < 0.5) {
    // Approximate trigger delay for 30% power
    _triac->setTriggerWait(0.00526 / CLOCK_DUR);
    return;
  }
  if (drivingPercent > 0.8) {
    _triac->setTriggerWait(0);
    return;
  }
  float fanScaled = _fanSlope * drivingPercent + _fanYInt;

  // Using polynomial to approximate integral of 60hz half period sin wave
  float triggerDelay =
      -0.05682765 * pow(fanScaled, 4.0) + 0.12686553 * pow(fanScaled, 3.0) -
      0.10213589 * pow(fanScaled, 2.0) + 0.02949355 * fanScaled + 0.00262442;

  _triac->setTriggerWait(triggerDelay / CLOCK_DUR);
}
