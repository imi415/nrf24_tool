//
// Created by imi415 on 19/04/17.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "nrf24_types.h"
#include "nrf24_regs.h"
#include "nrf24_driver.h"

uint8_t nrf24_driver_init(nrf24_drv_t *drv) {
    uint8_t ret = nrf24_spi_init(&drv->hal);
    if(ret != NRF_OK) {
        printf("Failed to init SPI driver.\r\n");
        return NRF_FAIL;
    }
    ret = nrf24_gpio_init(&drv->hal);
    if(ret != NRF_OK) {
        printf("Failed to init GPIO.\r\n");
        return NRF_FAIL;
    }
    ret = nrf24_irq_init(&drv->hal, nrf24_irq_callback, drv);
    return ret;
}

uint8_t nrf24_write_register(nrf24_drv_t *drv, uint8_t reg, uint8_t *value, uint8_t len) {
    uint8_t tx_buf[6] = {0x00};
    uint8_t rx_buf[6] = {0x00};
    tx_buf[0] = N_CMD_W_REGISTER(reg);
    memcpy(&tx_buf[1], value, len);
    uint8_t ret;
    if((ret = nrf24_spi_rw(&drv->hal, tx_buf, rx_buf, len + 1)) != NRF_OK) {
        printf("Failed to write register.\r\n");
    }
    return ret;
}

uint8_t nrf24_read_register(nrf24_drv_t *drv, uint8_t reg, uint8_t *value, uint8_t len) {
    uint8_t tx_buf[6] = {0x00};
    uint8_t rx_buf[6] = {0x00};
    tx_buf[0] = N_CMD_R_REGISTER(reg);
    uint8_t ret;
    if((ret = nrf24_spi_rw(&drv->hal, tx_buf, rx_buf, len + 1)) != NRF_OK) {
        printf("Failed to read register.\r\n");
    }
    memcpy(value, &rx_buf[1], len);
    return ret;
}

uint8_t nrf24_read_payload(nrf24_drv_t *drv, uint8_t *payload, uint8_t len) {
    uint8_t tx_buf[33] = {0x00}; // Max payload length is 32.
    uint8_t rx_buf[33] = {0x00}; //
    tx_buf[0] = N_CMD_R_RX_PAYLOAD;
    uint8_t ret;
    if((ret = nrf24_spi_rw(&drv->hal, tx_buf, rx_buf, len + 1)) != NRF_OK) {
        printf("Failed to read payload.\r\n");
    }
    memcpy(payload, &rx_buf[1], len);
    return ret;
}

uint8_t nrf24_write_payload(nrf24_drv_t *drv, uint8_t *payload, uint8_t len) {
    uint8_t tx_buf[33] = {0x00};
    uint8_t rx_buf[33] = {0x00};
    tx_buf[0] = N_CMD_W_TX_PAYLOAD;
    memcpy(&tx_buf[1], payload, len);
    uint8_t ret;
    if((ret = nrf24_spi_rw(&drv->hal, tx_buf, rx_buf, len + 1)) != NRF_OK) {
        printf("Failed to write payload.\r\n");
    }
    return ret;
}

uint8_t nrf24_flush_tx(nrf24_drv_t *drv) {
    uint8_t tx_buf[2] = {0xE1, 0x00};
    uint8_t rx_buf[2];
    uint8_t ret;
    if((ret = nrf24_spi_rw(&drv->hal, tx_buf, rx_buf, 0x01)) != NRF_OK) {
        printf("Failed to flush TX FIFO.\r\n");
    }
    return ret;
}

uint8_t nrf24_flush_rx(nrf24_drv_t *drv) {
    uint8_t tx_buf[2] = {0xE2, 0x00};
    uint8_t rx_buf[2];
    uint8_t ret;
    if((ret = nrf24_spi_rw(&drv->hal, tx_buf, rx_buf, 0x01)) != NRF_OK) {
        printf("Failed to flush RX FIFO.\r\n");
    }
    return ret;
}

uint8_t nrf24_enable(nrf24_drv_t *drv, uint8_t enabled) {
    return nrf24_write_ce(&drv->hal, enabled);
}

uint8_t nrf24_irq_callback(void *arg) {
    nrf24_drv_t *drv = arg;
    printf("Interrupt captured. STAT: %d\r\n", drv->status);
    uint8_t chip_status, ret;
    if((ret = nrf24_read_register(drv, N_STATUS, &chip_status, 0x01)) != NRF_OK) {
        return ret;
    }
    switch(drv->status) {
        case N_STAT_TX:
            if((chip_status & 0x20U)) { // Transition: TX_DS
                drv->status = N_STAT_STANDBY_I;
                drv->error = N_ERROR_NONE;
            }
            else if(chip_status & 0x10U) { // MAX_RT
                drv->status = N_STAT_STANDBY_I;
                drv->error = N_ERROR_TX;
            }
            printf("TX event.\r\n");
            break;
        case N_STAT_RX:
            if(chip_status & 0x40U) {
                drv->status = N_STAT_STANDBY_I;
            }
            printf("RX event.\r\n");
            break;
        default:
            printf("Whoops! Undefined event occurred.\r\n");
            break;
    }
    chip_status = 0x70;
    ret = nrf24_write_register(drv, N_STATUS, &chip_status, 0x01);
    return ret;
}

uint8_t nrf24_driver_deinit(nrf24_drv_t *drv) {
    uint8_t ret = nrf24_spi_deinit(&drv->hal);
    if(ret != NRF_OK) {
        printf("Failed to deinit SPI driver.\r\n");
        return NRF_FAIL;
    }
    ret = nrf24_gpio_deinit(&drv->hal);
    if(ret != NRF_OK) {
        printf("Failed to deinit GPIO driver.\r\n");
        return NRF_FAIL;
    }
    ret = nrf24_irq_deinit(&drv->hal);
    return ret;
}