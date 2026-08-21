#include <assert.h>
#include "can/can_model.h"
int main(void){
 can_frame_t f={0x123U,8U,{0}};assert(can_frame_valid(&f));f.id=0x800U;assert(!can_frame_valid(&f));f.id=0x123U;f.dlc=9U;assert(!can_frame_valid(&f));f.dlc=1U;
 can_filter_t flt={0x120U,0x7F0U};assert(can_filter_accept(&flt,&f));f.id=0x130U;assert(!can_filter_accept(&flt,&f));
 can_node_t n;can_node_init(&n,"N");can_bus_t b;can_bus_init(&b);assert(can_bus_add_node(&b,&n));
 can_frame_t a={0x080U,1U,{1}},z={0x300U,1U,{2}};assert(can_bus_request(&b,"A",&a));assert(can_bus_request(&b,"Z",&z));can_frame_t w;const char *s=0;
 assert(can_bus_arbitrate(&b,&w,&s));assert(w.id==0x080U);assert(can_bus_arbitrate(&b,&w,&s));assert(w.id==0x300U);assert(n.rx_count==2U);return 0;
}
