#ifndef Triac_h
#define Triac_h

#include <stdio.h>

#include "Triac.pio.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"

const double CLOCK_DUR = 1 / (double)41666.;

class Triac {
 public:
  Triac(uint8_t zcPin, uint8_t triggerPin);
  void setTriggerWait(uint8_t cycles);

 private:
  uint8_t _zcPin;
  uint8_t _triggerPin;
  PIO _pio;
  uint _sm;
  pio_sm_config *_c;
  uint _offset;
};

#endif
