#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#include "nrf24_api.h"

static const char *const shortopts = "+hvm:s:f:tr";

static const struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"mode", required_argument, NULL, 'm'},
        {"tx-address", required_argument, NULL, 's'},
        {"rx-address", required_argument, NULL, 'f'},
        {"tx", no_argument, NULL, 't'},
        {"rx", no_argument, NULL, 'r'}
};

nrf_handle_t nrf;

int help(void);
int version(void);
int set_mode(const char *mode);
int set_tx_addr(const char *addr);
int unused_t(void);
int unused_r(void);

int main(int argc, char *argv[]) {
    int optc, opti;
    for(;;) {
        optc = getopt_long(argc, argv, shortopts, longopts, &opti);
        if(optc < 0) break;
        switch(optc) {
            case 'h':
                help();
                break;
            case 'v':
                version();
                break;
            case 't':
                unused_t();
                break;
            case 'r':
                unused_r();
                break;
            case 'm':
                set_mode(optarg);
                break;
            case 's':
                set_tx_addr(optarg);
            default:
                break;
        }
    }
    return 0;
}



int help(void) {
    printf("This is help text.\r\n");
    nrf24_ms_tick(NULL);
    return 0;
}

int version(void) {
    printf("This is version.\r\n");
    return 0;
}

int set_mode(const char *mode) {
    if(strcmp(mode, "tx") == 0) {
        nrf.config.mode = N_MODE_TX;
    }
    else if(strcmp(mode, "rx") == 0) {
        nrf.config.mode = N_MODE_RX;
    }
    else {
        printf("Invalid mode %s.\r\n", mode);
    }
    return 0;
}

int set_tx_addr(const char *addr) {
    
    printf("TXAddr: %s", addr);
    return 0;
}

int set_rx_addr(char *addr) {
    return 0;
}

int unused_t(void) {
    nrf24_init(&nrf);
    nrf24_power(&nrf, true);
    nrf.config.packet_size = 32;
    nrf.config.tx_addr[0] = 0x0A;
    nrf.config.tx_addr[1] = 0x0B;
    nrf.config.tx_addr[2] = 0x0C;
    nrf.config.tx_addr[3] = 0x0D;
    nrf.config.tx_addr[4] = 0x0E;
    nrf.config.rx_addr[0] = 0x0A;
    nrf.config.rx_addr[1] = 0x0B;
    nrf.config.rx_addr[2] = 0x0C;
    nrf.config.rx_addr[3] = 0x0D;
    nrf.config.rx_addr[4] = 0x0F;
    nrf.config.mode = N_MODE_TX;
    nrf.config.channel = 10;
    nrf24_config(&nrf);
    uint8_t payload[32] = "This is PTX test payload";
    printf("Enter send mode.\r\n");
    if(nrf24_send(&nrf, payload, 1000) == NRF_OK) {
        printf("Send succeeded. Enter receive mode.\r\n");
    }
    nrf.config.mode = N_MODE_RX;
    nrf24_config(&nrf);
    if(nrf24_receive(&nrf, payload, 30000) == NRF_OK) {
        printf("Receive succeeded, payload: %s\r\n", payload);
    }
    int ret = nrf24_deinit(&nrf);
    return ret;
}

int unused_r(void) {
    nrf24_init(&nrf);
    nrf24_power(&nrf, true);
    nrf.config.packet_size = 32;
    nrf.config.tx_addr[0] = 0x0A;
    nrf.config.tx_addr[1] = 0x0B;
    nrf.config.tx_addr[2] = 0x0C;
    nrf.config.tx_addr[3] = 0x0D;
    nrf.config.tx_addr[4] = 0x0F;
    nrf.config.rx_addr[0] = 0x0A;
    nrf.config.rx_addr[1] = 0x0B;
    nrf.config.rx_addr[2] = 0x0C;
    nrf.config.rx_addr[3] = 0x0D;
    nrf.config.rx_addr[4] = 0x0E;
    nrf.config.mode = N_MODE_RX;
    nrf.config.channel = 10;
    nrf24_config(&nrf);
    uint8_t payload[32] = {0x00};
    printf("Enter receive mode.\r\n");
    if(nrf24_receive(&nrf, payload, 30000) == NRF_OK) {
        printf("Receive succeeded, payload: %s. Enter TX mode.\r\n", payload);
    }
    nrf.config.mode = N_MODE_TX;
    nrf24_config(&nrf);
    usleep(500);
    snprintf(payload, 32, "This is PRX test payload");
    if(nrf24_send(&nrf, payload, 1000) == NRF_OK) {
        printf("Send succeed.\r\n");
    }
    int ret = nrf24_deinit(&nrf);
    return ret;
}