//
// Created by imi415 on 19/04/22.
//

#ifndef NRF24_NRF24_TYPES_H
#define NRF24_NRF24_TYPES_H

#define NRF_OK 0
#define NRF_FAIL 1

typedef enum {
    N_STAT_UNDEFINED = 0x00U,
    N_STAT_POWER_DOWN,
    N_STAT_STANDBY_I,
    N_STAT_STANDBY_II,
    N_STAT_RX,
    N_STAT_TX
} nrf_status_t;

typedef enum {
    N_ERROR_NONE = 0x00U,
    N_ERROR_TX,
    N_ERROR_TIMEOUT
} nrf_error_t;

typedef enum {
    N_SPEED_250K = 0x00U,
    N_SPEED_1M,
    N_SPEED_2M
} nrf_speed_t;

typedef enum {
    N_POWER_M18 = 0x00U,
    N_POWER_M12,
    N_POWER_M6,
    N_POWER_M0
} nrf_power_t;

typedef struct {
    uint8_t channel;
    uint8_t packet_size;
    uint8_t tx_addr[5];
    uint8_t rx_addr[5];
} nrf_config_t;


#endif //NRF24_NRF24_TYPES_H
