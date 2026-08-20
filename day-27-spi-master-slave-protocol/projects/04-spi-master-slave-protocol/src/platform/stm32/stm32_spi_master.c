#include "spi/spi_transaction.h"
#include "stm32f1xx_hal.h"
extern SPI_HandleTypeDef hspi1;

#define SPI_CS_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_PIN_0

static bool stm32_cs_assert(void *context)
{
    (void)context; HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET); return true;
}
static void stm32_cs_deassert(void *context)
{
    (void)context; HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);
}
static bool stm32_transfer(void *context,const uint8_t *tx,uint8_t *rx,size_t length,uint32_t timeout_ms)
{
    SPI_HandleTypeDef *h=(SPI_HandleTypeDef*)context;
    return HAL_SPI_TransmitReceive(h,(uint8_t*)tx,rx,(uint16_t)length,timeout_ms)==HAL_OK;
}

void spi_master_app_init(spi_master_t *master)
{
    const spi_bus_t bus={&hspi1,stm32_cs_assert,stm32_transfer,stm32_cs_deassert};
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);
    (void)spi_master_init(master,&bus);
}
