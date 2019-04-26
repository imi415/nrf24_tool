#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#include "nrf24_api.h"

static const char *const shortopts = "+hv";

static const struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'}
};

nrf_handle_t nrf;

int help(void);
int version(void);

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
            default:
                break;
        }
    }
    return 0;
}

int help(void) {
    printf("This is help text.\r\n");
    nrf24_get_tick(NULL);
    return 0;
}

int version(void) {
    printf("This is version.\r\n");
    return 0;
}

int unused(void) {
    nrf24_init(&nrf);
    nrf24_power(&nrf, true);
    nrf24_config(&nrf);
    int ret = nrf24_deinit(&nrf);
    return ret;
}