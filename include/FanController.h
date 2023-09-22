#ifndef FanController_h
#define FanController_h

#include "pico/stdlib.h"
#include "RgbLed.h"
#include "Shared.h"

class FanController {
    public:
        FanController(ZwiftData *zwiftData, RgbLed *heartRateLed);
        void updateFan();
    private:
        ZwiftData *_zwiftData;
        RgbLed *_heartRateLed;
};

#endif
