//
// Created by imi415 on 19/04/24.
//

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nrf24_api.h"

uint8_t nrf24_init(nrf_handle_t *nrf) {
    nrf->drv.status = N_STAT_UNDEFINED;
    if(nrf24_driver_init(&nrf->drv) == NRF_OK) {
        nrf->drv.status = N_STAT_POWER_DOWN;
        nrf24_flush_tx(&nrf->drv);
        nrf24_flush_rx(&nrf->drv);
    }
    else {
        return NRF_FAIL;
    }
    return NRF_OK;
}

uint8_t nrf24_config(nrf_handle_t *nrf) {

    uint8_t reg[6];
    // Mode
    nrf24_read_register(&nrf->drv, N_CONFIG, reg, 0x01);
    if(nrf->config.mode == N_MODE_RX) {
        reg[0] |= 0x01U;
    }
    else {
        reg[0] &= 0xFEU;
    }
    nrf24_write_register(&nrf->drv, N_CONFIG, reg, 0x01);

    // Clear IRQ status.
    reg[0] = 0x70U;
    nrf24_write_register(&nrf->drv, N_STATUS, reg, 0x01);

    memcpy(reg, nrf->config.tx_addr, 0x05);
    nrf24_write_register(&nrf->drv, N_TX_ADDR, reg, 0x05);
    nrf24_write_register(&nrf->drv, N_RX_ADDR_P(0), reg, 0x05);
    memcpy(reg, nrf->config.rx_addr, 0x05);
    nrf24_write_register(&nrf->drv, N_RX_ADDR_P(1), reg, 0x05);
    reg[0] = nrf->config.packet_size & 0x3FU;
    nrf24_write_register(&nrf->drv, N_RX_PW_P(1), reg, 0x01);
    reg[0] = nrf->config.channel & 0x7FU;
    nrf24_write_register(&nrf->drv, N_RF_CH, reg, 0x01);
}

uint8_t nrf24_power(nrf_handle_t *nrf, uint8_t status) {
    uint8_t reg = 0x00;
    uint8_t ret;
    uint8_t stat = nrf->drv.status;
    nrf24_read_register(&nrf->drv, N_CONFIG, &reg, 0x01);
    if(status) {
        if(stat == N_STAT_POWER_DOWN) {
            stat = N_STAT_STANDBY_I;
        }
        reg |= 0x02U;
    }
    else {
        stat = N_STAT_POWER_DOWN;
        reg &= 0xFDU;
    }
    if((ret = nrf24_write_register(&nrf->drv, N_CONFIG, &reg, 0x01)) == NRF_OK) {
        nrf->drv.status = stat;
    }
    return ret;
}

uint8_t nrf24_send(nrf_handle_t *nrf, uint8_t *packet, uint32_t timeout) {
    uint8_t ret = NRF_OK;
    nrf24_write_payload(&nrf->drv, packet, nrf->config.packet_size);
    nrf->drv.status = N_STAT_TX;

    nrf24_enable(&nrf->drv, true);
    usleep(10); // A high pulse, said by Nordic...
    nrf24_enable(&nrf->drv, false);

    uint64_t tick_start = nrf24_get_tick(&nrf->drv);

    while(nrf->drv.status == N_STAT_TX) {
        if(nrf24_get_tick(&nrf->drv) - tick_start > timeout) {
            nrf->drv.status = N_STAT_STANDBY_I;
            nrf->drv.error = N_ERROR_TIMEOUT;
            break;
        }
        usleep(5); // Wait for the interrupt.
    }

    // See if we got a positive status.
    switch(nrf->drv.error) {
        case N_ERROR_NONE:
            printf("Successfully sent!\r\n");
            break;
        case N_ERROR_TX:
            ret = NRF_FAIL;
            printf("TX Error.\r\n");
            break;
        case N_ERROR_TIMEOUT:
            ret = NRF_FAIL;
            printf("TX Timed Out.\r\n");
            break;
        default:
            ret = NRF_FAIL;
            printf("Whoops! What is that?\r\n");
            break;
    }
    return ret;
}

uint8_t nrf24_receive(nrf_handle_t *nrf, uint8_t *packet, uint32_t timeout) {
    uint8_t ret = NRF_OK;
    nrf24_enable(&nrf->drv, true);
    nrf->drv.status = N_STAT_RX;

    uint64_t tick_start = nrf24_get_tick(&nrf->drv);

    while(nrf->drv.status == N_STAT_RX) {
        if(nrf24_get_tick(&nrf->drv) - tick_start > timeout) {
            nrf->drv.status = N_STAT_STANDBY_I;
            nrf->drv.error = N_ERROR_TIMEOUT;
            break;
        }
        usleep(5);
    }
    nrf24_enable(&nrf->drv, false);
    switch(nrf->drv.error) {
        case N_ERROR_NONE:
            printf("RX!\r\n");
            break;
        case N_ERROR_TIMEOUT:
            printf("RX Timed Out.\r\n");
            ret = NRF_FAIL;
            break;
        default:
            ret = NRF_FAIL;
            printf("Whoops! What is that?\r\n");
    }
    nrf24_read_payload(&nrf->drv, packet, nrf->config.packet_size);
    return ret;
}

uint8_t nrf24_deinit(nrf_handle_t *nrf) {
    nrf->drv.status = N_STAT_UNDEFINED;
    return nrf24_driver_deinit(&nrf->drv);
}