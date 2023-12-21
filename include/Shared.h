#ifndef Shared_h
#define Shared_h

#include "pico/stdlib.h"

typedef struct __attribute((packed)) {
  uint32_t hr;
  uint32_t power;
} ZwiftData;

#endif
