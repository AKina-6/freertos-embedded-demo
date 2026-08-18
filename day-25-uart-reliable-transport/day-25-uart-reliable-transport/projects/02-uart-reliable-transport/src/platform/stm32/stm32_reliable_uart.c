/* Integration sketch with Day 24 protocol frame/parser. */
#include "reliable/reliable_sender.h"
#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart1;
static reliable_sender_t sender;
static bool uart_send(void *context,const uint8_t *data,size_t length){UART_HandleTypeDef *u=(UART_HandleTypeDef*)context;return HAL_UART_Transmit(u,(uint8_t*)data,(uint16_t)length,100U)==HAL_OK;}
void reliable_uart_init(void){reliable_link_t link={&huart1,uart_send};(void)reliable_sender_init(&sender,&link,200U,3U);}
bool reliable_uart_send_raw(const uint8_t *frame,size_t length,uint8_t seq,uint8_t cmd){return reliable_sender_start(&sender,frame,length,seq,cmd,HAL_GetTick());}
void reliable_uart_poll(void){(void)reliable_sender_poll(&sender,HAL_GetTick());}
void reliable_uart_on_ack(uint8_t seq){(void)reliable_sender_on_ack(&sender,seq);}
