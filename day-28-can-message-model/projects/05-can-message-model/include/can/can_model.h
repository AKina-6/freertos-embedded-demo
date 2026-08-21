#ifndef CAN_MODEL_H
#define CAN_MODEL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define CAN_STD_ID_MAX 0x7FFU
#define CAN_MAX_DLC 8U
#define CAN_MAX_NODES 8U
#define CAN_MAX_FILTERS 4U
#define CAN_MAX_PENDING 8U
typedef struct { uint16_t id; uint8_t dlc; uint8_t data[8]; } can_frame_t;
typedef struct { uint16_t id; uint16_t mask; } can_filter_t;
typedef struct {
 const char *name; can_filter_t filters[CAN_MAX_FILTERS]; size_t filter_count;
 uint32_t rx_count, filtered_count; can_frame_t last_rx;
} can_node_t;
typedef struct {
 can_node_t *nodes[CAN_MAX_NODES]; size_t node_count;
 can_frame_t pending[CAN_MAX_PENDING]; const char *sources[CAN_MAX_PENDING]; size_t pending_count;
 uint32_t transmitted_count, arbitration_count, invalid_rejects;
} can_bus_t;
bool can_frame_valid(const can_frame_t *f);
bool can_filter_accept(const can_filter_t *flt,const can_frame_t *f);
void can_node_init(can_node_t *n,const char *name);
bool can_node_add_filter(can_node_t *n,can_filter_t f);
bool can_node_receive(can_node_t *n,const can_frame_t *f);
void can_bus_init(can_bus_t *b);
bool can_bus_add_node(can_bus_t *b,can_node_t *n);
bool can_bus_request(can_bus_t *b,const char *source,const can_frame_t *f);
bool can_bus_arbitrate(can_bus_t *b,can_frame_t *winner,const char **source);
#endif
