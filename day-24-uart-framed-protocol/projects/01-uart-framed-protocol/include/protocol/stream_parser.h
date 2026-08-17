#ifndef STREAM_PARSER_H
#define STREAM_PARSER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "protocol/protocol_frame.h"
typedef enum { PARSER_WAIT_SOF=0,PARSER_READ_HEADER,PARSER_READ_PAYLOAD,PARSER_READ_CRC } parser_state_t;
typedef struct { parser_state_t state; uint8_t buffer[PROTOCOL_MAX_FRAME_SIZE]; size_t index,expected_total; uint32_t frames_ok,crc_errors,length_errors,resync_count; } stream_parser_t;
void stream_parser_init(stream_parser_t *parser);
bool stream_parser_feed_byte(stream_parser_t *parser,uint8_t byte,protocol_frame_t *frame_out);
size_t stream_parser_feed(stream_parser_t *parser,const uint8_t *data,size_t length,protocol_frame_t *frames,size_t frame_capacity);
#endif
