#include "app/ring_buffer.h"
#include <stddef.h>
void ring_buffer_init(ring_buffer_t *rb, ring_buffer_overflow_policy_t policy) {
    if (!rb) return;
    rb->head=rb->tail=rb->count=rb->high_watermark=0U;
    rb->pushed=rb->popped=rb->dropped_newest=rb->dropped_oldest=0U;
    rb->policy=policy;
}
bool ring_buffer_push(ring_buffer_t *rb, const sample_t *sample) {
    if (!rb || !sample) return false;
    if (rb->count == RING_BUFFER_CAPACITY) {
        if (rb->policy == RING_BUFFER_DROP_NEWEST) {
            rb->dropped_newest++;
            return false;
        }
        rb->tail=(rb->tail+1U)%RING_BUFFER_CAPACITY;
        rb->count--;
        rb->dropped_oldest++;
    }
    rb->data[rb->head]=*sample;
    rb->head=(rb->head+1U)%RING_BUFFER_CAPACITY;
    rb->count++;
    rb->pushed++;
    if (rb->count>rb->high_watermark) rb->high_watermark=rb->count;
    return true;
}
bool ring_buffer_pop(ring_buffer_t *rb, sample_t *sample) {
    if (!rb || !sample || rb->count==0U) return false;
    *sample=rb->data[rb->tail];
    rb->tail=(rb->tail+1U)%RING_BUFFER_CAPACITY;
    rb->count--; rb->popped++; return true;
}
size_t ring_buffer_size(const ring_buffer_t *rb) { return rb?rb->count:0U; }
bool ring_buffer_is_full(const ring_buffer_t *rb) { return rb && rb->count==RING_BUFFER_CAPACITY; }
