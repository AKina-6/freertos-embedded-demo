#ifndef HOST_LINK_H
#define HOST_LINK_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct { uint32_t send_count; size_t last_length; uint8_t last_frame[40]; bool fail_next_send; } host_link_t;
bool host_link_send(void *context,const uint8_t *data,size_t length);
#endif
