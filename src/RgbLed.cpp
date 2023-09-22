#include "RgbLed.h"

RgbLed::RgbLed(uint r, uint g, uint b) {
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    _rPin = r;
    _gPin = g;
    _bPin = b;
    initLedPin(_rPin, &config);
    initLedPin(_gPin, &config);
    initLedPin(_bPin, &config);
}

void RgbLed::initLedPin(uint pin, pwm_config *config) {
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_init(slice_num, config, true);
}

void RgbLed::setLedRgb(uint rgb) {
  pwm_set_gpio_level(_rPin, ((rgb >> 16) & 0xFF) * 200);
  pwm_set_gpio_level(_gPin, ((rgb >> 8) & 0xFF) * 120);
  pwm_set_gpio_level(_bPin, (rgb & 0xFF) * 64);
}