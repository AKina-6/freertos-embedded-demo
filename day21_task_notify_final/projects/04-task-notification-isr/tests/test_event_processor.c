#include <assert.h>
#include "app/isr_event_processor.h"
int main(void){isr_event_processor_t p;isr_event_processor_init(&p);isr_event_processor_handle(&p,ISR_EVENT_DMA_DONE);isr_event_processor_handle(&p,ISR_EVENT_BUTTON|ISR_EVENT_UART_RX);assert(p.dma_count==1U);assert(p.button_count==1U);assert(p.uart_count==1U);assert(p.batches==2U);return 0;}
