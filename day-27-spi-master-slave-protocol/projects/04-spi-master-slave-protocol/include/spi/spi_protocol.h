#ifndef SPI_PROTOCOL_H
#define SPI_PROTOCOL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SPI_PROTOCOL_SOF 0x5AU
#define SPI_PROTOCOL_MAX_PAYLOAD 16U
#define SPI_PROTOCOL_MAX_FRAME (5U + SPI_PROTOCOL_MAX_PAYLOAD)

typedef enum {
    SPI_CMD_NOP = 0x00,
    SPI_CMD_READ_STATUS = 0x10,
    SPI_CMD_WRITE_CONFIG = 0x20,
    SPI_CMD_READ_DATA = 0x30
} spi_command_t;

typedef enum {
    SPI_STATUS_OK = 0x00,
    SPI_STATUS_BAD_CMD = 0x01,
    SPI_STATUS_BAD_LENGTH = 0x02,
    SPI_STATUS_CRC_ERROR = 0x03
} spi_status_t;

typedef struct {
    uint8_t command;
    uint8_t sequence;
    uint8_t length;
    uint8_t payload[SPI_PROTOCOL_MAX_PAYLOAD];
} spi_packet_t;

uint8_t spi_protocol_checksum(const uint8_t *data, size_t length);
size_t spi_protocol_encode(const spi_packet_t *packet, uint8_t *out, size_t cap);
bool spi_protocol_decode(const uint8_t *data, size_t length, spi_packet_t *packet);
#endif
