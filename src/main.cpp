#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "math.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdio.h"

// Todo: use a struct to store rgb pins to abstract this out

typedef struct {
  uint r;
  uint g;
  uint b;
} RgbLed;

RgbLed heartRateLed = {16, 17, 18};
RgbLed powerLed = {16, 17, 18};
RgbLed combinedLed = {16, 17, 18};
RgbLed statusLed = {19, 20, 21};

uint heartRateZ1 = 0x2c88ff;
uint heartRateZ2 = 0x56c151;
uint heartRateZ3 = 0xffce20;
uint heartRateZ4 = 0xff652d;
uint heartRateZ5 = 0xff2f00;

void initLedPin(uint pin, pwm_config *config) {
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);
  pwm_init(slice_num, config, true);
}

void initLed(RgbLed led, pwm_config *config) {
  initLedPin(led.r, config);
  initLedPin(led.g, config);
  initLedPin(led.b, config);
}

void setLedRgb(RgbLed led, uint rgb) {
  pwm_set_gpio_level(led.r, ((rgb >> 16) & 0xFF) * 200);
  pwm_set_gpio_level(led.g, ((rgb >> 8) & 0xFF) * 120);
  pwm_set_gpio_level(led.b, (rgb & 0xFF) * 64);
}

typedef struct TCP_SERVER_T_ {
  struct tcp_pcb *server_pcb;
  struct tcp_pcb *client_pcb;
  bool complete;
  uint8_t buffer_sent[BUF_SIZE];
  uint8_t buffer_recv[BUF_SIZE];
  int sent_len;
  int recv_len;
  int run_count;
} TCP_SERVER_T;

TCP_SERVER_T *tcp_server_init(void) {
  TCP_SERVER_T *state = {};
  if (!state) {
    printf("failed to allocate state\n");
    return NULL;
  }
  return state;
}

int main() {
  stdio_init_all();
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 4.f);
  initLed(heartRateLed, &config);
  initLed(powerLed, &config);
  initLed(combinedLed, &config);
  initLed(statusLed, &config);

  setLedRgb(statusLed, 0x0000FF);

  if (cyw43_arch_init()) {
    printf("failed to initialize\n");
    setLedRgb(statusLed, 0xFFFF00);
    return 1;
  }

  cyw43_arch_enable_sta_mode();

  printf("Connecting to Wi-Fi...\n");
  if (cyw43_arch_wifi_connect_timeout_ms("Slow", "orangemesa865",
                                         CYW43_AUTH_WPA2_AES_PSK, 30000)) {
    printf("failed to connect.\n");
    setLedRgb(statusLed, 0xFF0000);
    return 1;
  } else {
    printf("Connected.\n");
    setLedRgb(statusLed, 0x00FF00);
  }

  TCP_SERVER_T *state = tcp_server_init();
  if (!state) {
    return;
  }
  if (!tcp_server_open(state)) {
    tcp_server_result(state, -1);
    return;
  }
  while (!state->complete) {
    sleep_ms(1000);
  }
  free(state);
}