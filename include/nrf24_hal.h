//
// Created by imi415 on 19/04/17.
//

#ifndef NRF24_NRF24_HAL_H
#define NRF24_NRF24_HAL_H

#include "nrf24_conf.h"

uint8_t nrf24_spi_init(nrf24_hal_t *hal);
uint8_t nrf24_spi_rw(nrf24_hal_t *hal, uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len);
uint8_t nrf24_write_ce(nrf24_hal_t *hal, uint8_t enabled);
uint8_t nrf24_irq_init(nrf24_hal_t *hal, void *callback, void *arg);
uint8_t nrf24_irq_deinit(nrf24_hal_t *hal);
uint8_t nrf24_spi_deinit(nrf24_hal_t *hal);
uint8_t nrf24_gpio_init(nrf24_hal_t *hal);
uint8_t nrf24_gpio_deinit(nrf24_hal_t *hal);
uint64_t nrf24_ms_tick(nrf24_hal_t *hal);

#endif //NRF24_NRF24_HAL_H
