#include <assert.h>
#include "app/producer_consumer.h"
int main(void) {
    producer_consumer_t pc; producer_consumer_init(&pc,RING_BUFFER_DROP_NEWEST);
    assert(producer_consumer_produce(&pc,100U,42));
    assert(pc.produced==1U);
    assert(!producer_consumer_should_throttle(&pc,6U));
    sample_t s; assert(producer_consumer_consume(&pc,&s));
    assert(s.sequence==1U && s.value==42);
    assert(pc.consumed==1U);
    assert(!producer_consumer_consume(&pc,&s));
    assert(pc.consumer_idle==1U);
    for (unsigned i=0;i<6U;i++) assert(producer_consumer_produce(&pc,200U+i,(int32_t)i));
    assert(producer_consumer_should_throttle(&pc,6U));
    return 0;
}
