//
// Created by imi415 on 19/04/17.
//

#ifndef NRF24_NRF24_CONF_H
#define NRF24_NRF24_CONF_H

#if HAL_DRIVER == LINUX

#include <pthread.h>
#include <gpiod.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

typedef struct {
    int spi_fd;
    int spi_speed;
    pthread_t irq_id;
    void *irq_callback;
    void *irq_cb_arg;
} nrf24_hal_t;

#define SPIDEV_CHARDEV "/dev/spidev0.0"
#define SPIDEV_MAX_SPEED 5000000

#define GPIOD_CONSUMER "nRF24"

#if 1

#define GPIO_CE_CHARDEV "/dev/gpiochip1"
#define GPIO_CE_OFFSET 230 // PH6

#define GPIO_IRQ_CHARDEV "/dev/gpiochip1"
#define GPIO_IRQ_OFFSET 229 // PH5

#else

#define GPIO_CE_CHARDEV "/dev/gpiochip0"
#define GPIO_CE_OFFSET 19 // PA19

#define GPIO_IRQ_CHARDEV "/dev/gpiochip0"
#define GPIO_IRQ_OFFSET 7 // PA7

#endif

int _nrf24_irq_event_callback(int event_type, unsigned int offset, const struct timespec * ts, void *data);

#endif

#endif //NRF24_NRF24_CONF_H
