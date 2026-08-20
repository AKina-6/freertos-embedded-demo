#include "spi/spi_slave_model.h"
#include <stddef.h>

void spi_slave_model_init(spi_slave_model_t *slave)
{
    if (!slave) return;
    slave->config_value=0U; slave->sensor_value=0x1234U; slave->request_count=0U; slave->bad_command_count=0U;
}

spi_status_t spi_slave_model_handle(spi_slave_model_t *slave, const spi_packet_t *request, spi_packet_t *response)
{
    if (!slave || !request || !response) return SPI_STATUS_BAD_CMD;
    slave->request_count++;
    response->command = request->command;
    response->sequence = request->sequence;
    response->length = 1U;
    response->payload[0] = SPI_STATUS_OK;

    switch (request->command) {
        case SPI_CMD_NOP:
            return SPI_STATUS_OK;
        case SPI_CMD_READ_STATUS:
            response->length=2U; response->payload[0]=SPI_STATUS_OK; response->payload[1]=slave->config_value; return SPI_STATUS_OK;
        case SPI_CMD_WRITE_CONFIG:
            if (request->length != 1U) { response->payload[0]=SPI_STATUS_BAD_LENGTH; return SPI_STATUS_BAD_LENGTH; }
            slave->config_value=request->payload[0]; return SPI_STATUS_OK;
        case SPI_CMD_READ_DATA:
            response->length=3U; response->payload[0]=SPI_STATUS_OK;
            response->payload[1]=(uint8_t)(slave->sensor_value>>8U);
            response->payload[2]=(uint8_t)(slave->sensor_value&0xFFU);
            return SPI_STATUS_OK;
        default:
            slave->bad_command_count++; response->payload[0]=SPI_STATUS_BAD_CMD; return SPI_STATUS_BAD_CMD;
    }
}
