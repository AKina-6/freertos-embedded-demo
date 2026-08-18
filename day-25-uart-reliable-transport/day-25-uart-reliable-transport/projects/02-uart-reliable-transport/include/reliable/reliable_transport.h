#ifndef RELIABLE_TRANSPORT_H
#define RELIABLE_TRANSPORT_H
#include <stdbool.h>
#include <stdint.h>
typedef enum { RELIABLE_IDLE=0, RELIABLE_WAIT_ACK, RELIABLE_SUCCESS, RELIABLE_FAILED } reliable_state_t;
typedef struct {
 uint8_t sequence, command, retry_count, max_retries;
 uint32_t timeout_ms, deadline_ms, tx_count, timeout_count, ack_count, ignored_ack_count;
 reliable_state_t state;
} reliable_transport_t;
void reliable_transport_init(reliable_transport_t *t,uint32_t timeout_ms,uint8_t max_retries);
bool reliable_transport_start(reliable_transport_t *t,uint8_t sequence,uint8_t command,uint32_t now_ms);
bool reliable_transport_on_ack(reliable_transport_t *t,uint8_t ack_sequence);
void reliable_transport_mark_transmitted(reliable_transport_t *t,uint32_t now_ms);
const char *reliable_state_name(reliable_state_t state);
#endif
