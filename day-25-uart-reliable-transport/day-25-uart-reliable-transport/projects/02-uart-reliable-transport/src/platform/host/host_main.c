#include <stdio.h>
#include "reliable/reliable_sender.h"
#include "host_link.h"
static void p(const char *label,const reliable_sender_t *s,const host_link_t *h){
 printf("%s state=%s seq=%u retries=%u tx=%lu timeouts=%lu acks=%lu ignored=%lu link_sends=%lu\n",label,reliable_state_name(s->transport.state),s->transport.sequence,s->transport.retry_count,(unsigned long)s->transport.tx_count,(unsigned long)s->transport.timeout_count,(unsigned long)s->transport.ack_count,(unsigned long)s->transport.ignored_ack_count,(unsigned long)h->send_count);
}
int main(void){
 host_link_t h={0}; reliable_link_t link={&h,host_link_send}; reliable_sender_t s; const uint8_t frame[]={0xA5,0x10,0x2A,0x01,0x55,0x99};
 if(!reliable_sender_init(&s,&link,100U,2U))return 1;
 reliable_sender_start(&s,frame,sizeof(frame),0x2A,0x10,0U); p("start",&s,&h);
 reliable_sender_poll(&s,100U); p("timeout_retry",&s,&h);
 reliable_sender_on_ack(&s,0x2AU); p("ack_ok",&s,&h);
 reliable_sender_on_ack(&s,0x2AU); p("duplicate_ack",&s,&h);
 reliable_sender_init(&s,&link,50U,2U); reliable_sender_start(&s,frame,sizeof(frame),0x2B,0x10,1000U);
 reliable_sender_poll(&s,1050U); reliable_sender_poll(&s,1100U); reliable_sender_poll(&s,1150U); p("retry_exhausted",&s,&h); return 0;
}
