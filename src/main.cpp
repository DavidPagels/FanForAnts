#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FanController.h"
#include "RgbLed.h"
#include "Shared.h"
#include "Triac.h"
#include "ZwiftServer.h"
#include "pico/stdlib.h"
#include "pico/time.h"

const uint16_t MAX_HR = 188;
const uint16_t FTP = 190;

int main() {
  stdio_init_all();
  ZwiftData *zwiftData = new ZwiftData();

  RgbLed *powerLed = new RgbLed(7, 8, 9);         // 19, 20, 21
  RgbLed *heartRateLed = new RgbLed(19, 20, 21);  // 22, 26, 27
  RgbLed *combinedLed = new RgbLed(16, 17, 18);   // 16, 17, 18
  RgbLed *statusLed = new RgbLed(11, 12, 13);     // 11, 12, 13
  Triac *triac = new Triac(15, 14);
  FanController *fanController = new FanController(
      zwiftData, heartRateLed, powerLed, combinedLed, triac, MAX_HR, FTP);

  ZwiftServer *zwiftServer =
      new ZwiftServer(zwiftData, fanController, statusLed);
  zwiftServer->init();

  cyw43_arch_enable_sta_mode();

  while (true) {
    sleep_ms(1000);
  }
}