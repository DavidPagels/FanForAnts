#include "pico/stdlib.h"
#include "FanController.h"

FanController::FanController(ZwiftData *zwiftData, RgbLed *heartRateLed) {
    _zwiftData = zwiftData;
    _heartRateLed = heartRateLed;
}

void FanController::updateFan() {
    _heartRateLed->setLedRgb(0xbadbed);
}
