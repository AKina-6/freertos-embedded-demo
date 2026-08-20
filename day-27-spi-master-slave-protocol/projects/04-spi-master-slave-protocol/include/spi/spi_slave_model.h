#ifndef SPI_SLAVE_MODEL_H
#define SPI_SLAVE_MODEL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "spi/spi_protocol.h"

typedef struct {
    uint8_t config_value;
    uint16_t sensor_value;
    uint32_t request_count;
    uint32_t bad_command_count;
} spi_slave_model_t;

void spi_slave_model_init(spi_slave_model_t *slave);
spi_status_t spi_slave_model_handle(spi_slave_model_t *slave, const spi_packet_t *request, spi_packet_t *response);
#endif
