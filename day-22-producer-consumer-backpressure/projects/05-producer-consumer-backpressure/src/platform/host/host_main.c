#include <stdio.h>
#include "app/producer_consumer.h"
int main(void) {
    producer_consumer_t pc;
    producer_consumer_init(&pc,RING_BUFFER_DROP_OLDEST);
    /* Producer burst: 10 items into capacity 8 -> 2 oldest samples evicted. */
    for (uint32_t i=0;i<10U;i++) {
        producer_consumer_produce(&pc,i*10U,(int32_t)(100+i));
        if (producer_consumer_should_throttle(&pc,6U)) pc.producer_throttles++;
    }
    printf("after_burst size=%lu high_watermark=%lu produced=%lu dropped_oldest=%lu throttles=%lu\n",
        (unsigned long)ring_buffer_size(&pc.queue),(unsigned long)pc.queue.high_watermark,
        (unsigned long)pc.produced,(unsigned long)pc.queue.dropped_oldest,
        (unsigned long)pc.producer_throttles);
    sample_t s;
    while (producer_consumer_consume(&pc,&s)) {
        printf("consume seq=%lu ts=%lu value=%ld remaining=%lu\n",
            (unsigned long)s.sequence,(unsigned long)s.timestamp_ms,(long)s.value,
            (unsigned long)ring_buffer_size(&pc.queue));
    }
    printf("summary produced=%lu consumed=%lu idle=%lu pushed=%lu popped=%lu drop_oldest=%lu drop_newest=%lu\n",
        (unsigned long)pc.produced,(unsigned long)pc.consumed,(unsigned long)pc.consumer_idle,
        (unsigned long)pc.queue.pushed,(unsigned long)pc.queue.popped,
        (unsigned long)pc.queue.dropped_oldest,(unsigned long)pc.queue.dropped_newest);
    return 0;
}
