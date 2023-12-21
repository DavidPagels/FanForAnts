#ifndef ZwiftServer_h
#define ZwiftServer_h

#include <stdio.h>

#include "FanController.h"
#include "RgbLed.h"
#include "Shared.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "pico/cyw43_arch.h"

#define TCP_PORT 9090
#define BUF_SIZE 2048
#define TEST_ITERATIONS 10
#define POLL_TIME_S 5

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

class ZwiftServer {
 public:
  ZwiftServer(ZwiftData *zwiftData, FanController *fanController,
              RgbLed *statusLed);
  int init();

 private:
  bool tcp_server_open();
  static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb,
                                 err_t err);
  static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
                               err_t err);
  static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
  static void tcp_server_err(void *arg, err_t err);
  TCP_SERVER_T *_state;
  ZwiftData *_zwiftData;
  FanController *_fanController;
  RgbLed *_statusLed;
};

#endif
