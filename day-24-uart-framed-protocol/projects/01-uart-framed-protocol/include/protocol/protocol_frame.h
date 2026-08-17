#ifndef PROTOCOL_FRAME_H
#define PROTOCOL_FRAME_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define PROTOCOL_SOF 0xA5U
#define PROTOCOL_MAX_PAYLOAD 32U
#define PROTOCOL_HEADER_SIZE 4U
#define PROTOCOL_CRC_SIZE 1U
#define PROTOCOL_MAX_FRAME_SIZE (PROTOCOL_HEADER_SIZE + PROTOCOL_MAX_PAYLOAD + PROTOCOL_CRC_SIZE)
typedef struct { uint8_t command, sequence, length; uint8_t payload[PROTOCOL_MAX_PAYLOAD]; } protocol_frame_t;
uint8_t protocol_crc8(const uint8_t *data,size_t length);
size_t protocol_frame_encode(const protocol_frame_t *frame,uint8_t *output,size_t output_capacity);
bool protocol_frame_decode(const uint8_t *data,size_t length,protocol_frame_t *frame);
#endif
