// See LICENSE for license details.

#ifndef _SIFIVE_I2C_H
#define _SIFIVE_I2C_H


// CHECK: word vs byte alignment
// OC I2C Linux driver defines registers w/o alignment:
// http://lxr.free-electrons.com/source/drivers/i2c/busses/i2c-ocores.c?v=4.6#L46
//
// However, register accessor functions use reg_shift value
// http://lxr.free-electrons.com/source/drivers/i2c/busses/i2c-ocores.c?v=4.6#L80
// which is platform specific:
// http://lxr.free-electrons.com/source/drivers/i2c/busses/i2c-ocores.c?v=4.6#L346

#define I2C_PRESCALER_LO  (0x00)
#define I2C_PRESCALER_HI  (0x04)
#define I2C_CONTROL       (0x08)
#define I2C_DATA          (0x0C)
#define I2C_CMD           (0x10) /* write only */
#define I2C_STATUS        (0x10) /* read only, same address as I2C_CMD */

// I2C_CONTROL register
#define I2C_CONTROL_CORE_EN(x)  (((x) & 0x1) << 7)
#define I2C_CONTROL_INT_EN(x)   (((x) & 0x1) << 6)

// I2C_CMD register
#define I2C_CMD_START(x)              (((x) & 0x1) << 7)
#define I2C_CMD_STOP(x)               (((x) & 0x1) << 6)
#define I2C_CMD_READ(x)               (((x) & 0x1) << 5)
#define I2C_CMD_WRITE(x)              (((x) & 0x1) << 4)
#define I2C_CMD_ACK(x)                (((x) & 0x1) << 3)
#define I2C_CMD_IRQ_ACK(x)            (((x) & 0x1))

// I2C_STATUS register
#define I2C_STATUS_RECEIVED_ACK(x)    (((x) & 0x80) >> 7)
#define I2C_STATUS_BUSY(x)            (((x) & 0x40) >> 6)
#define I2C_STATUS_ARB_LOST(x)        (((x) & 0x20) >> 5)
#define I2C_STATUS_TRS_INPROGRESS(x)  (((x) & 0x02) >> 1)
#define I2C_STATUS_IRG_FLAG(x)        (((x) & 0x01))


#endif /* _SIFIVE_I2C_H */
