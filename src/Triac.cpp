#include "Triac.h"

Triac::Triac(uint8_t zcPin, uint8_t triggerPin) {
  _zcPin = zcPin;
  _triggerPin = triggerPin;

  pio_hw_t *pios[2] = {pio0, pio1};
  uint pio_index = 0;
  if (!pio_can_add_program(pios[pio_index], &triac_program)) {
    pio_index = 1;
    if (!pio_can_add_program(pios[pio_index], &triac_program)) {
      throw 0;
    }
  }
  _pio = pios[pio_index];
  _offset = pio_add_program(_pio, &triac_program);
  _sm = pio_claim_unused_sm(_pio, true);
  pio_sm_config tmpConfig = triac_program_get_default_config(_offset);
  _c = &tmpConfig;
  triac_program_init(_pio, _sm, _offset, _zcPin, _triggerPin, _c);
}

void Triac::setTriggerWait(uint8_t cycles) {
  printf("cycles: %d\n", cycles);
  pio_sm_clear_fifos(_pio, _sm);
  // Less than 60 and triac interprets it as firing before zc for some reason,
  // even though probe shows it's firing as expected.
  uint8_t newCycles = cycles < 60 ? 0 : cycles;
  pio_sm_put_blocking(_pio, _sm, newCycles << 24);
  busy_wait_us(4 * 8 + 450);
}
