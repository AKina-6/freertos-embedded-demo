#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "app/sample.h"
#define RING_BUFFER_CAPACITY 8U
typedef enum {
    RING_BUFFER_DROP_NEWEST = 0,
    RING_BUFFER_DROP_OLDEST
} ring_buffer_overflow_policy_t;
typedef struct {
    sample_t data[RING_BUFFER_CAPACITY];
    size_t head, tail, count, high_watermark;
    uint32_t pushed, popped, dropped_newest, dropped_oldest;
    ring_buffer_overflow_policy_t policy;
} ring_buffer_t;
void ring_buffer_init(ring_buffer_t *rb, ring_buffer_overflow_policy_t policy);
bool ring_buffer_push(ring_buffer_t *rb, const sample_t *sample);
bool ring_buffer_pop(ring_buffer_t *rb, sample_t *sample);
size_t ring_buffer_size(const ring_buffer_t *rb);
bool ring_buffer_is_full(const ring_buffer_t *rb);
#endif
