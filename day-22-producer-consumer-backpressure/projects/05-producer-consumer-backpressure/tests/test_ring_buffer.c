#include <assert.h>
#include "app/ring_buffer.h"
int main(void) {
    ring_buffer_t rb; ring_buffer_init(&rb,RING_BUFFER_DROP_NEWEST);
    sample_t s={1U,10U,1};
    for (unsigned i=0;i<RING_BUFFER_CAPACITY;i++) { s.sequence=i+1U; assert(ring_buffer_push(&rb,&s)); }
    assert(ring_buffer_is_full(&rb));
    assert(!ring_buffer_push(&rb,&s));
    assert(rb.dropped_newest==1U);
    sample_t out; assert(ring_buffer_pop(&rb,&out)); assert(out.sequence==1U);
    ring_buffer_init(&rb,RING_BUFFER_DROP_OLDEST);
    for (unsigned i=0;i<RING_BUFFER_CAPACITY+2U;i++) { s.sequence=i+1U; assert(ring_buffer_push(&rb,&s)); }
    assert(rb.dropped_oldest==2U);
    assert(ring_buffer_pop(&rb,&out)); assert(out.sequence==3U);
    return 0;
}
