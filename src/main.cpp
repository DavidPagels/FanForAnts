#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdio.h"

#include "RgbLed.h"
#include "ZwiftServer.h"
#include "FanController.h"
#include "Shared.h"

uint heartRateZ1 = 0x2c88ff;
uint heartRateZ2 = 0x56c151;
uint heartRateZ3 = 0xffce20;
uint heartRateZ4 = 0xff652d;
uint heartRateZ5 = 0xff2f00;

int main() {
  stdio_init_all();
  ZwiftData zwiftData = {.hr = 0, .power = 0};

  RgbLed *heartRateLed = new RgbLed(16, 17, 18);
  RgbLed *statusLed = new RgbLed(19, 20, 21);
  FanController *fanController = new FanController(&zwiftData, heartRateLed);

  ZwiftServer *zwiftServer = new ZwiftServer(&zwiftData, fanController, statusLed);
  zwiftServer->init();

  cyw43_arch_enable_sta_mode();

  while (true) {
    sleep_ms(1000);
  }
}