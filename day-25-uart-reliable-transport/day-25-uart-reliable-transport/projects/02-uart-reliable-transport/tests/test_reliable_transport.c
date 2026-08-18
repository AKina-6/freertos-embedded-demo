#include <assert.h>
#include "reliable/reliable_transport.h"
int main(void){reliable_transport_t t;reliable_transport_init(&t,100U,2U);assert(reliable_transport_start(&t,7U,0x10U,0U));reliable_transport_mark_transmitted(&t,0U);assert(t.deadline_ms==100U);assert(!reliable_transport_on_ack(&t,8U));assert(t.ignored_ack_count==1U);assert(reliable_transport_on_ack(&t,7U));assert(t.state==RELIABLE_SUCCESS);return 0;}
