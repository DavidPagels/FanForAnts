#ifndef RgbLed_h
#define RgbLed_h

#include "hardware/pwm.h"
#include "pico/stdlib.h"

enum Zones {
  ZONE_1 = 0x2c88ff,
  ZONE_2 = 0x56c151,
  ZONE_3 = 0xffce20,
  ZONE_4 = 0xff652d,
  ZONE_5 = 0xff2f00,
  ZONE_PLUS = 0xff00ff
};

enum Status {
  STATUS_INIT = 0x0000FF,
  STATUS_CONNECTED = 0x00FF00,
  STATUS_WARN = 0xFFFF00,
  STATUS_ERROR = 0xFF0000
};

class RgbLed {
 public:
  RgbLed(uint rPin, uint gPin, uint bPin);
  void setLedZone(float percent);
  void setLedStatus(Status status);

 private:
  void setLedRgb(uint rgb);
  void initLedPin(uint pin, pwm_config *config);
  uint _rPin;
  uint _gPin;
  uint _bPin;
};

#endif
