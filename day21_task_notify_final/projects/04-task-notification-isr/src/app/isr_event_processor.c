#include "app/isr_event_processor.h"
#include <stddef.h>
void isr_event_processor_init(isr_event_processor_t *p){if(!p)return;p->button_count=p->dma_count=p->uart_count=p->batches=0U;}
void isr_event_processor_handle(isr_event_processor_t *p,uint32_t bits){if(!p||!bits)return;if(bits&ISR_EVENT_BUTTON)p->button_count++;if(bits&ISR_EVENT_DMA_DONE)p->dma_count++;if(bits&ISR_EVENT_UART_RX)p->uart_count++;p->batches++;}
