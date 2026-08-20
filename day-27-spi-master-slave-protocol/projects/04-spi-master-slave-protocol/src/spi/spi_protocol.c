#include "spi/spi_protocol.h"
#include <string.h>

uint8_t spi_protocol_checksum(const uint8_t *data, size_t length)
{
    uint8_t sum = 0U;
    for (size_t i=0U;i<length;++i) sum = (uint8_t)(sum + data[i]);
    return (uint8_t)(0U - sum);
}

size_t spi_protocol_encode(const spi_packet_t *packet, uint8_t *out, size_t cap)
{
    if (!packet || !out || packet->length > SPI_PROTOCOL_MAX_PAYLOAD) return 0U;
    size_t total = 5U + packet->length;
    if (cap < total) return 0U;
    out[0] = SPI_PROTOCOL_SOF;
    out[1] = packet->command;
    out[2] = packet->sequence;
    out[3] = packet->length;
    if (packet->length) memcpy(&out[4], packet->payload, packet->length);
    out[total-1U] = spi_protocol_checksum(&out[1], total-2U);
    return total;
}

bool spi_protocol_decode(const uint8_t *data, size_t length, spi_packet_t *packet)
{
    if (!data || !packet || length < 5U || data[0] != SPI_PROTOCOL_SOF) return false;
    uint8_t n = data[3];
    if (n > SPI_PROTOCOL_MAX_PAYLOAD || length != (size_t)(5U+n)) return false;
    uint8_t sum = 0U;
    for (size_t i=1U;i<length;++i) sum = (uint8_t)(sum + data[i]);
    if (sum != 0U) return false;
    packet->command=data[1]; packet->sequence=data[2]; packet->length=n;
    if (n) memcpy(packet->payload,&data[4],n);
    return true;
}
