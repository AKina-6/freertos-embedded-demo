#include "i2c/i2c_bus.h"
#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

static i2c_bus_status_t map_status(HAL_StatusTypeDef s)
{
    if (s == HAL_OK) return I2C_BUS_OK;
    if (s == HAL_TIMEOUT) return I2C_BUS_TIMEOUT;
    if (HAL_I2C_GetError(&hi2c1) & HAL_I2C_ERROR_AF) return I2C_BUS_NACK;
    return I2C_BUS_ERROR;
}

static i2c_bus_status_t stm32_write(void *context, uint8_t addr, const uint8_t *data, size_t len)
{
    I2C_HandleTypeDef *h=(I2C_HandleTypeDef*)context;
    return map_status(HAL_I2C_Master_Transmit(h,(uint16_t)(addr<<1U),(uint8_t*)data,(uint16_t)len,100U));
}

static i2c_bus_status_t stm32_read(void *context, uint8_t addr, uint8_t *data, size_t len)
{
    I2C_HandleTypeDef *h=(I2C_HandleTypeDef*)context;
    return map_status(HAL_I2C_Master_Receive(h,(uint16_t)(addr<<1U),data,(uint16_t)len,100U));
}

static bool stm32_recover(void *context)
{
    I2C_HandleTypeDef *h=(I2C_HandleTypeDef*)context;
    HAL_I2C_DeInit(h);

    /*
     * Board-specific recovery should temporarily configure SCL/SDA as GPIO open-drain:
     * 1. release SDA;
     * 2. toggle SCL up to 9 times;
     * 3. generate STOP: SDA low -> SCL high -> SDA high;
     * 4. restore alternate-function I2C pins;
     * 5. HAL_I2C_Init().
     *
     * GPIO details are intentionally left board-specific.
     */

    return HAL_I2C_Init(h) == HAL_OK;
}

bool stm32_i2c_bus_create(i2c_bus_t *bus)
{
    i2c_bus_ops_t ops = {&hi2c1, stm32_write, stm32_read, stm32_recover};
    return i2c_bus_init(bus,&ops);
}
