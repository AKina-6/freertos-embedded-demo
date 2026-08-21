#include <stdio.h>
#include "can/can_model.h"
static void pf(const char *src,const can_frame_t *f){printf("winner source=%s id=0x%03X dlc=%u data=",src,f->id,f->dlc);for(uint8_t i=0;i<f->dlc;i++){printf("%02X",f->data[i]);if(i+1<f->dlc)printf(" ");}printf("\n");}
int main(void){
 can_node_t display,gateway,logger;can_node_init(&display,"Display");can_node_init(&gateway,"Gateway");can_node_init(&logger,"Logger");
 can_node_add_filter(&display,(can_filter_t){0x100U,0x7F0U});
 can_node_add_filter(&gateway,(can_filter_t){0x200U,0x7FFU});
 can_bus_t bus;can_bus_init(&bus);can_bus_add_node(&bus,&display);can_bus_add_node(&bus,&gateway);can_bus_add_node(&bus,&logger);
 can_frame_t sensor={0x120U,4U,{0x19,0x08,0x02,0x58}},alarm={0x080U,2U,{0x01,0xFF}},ui={0x105U,3U,{0x41,0x42,0x43}};
 can_bus_request(&bus,"Sensor",&sensor);can_bus_request(&bus,"Alarm",&alarm);can_bus_request(&bus,"UI",&ui);
 can_frame_t w;const char *src=0;while(can_bus_arbitrate(&bus,&w,&src))pf(src,&w);
 printf("display rx=%lu filtered=%lu last=0x%03X\n",(unsigned long)display.rx_count,(unsigned long)display.filtered_count,display.rx_count?display.last_rx.id:0U);
 printf("gateway rx=%lu filtered=%lu\n",(unsigned long)gateway.rx_count,(unsigned long)gateway.filtered_count);
 printf("logger rx=%lu filtered=%lu last=0x%03X\n",(unsigned long)logger.rx_count,(unsigned long)logger.filtered_count,logger.rx_count?logger.last_rx.id:0U);
 printf("summary transmitted=%lu arbitrations=%lu invalid=%lu\n",(unsigned long)bus.transmitted_count,(unsigned long)bus.arbitration_count,(unsigned long)bus.invalid_rejects);return 0;
}
