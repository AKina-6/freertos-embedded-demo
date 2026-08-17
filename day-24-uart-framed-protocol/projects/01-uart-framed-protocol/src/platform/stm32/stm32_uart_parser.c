#include "protocol/stream_parser.h"
#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart1;
static uint8_t rxbuf[64]; static stream_parser_t parser;
static void protocol_handle_frame(const protocol_frame_t *f){if(!f)return;switch(f->command){case 0x10U: break;case 0x20U: break;default: break;}}
void uart_protocol_start(void){stream_parser_init(&parser);HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rxbuf,sizeof(rxbuf));__HAL_DMA_DISABLE_IT(huart1.hdmarx,DMA_IT_HT);}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size){if(huart!=&huart1)return;for(uint16_t i=0;i<size;++i){protocol_frame_t f;if(stream_parser_feed_byte(&parser,rxbuf[i],&f))protocol_handle_frame(&f);}HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rxbuf,sizeof(rxbuf));__HAL_DMA_DISABLE_IT(huart1.hdmarx,DMA_IT_HT);}
