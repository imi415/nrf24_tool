//
// Created by imi415 on 19/04/17.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "nrf24_types.h"
#include "nrf24_regs.h"
#include "nrf24_hal.h"

#if HAL_DRIVER == LINUX


uint8_t nrf24_spi_init(nrf24_hal_t *hal) {
    hal->spi_speed = SPIDEV_MAX_SPEED;
    if((hal->spi_fd = open(SPIDEV_CHARDEV, O_RDWR)) == -1) {
        int f_err = errno;
        printf("[HAL] Init failed: %s.\r\n", strerror(f_err));
        return NRF_FAIL;
    }
    if(ioctl(hal->spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &hal->spi_speed) < 0) {
        int f_err = errno;
        printf("[HAL] Speed set failed: %s.\r\n", strerror(f_err));
        return NRF_FAIL;
    }
    return NRF_OK;
}

 uint8_t nrf24_spi_rw(nrf24_hal_t *hal, uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len) {
    struct spi_ioc_transfer spi_transfer;

    memset(&spi_transfer, 0, sizeof(spi_transfer));
    memset(rx_buf, 0, sizeof(rx_buf));

    spi_transfer.tx_buf = (unsigned long)tx_buf;
    spi_transfer.rx_buf = (unsigned long)rx_buf;
    spi_transfer.len = len;

    if(ioctl(hal->spi_fd, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        int f_err = errno;
        printf("[HAL] Transfer failed: %s.\r\n", strerror(f_err));
        return NRF_FAIL;
    }
    return NRF_OK;

}

uint8_t nrf24_spi_deinit(nrf24_hal_t *hal) {
    if(close(hal->spi_fd) == -1) {
        int f_err = errno;
        printf("[HAL] Close failed: %s.\r\n", strerror(f_err));
        return NRF_FAIL;
    }
    return NRF_OK;
}

uint8_t nrf24_gpio_init(nrf24_hal_t *hal) {
    gpiod_ctxless_set_value(GPIO_CE_CHARDEV, GPIO_CE_OFFSET,
            0, false, GPIOD_CONSUMER, NULL, NULL);
    return NRF_OK;
}

uint8_t nrf24_gpio_deinit(nrf24_hal_t *hal) {
    gpiod_ctxless_get_value(GPIO_CE_CHARDEV, GPIO_CE_OFFSET, false, GPIOD_CONSUMER);
    return NRF_OK;
}

uint8_t nrf24_write_ce(nrf24_hal_t *hal, uint8_t enabled) {
    gpiod_ctxless_set_value(GPIO_CE_CHARDEV, GPIO_CE_OFFSET,
            enabled, false, GPIOD_CONSUMER, NULL, NULL);
    return NRF_OK;
}

void* nrf24_irq_thread(void *arg) {
    nrf24_hal_t *hal = arg;
    gpiod_ctxless_event_monitor(GPIO_IRQ_CHARDEV, GPIOD_CTXLESS_EVENT_FALLING_EDGE,
            GPIO_IRQ_OFFSET, false, GPIOD_CONSUMER, NULL, NULL, _nrf24_irq_event_callback, hal);
    printf("[HAL] Monitor returned.\r\n");
    return hal;
}

int _nrf24_irq_event_callback(int event_type, unsigned int offset, const struct timespec * ts, void *data) {
    nrf24_hal_t *hal = data;
    void (*cb)(void *) = hal->irq_callback;
    cb(hal->irq_cb_arg);
    return GPIOD_CTXLESS_EVENT_CB_RET_OK;
}



uint8_t nrf24_irq_init(nrf24_hal_t *hal, void *callback, void *arg) {
    hal->irq_callback = callback;
    hal->irq_cb_arg = arg;
    pthread_create(&hal->irq_id, NULL, nrf24_irq_thread, hal);
    return NRF_OK;
}

uint8_t nrf24_irq_deinit(nrf24_hal_t *hal) {
    int ret = pthread_cancel(hal->irq_id);
    return ret;
}

uint64_t nrf24_ms_tick(nrf24_hal_t *hal) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return ms;
}

#endif