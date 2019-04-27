//
// Created by imi415 on 19/04/17.
//

#ifndef NRF24_NRF24_DRIVER_H
#define NRF24_NRF24_DRIVER_H

#include "nrf24_hal.h"

typedef struct {
    nrf24_hal_t hal;
    nrf_status_t status;
    nrf_error_t error;
    uint8_t rx_pipe;
} nrf24_drv_t;

uint8_t nrf24_driver_init(nrf24_drv_t *drv);
uint8_t nrf24_write_register(nrf24_drv_t *drv, uint8_t reg, uint8_t *value, uint8_t len);
uint8_t nrf24_read_register(nrf24_drv_t *drv, uint8_t reg, uint8_t *value, uint8_t len);
uint8_t nrf24_read_payload(nrf24_drv_t *drv, uint8_t *payload, uint8_t len);
uint8_t nrf24_write_payload(nrf24_drv_t *drv, uint8_t *payload, uint8_t len);
uint8_t nrf24_flush_tx(nrf24_drv_t *drv);
uint8_t nrf24_flush_rx(nrf24_drv_t *drv);
uint8_t nrf24_enable(nrf24_drv_t *drv, uint8_t enabled);
uint64_t nrf24_get_tick(nrf24_drv_t *drv);
uint8_t nrf24_irq_callback(void *arg);
uint8_t nrf24_driver_deinit(nrf24_drv_t *drv);

#endif //NRF24_NRF24_DRIVER_H
