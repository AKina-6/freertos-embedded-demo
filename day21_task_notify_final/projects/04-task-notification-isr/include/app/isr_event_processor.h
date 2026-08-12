#ifndef ISR_EVENT_PROCESSOR_H
#define ISR_EVENT_PROCESSOR_H
#include <stdint.h>
#define ISR_EVENT_BUTTON (1UL<<0)
#define ISR_EVENT_DMA_DONE (1UL<<1)
#define ISR_EVENT_UART_RX (1UL<<2)
typedef struct {uint32_t button_count,dma_count,uart_count,batches;} isr_event_processor_t;
void isr_event_processor_init(isr_event_processor_t *p);
void isr_event_processor_handle(isr_event_processor_t *p,uint32_t bits);
#endif
