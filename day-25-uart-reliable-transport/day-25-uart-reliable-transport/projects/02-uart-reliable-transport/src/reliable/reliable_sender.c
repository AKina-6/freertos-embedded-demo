#include "reliable/reliable_sender.h"
#include <string.h>
bool reliable_sender_init(reliable_sender_t *s,const reliable_link_t *link,uint32_t timeout_ms,uint8_t max_retries){
 if(!s||!link||!link->send_bytes||timeout_ms==0U)return false; s->link=*link;s->frame_length=0U;reliable_transport_init(&s->transport,timeout_ms,max_retries);return true;
}
bool reliable_sender_start(reliable_sender_t *s,const uint8_t *frame,size_t len,uint8_t sequence,uint8_t command,uint32_t now_ms){
 if(!s||!frame||len==0U||len>sizeof(s->frame))return false;
 if(!reliable_transport_start(&s->transport,sequence,command,now_ms))return false;
 memcpy(s->frame,frame,len);s->frame_length=len;
 if(!s->link.send_bytes(s->link.context,s->frame,s->frame_length)){s->transport.state=RELIABLE_FAILED;return false;}
 reliable_transport_mark_transmitted(&s->transport,now_ms);return true;
}
bool reliable_sender_poll(reliable_sender_t *s,uint32_t now_ms){
 if(!s||s->transport.state!=RELIABLE_WAIT_ACK)return false;
 if((int32_t)(now_ms-s->transport.deadline_ms)<0)return false;
 s->transport.timeout_count++;
 if(s->transport.retry_count>=s->transport.max_retries){s->transport.state=RELIABLE_FAILED;return false;}
 s->transport.retry_count++;
 if(!s->link.send_bytes(s->link.context,s->frame,s->frame_length)){s->transport.state=RELIABLE_FAILED;return false;}
 reliable_transport_mark_transmitted(&s->transport,now_ms);return true;
}
bool reliable_sender_on_ack(reliable_sender_t *s,uint8_t ack_sequence){return s?reliable_transport_on_ack(&s->transport,ack_sequence):false;}
