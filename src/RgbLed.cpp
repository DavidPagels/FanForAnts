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

void RgbLed::setLedZone(float percent) {
  if (percent < 0.65) {
    setLedRgb(ZONE_1);
  } else if (percent < 0.75) {
    setLedRgb(ZONE_2);
  } else if (percent < 0.85) {
    setLedRgb(ZONE_3);
  } else if (percent < 0.95) {
    setLedRgb(ZONE_4);
  } else if (percent < 1.0) {
    setLedRgb(ZONE_5);
  } else {
    setLedRgb(ZONE_PLUS);
  }
}

void RgbLed::setLedStatus(Status status) { setLedRgb(status); }

void RgbLed::setLedRgb(uint rgb) {
  pwm_set_gpio_level(_rPin, ((rgb >> 16) & 0xFF) * 200);
  pwm_set_gpio_level(_gPin, ((rgb >> 8) & 0xFF) * 120);
  pwm_set_gpio_level(_bPin, (rgb & 0xFF) * 64);
}