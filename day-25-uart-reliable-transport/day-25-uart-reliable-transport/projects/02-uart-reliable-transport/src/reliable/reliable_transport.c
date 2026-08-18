#include "reliable/reliable_transport.h"
#include <stddef.h>
void reliable_transport_init(reliable_transport_t *t,uint32_t timeout_ms,uint8_t max_retries){
 if(!t)return; t->sequence=0U;t->command=0U;t->retry_count=0U;t->max_retries=max_retries;t->timeout_ms=timeout_ms;t->deadline_ms=0U;t->tx_count=0U;t->timeout_count=0U;t->ack_count=0U;t->ignored_ack_count=0U;t->state=RELIABLE_IDLE;
}
bool reliable_transport_start(reliable_transport_t *t,uint8_t sequence,uint8_t command,uint32_t now_ms){
 if(!t||t->state==RELIABLE_WAIT_ACK||t->timeout_ms==0U)return false;
 t->sequence=sequence;t->command=command;t->retry_count=0U;t->state=RELIABLE_WAIT_ACK;t->deadline_ms=now_ms;return true;
}
bool reliable_transport_on_ack(reliable_transport_t *t,uint8_t ack_sequence){
 if(!t)return false;
 if(t->state!=RELIABLE_WAIT_ACK||ack_sequence!=t->sequence){t->ignored_ack_count++;return false;}
 t->ack_count++;t->state=RELIABLE_SUCCESS;return true;
}
void reliable_transport_mark_transmitted(reliable_transport_t *t,uint32_t now_ms){
 if(!t||t->state!=RELIABLE_WAIT_ACK)return; t->tx_count++;t->deadline_ms=now_ms+t->timeout_ms;
}
const char *reliable_state_name(reliable_state_t s){switch(s){case RELIABLE_IDLE:return "IDLE";case RELIABLE_WAIT_ACK:return "WAIT_ACK";case RELIABLE_SUCCESS:return "SUCCESS";case RELIABLE_FAILED:return "FAILED";default:return "UNKNOWN";}}
