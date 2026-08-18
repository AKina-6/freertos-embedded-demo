#include "host_link.h"
#include <string.h>
bool host_link_send(void *context,const uint8_t *data,size_t length){
 host_link_t *h=(host_link_t*)context; if(!h||!data||length>sizeof(h->last_frame))return false;
 if(h->fail_next_send){h->fail_next_send=false;return false;}
 memcpy(h->last_frame,data,length);h->last_length=length;h->send_count++;return true;
}
