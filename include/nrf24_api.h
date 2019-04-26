//
// Created by imi415 on 19/04/24.
//

#ifndef NRF24_NRF24_API_H
#define NRF24_NRF24_API_H

#include "nrf24_types.h"
#include "nrf24_driver.h"
#include "nrf24_regs.h"

typedef struct {
    nrf_config_t config;
    nrf24_drv_t drv;
} nrf_handle_t;

uint8_t nrf24_init(nrf_handle_t *nrf);
uint8_t nrf24_deinit(nrf_handle_t *nrf);

uint8_t nrf24_power(nrf_handle_t *nrf, uint8_t status);
uint8_t nrf24_send(nrf_handle_t *nrf, uint8_t *packet, uint32_t timeout);
uint8_t nrf24_receive(nrf_handle_t *nrf, uint8_t *packet, uint32_t timeout);

uint8_t nrf24_config(nrf_handle_t *nrf);

#endif //NRF24_NRF24_API_H
