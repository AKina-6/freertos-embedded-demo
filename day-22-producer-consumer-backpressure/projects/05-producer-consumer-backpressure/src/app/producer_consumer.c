#include "app/producer_consumer.h"
#include <stddef.h>
void producer_consumer_init(producer_consumer_t *pc, ring_buffer_overflow_policy_t policy) {
    if (!pc) return;
    ring_buffer_init(&pc->queue,policy);
    pc->next_sequence=1U; pc->produced=pc->consumed=0U;
    pc->producer_throttles=pc->consumer_idle=0U;
}
bool producer_consumer_produce(producer_consumer_t *pc, uint32_t timestamp_ms, int32_t value) {
    if (!pc) return false;
    sample_t s={pc->next_sequence,timestamp_ms,value};
    const bool ok=ring_buffer_push(&pc->queue,&s);
    if (ok) { pc->next_sequence++; pc->produced++; }
    return ok;
}
bool producer_consumer_consume(producer_consumer_t *pc, sample_t *sample) {
    if (!pc || !sample) return false;
    if (!ring_buffer_pop(&pc->queue,sample)) { pc->consumer_idle++; return false; }
    pc->consumed++; return true;
}
bool producer_consumer_should_throttle(const producer_consumer_t *pc, size_t threshold) {
    return pc && threshold<=RING_BUFFER_CAPACITY && ring_buffer_size(&pc->queue)>=threshold;
}
