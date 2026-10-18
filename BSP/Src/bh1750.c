#include <string.h>

#include "main.h"
#include "bh1750.h"

#define CMD_POWER_OFF 0x00  // 断电
#define CMD_POWER_ON 0x01   // 上电（每次测量前必须先 Power On）
#define CMD_RESET 0x03      // 复位数据寄存器
#define CMD_H_RES_MODE 0x10 // 连续高分辨率模式（1 lx 分辨率，测量约 120ms）

extern I2C_HandleTypeDef hi2c3;

bh1750_dev_t bh1750_dev;

// 检查一下i2c地址上是否有设备
static HAL_StatusTypeDef bh1750_isready(bh1750_dev_t *dev)
{
    if (HAL_I2C_IsDeviceReady(dev->i2c_handle, BH1750_GOUND_ADDR_W, 3, 10) != HAL_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

// 初始化bh1750结构体
static void bh1750_dev_init(bh1750_dev_t *dev, I2C_HandleTypeDef *hi2c, char *name)
{
    strcpy(dev->name, name);
    dev->i2c_handle = hi2c;
    dev->addr_r = BH1750_GOUND_ADDR_R;
    dev->addr_w = BH1750_GOUND_ADDR_W;
}

// 发指令
static HAL_StatusTypeDef bh1750_send_command(bh1750_dev_t *dev, uint8_t cmd)
{
    if (HAL_I2C_Master_Transmit(dev->i2c_handle, dev->addr_w, &cmd, 1, 10) != HAL_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

// 开启bh1750传感器
static void bh1750_start(bh1750_dev_t *dev)
{
    // 1.断电
    bh1750_send_command(dev, CMD_POWER_OFF);
    // 2.通电
    bh1750_send_command(dev, CMD_POWER_ON);
    // 3.复位数据
    bh1750_send_command(dev, CMD_RESET);
    // 4.测量指令
    bh1750_send_command(dev, CMD_H_RES_MODE);
}

HAL_StatusTypeDef BH1750_get_lumen(bh1750_dev_t *dev)
{
    if (HAL_I2C_Master_Receive(dev->i2c_handle, dev->addr_r, dev->buff, 2, 10) != HAL_OK)
    {
        return HAL_ERROR;
    }

    dev->value = (dev->buff[0] << 8) | dev->buff[1];
    dev->value /= 1.2;
    return HAL_OK;
}

// 初始化
void BH1750_Init(void)
{
    bh1750_dev_init(&bh1750_dev, &hi2c3, "bh1750");
    if (bh1750_isready(&bh1750_dev) != HAL_OK)
    {
        printf("Device is undefined\r\n");
        return;
    }
    bh1750_start(&bh1750_dev);
}
