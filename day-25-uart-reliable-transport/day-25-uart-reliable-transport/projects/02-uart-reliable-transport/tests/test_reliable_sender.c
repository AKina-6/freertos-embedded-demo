#include <assert.h>
#include "reliable/reliable_sender.h"
#include "host_link.h"
int main(void){host_link_t h={0};reliable_link_t link={&h,host_link_send};reliable_sender_t s;const uint8_t f[]={1,2,3};assert(reliable_sender_init(&s,&link,50U,2U));assert(reliable_sender_start(&s,f,sizeof(f),5U,0x20U,0U));assert(h.send_count==1U);assert(reliable_sender_poll(&s,50U));assert(s.transport.retry_count==1U&&h.send_count==2U);assert(reliable_sender_poll(&s,100U));assert(s.transport.retry_count==2U&&h.send_count==3U);assert(!reliable_sender_poll(&s,150U));assert(s.transport.state==RELIABLE_FAILED);assert(s.transport.timeout_count==3U);return 0;}
