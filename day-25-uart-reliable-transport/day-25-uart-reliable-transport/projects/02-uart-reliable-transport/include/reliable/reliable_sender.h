#ifndef RELIABLE_SENDER_H
#define RELIABLE_SENDER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "reliable/reliable_transport.h"
typedef struct { void *context; bool (*send_bytes)(void *,const uint8_t *,size_t); } reliable_link_t;
typedef struct { reliable_transport_t transport; reliable_link_t link; uint8_t frame[40]; size_t frame_length; } reliable_sender_t;
bool reliable_sender_init(reliable_sender_t *s,const reliable_link_t *link,uint32_t timeout_ms,uint8_t max_retries);
bool reliable_sender_start(reliable_sender_t *s,const uint8_t *frame,size_t len,uint8_t sequence,uint8_t command,uint32_t now_ms);
bool reliable_sender_poll(reliable_sender_t *s,uint32_t now_ms);
bool reliable_sender_on_ack(reliable_sender_t *s,uint8_t ack_sequence);
#endif
