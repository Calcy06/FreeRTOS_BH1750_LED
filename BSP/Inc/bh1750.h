#ifndef BH1750_H_
#define BH1750_H_

#define BH1750_ADDR_H 0x5c
#define BH1750_ADDR_L 0x23
#define BH1750_GOUND_ADDR_R ((BH1750_ADDR_L << 1) | 1)
#define BH1750_GOUND_ADDR_W ((BH1750_ADDR_L << 1) | 0)

typedef struct bh1750_dev
{
    char name[10];
    I2C_HandleTypeDef *i2c_handle;
    uint8_t addr_r;
    uint8_t addr_w;
    volatile uint16_t value;
    uint8_t buff[2];

} bh1750_dev_t;

void BH1750_Init(void);

extern HAL_StatusTypeDef BH1750_get_lumen(bh1750_dev_t *dev);

extern bh1750_dev_t bh1750_dev;

#endif
