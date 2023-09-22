#ifndef RgbLed_h
#define RgbLed_h

#include "pico/stdlib.h"
#include "hardware/pwm.h"

class RgbLed {
    public:
        RgbLed(uint rPin, uint gPin, uint bPin);
        void setLedRgb(uint rgb);
    private:
        void initLedPin(uint pin, pwm_config *config);
        uint _rPin;
        uint _gPin;
        uint _bPin;
};

#endif
