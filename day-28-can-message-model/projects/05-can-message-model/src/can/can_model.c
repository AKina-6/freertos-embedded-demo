#include "can/can_model.h"
#include <stddef.h>
bool can_frame_valid(const can_frame_t *f){return f&&f->id<=CAN_STD_ID_MAX&&f->dlc<=CAN_MAX_DLC;}
bool can_filter_accept(const can_filter_t *flt,const can_frame_t *f){
 return flt&&can_frame_valid(f)&&((f->id&flt->mask)==(flt->id&flt->mask));
}
void can_node_init(can_node_t *n,const char *name){if(!n)return;n->name=name;n->filter_count=0;n->rx_count=0;n->filtered_count=0;}
bool can_node_add_filter(can_node_t *n,can_filter_t f){
 if(!n||n->filter_count>=CAN_MAX_FILTERS||f.id>CAN_STD_ID_MAX||f.mask>CAN_STD_ID_MAX)return false;
 n->filters[n->filter_count++]=f;return true;
}
bool can_node_receive(can_node_t *n,const can_frame_t *f){
 if(!n||!can_frame_valid(f))return false;
 if(n->filter_count==0){n->last_rx=*f;n->rx_count++;return true;}
 for(size_t i=0;i<n->filter_count;i++) if(can_filter_accept(&n->filters[i],f)){n->last_rx=*f;n->rx_count++;return true;}
 n->filtered_count++;return false;
}
void can_bus_init(can_bus_t *b){if(!b)return;b->node_count=0;b->pending_count=0;b->transmitted_count=0;b->arbitration_count=0;b->invalid_rejects=0;}
bool can_bus_add_node(can_bus_t *b,can_node_t *n){if(!b||!n||b->node_count>=CAN_MAX_NODES)return false;b->nodes[b->node_count++]=n;return true;}
bool can_bus_request(can_bus_t *b,const char *source,const can_frame_t *f){
 if(!b||!source||!f||b->pending_count>=CAN_MAX_PENDING)return false;
 if(!can_frame_valid(f)){b->invalid_rejects++;return false;}
 b->pending[b->pending_count]=*f;b->sources[b->pending_count]=source;b->pending_count++;return true;
}
bool can_bus_arbitrate(can_bus_t *b,can_frame_t *winner,const char **source){
 if(!b||!winner||!source||b->pending_count==0)return false;size_t best=0;
 for(size_t i=1;i<b->pending_count;i++) if(b->pending[i].id<b->pending[best].id) best=i;
 *winner=b->pending[best];*source=b->sources[best];
 for(size_t n=0;n<b->node_count;n++) (void)can_node_receive(b->nodes[n],winner);
 for(size_t i=best+1;i<b->pending_count;i++){b->pending[i-1]=b->pending[i];b->sources[i-1]=b->sources[i];}
 b->pending_count--;b->transmitted_count++;b->arbitration_count++;return true;
}
