#include "ZwiftServer.h"

ZwiftServer::ZwiftServer(ZwiftData *zwiftData, FanController *fanController, RgbLed *statusLed) {
    _zwiftData = zwiftData;
    _fanController = fanController;
    _statusLed = statusLed;
}

int ZwiftServer::init() {
    _statusLed->setLedRgb(0x0000FF);
    if (cyw43_arch_init()) {
        printf("failed to initialize\n");
        _statusLed->setLedRgb(0xFFFF00);
        return 1;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("Slow", "orangemesa865",
                                           CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        _statusLed->setLedRgb(0xFF0000);
        return 1;
    } else {
        printf("Connected.\n");
        _statusLed->setLedRgb(0x00FF00);
    }

    _state = (TCP_SERVER_T *)calloc(1, sizeof(TCP_SERVER_T));
    if (!_state || !tcp_server_open(_state)) {
        printf("failed to allocate state\n");
        return 1;
    }
    return 0;
}

err_t ZwiftServer::tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    return ERR_OK;
}

err_t ZwiftServer::tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        ZwiftServer *ptr = (ZwiftServer *)arg;
        pbuf_copy_partial(p, (void *)ptr->_zwiftData, sizeof(ZwiftData), 0);
        ptr->_fanController->updateFan();
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);
    return ERR_OK;
}

void ZwiftServer::tcp_server_err(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        printf("tcp_client_err_fn %d\n", err);
    }
}

err_t ZwiftServer::tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    if (err != ERR_OK || client_pcb == NULL) {
        printf("Failure in accept\n");
        return ERR_VAL;
    }
    printf("Client connected\n");

    state->client_pcb = client_pcb;
    tcp_arg(client_pcb, state);
    tcp_sent(client_pcb, tcp_server_sent);
    tcp_recv(client_pcb, tcp_server_recv);
    tcp_err(client_pcb, tcp_server_err);
    return ERR_OK;
}

bool ZwiftServer::tcp_server_open(void *arg) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    printf("Starting server at %s on port %u\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), TCP_PORT);

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        printf("failed to create pcb\n");
        return false;
    }

    err_t err = tcp_bind(pcb, NULL, TCP_PORT);
    if (err) {
        printf("failed to bind to port %u\n", TCP_PORT);
        return false;
    }

    state->server_pcb = tcp_listen_with_backlog(pcb, 1);
    if (!state->server_pcb) {
        printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

    tcp_arg(state->server_pcb, this);
    tcp_accept(state->server_pcb, tcp_server_accept);

    return true;
}

