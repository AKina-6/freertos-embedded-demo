#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H
#include <stdbool.h>
#include <stdint.h>
#include "app/ring_buffer.h"
typedef struct {
    ring_buffer_t queue;
    uint32_t next_sequence;
    uint32_t produced;
    uint32_t consumed;
    uint32_t producer_throttles;
    uint32_t consumer_idle;
} producer_consumer_t;
void producer_consumer_init(producer_consumer_t *pc, ring_buffer_overflow_policy_t policy);
bool producer_consumer_produce(producer_consumer_t *pc, uint32_t timestamp_ms, int32_t value);
bool producer_consumer_consume(producer_consumer_t *pc, sample_t *sample);
bool producer_consumer_should_throttle(const producer_consumer_t *pc, size_t threshold);
#endif
