//
// Created by imi415 on 19/04/17.
//

#ifndef NRF24_NRF24_REGS_H
#define NRF24_NRF24_REGS_H

#define N_CONFIG            0x00
#define N_EN_AA             0x01
#define N_EN_RXADDR         0x02
#define N_SETUP_AW          0x03
#define N_SETUP_RETR        0x04
#define N_RF_CH             0x05
#define N_RF_SETUP          0x06
#define N_STATUS            0x07
#define N_OBSERVE_TX        0x08
#define N_RPD               0x09
#define N_RX_ADDR_P(x)      (0x0A + x)
#define N_TX_ADDR           0x10
#define N_RX_PW_P(x)        (0x11 + x)
#define N_FIFO_STATUS       0x17
#define N_DYNPD             0x1C

#define N_CMD_R_REGISTER(reg)   (reg & 0x1FU)
#define N_CMD_W_REGISTER(reg)   ((reg & 0x1FU) | 0x20U)
#define N_CMD_R_RX_PAYLOAD      0x61
#define N_CMD_W_TX_PAYLOAD      0xA0

#endif //NRF24_NRF24_REGS_H
