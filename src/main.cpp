#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "math.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdio.h"

uint heartRateRPin = 16;
uint heartRateGPin = 17;
uint heartRateBPin = 18;

uint heartRateZ1 = 0x2c88ff;
uint heartRateZ2 = 0x56c151;
uint heartRateZ3 = 0xffce20;
uint heartRateZ4 = 0xff652d;
uint heartRateZ5 = 0xff2f00;

uint powerRPin = 19;
uint powerGPin = 20;
uint powerBPin = 21;

void initLedPin(uint pin, pwm_config *config) {
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_init(slice_num, config, true);
}

void setHeartRateRgb(uint rgb) {
  pwm_set_gpio_level(heartRateRPin, ((rgb >> 16) & 0xFF) * 200);
  pwm_set_gpio_level(heartRateGPin, ((rgb >> 8) & 0xFF) * 120);
  pwm_set_gpio_level(heartRateBPin, (rgb & 0xFF) * 64);
}

void setPowerRgb(uint rgb) {
  pwm_set_gpio_level(powerRPin, ((rgb >> 16) & 0xFF) * 200);
  pwm_set_gpio_level(powerGPin, ((rgb >> 8) & 0xFF) * 120);
  pwm_set_gpio_level(powerBPin, (rgb & 0xFF) * 64);
}

int main() {
  stdio_init_all();
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 4.f);
  initLedPin(heartRateRPin, &config);
  initLedPin(heartRateGPin, &config);
  initLedPin(heartRateBPin, &config);
  initLedPin(powerRPin, &config);
  initLedPin(powerGPin, &config);
  initLedPin(powerBPin, &config);

  while (true) {
    setHeartRateRgb(heartRateZ1);
    sleep_ms(500);
    setHeartRateRgb(heartRateZ2);
    sleep_ms(500);
    setHeartRateRgb(heartRateZ3);
    sleep_ms(500);
    setHeartRateRgb(heartRateZ4);
    sleep_ms(500);
    setHeartRateRgb(heartRateZ5);
    sleep_ms(500);
  }
}