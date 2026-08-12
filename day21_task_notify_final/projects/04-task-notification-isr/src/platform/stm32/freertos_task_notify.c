#include "FreeRTOS.h"
#include "task.h"
#include "stm32f1xx_hal.h"
#define NOTIFY_BUTTON_BIT (1UL<<0)
#define NOTIFY_DMA_DONE_BIT (1UL<<1)
#define NOTIFY_UART_RX_BIT (1UL<<2)
static TaskHandle_t adcTaskHandle=NULL,eventTaskHandle=NULL;
static void AdcTask(void *arg){(void)arg;for(;;){uint32_t pending=ulTaskNotifyTake(pdFALSE,portMAX_DELAY);if(pending>0U){/* process one DMA block */}}}
static void EventTask(void *arg){(void)arg;for(;;){uint32_t bits=0U;if(xTaskNotifyWait(0U,NOTIFY_BUTTON_BIT|NOTIFY_DMA_DONE_BIT|NOTIFY_UART_RX_BIT,&bits,portMAX_DELAY)==pdTRUE){if(bits&NOTIFY_BUTTON_BIT)HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);}}}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){(void)hadc;BaseType_t woken=pdFALSE;vTaskNotifyGiveFromISR(adcTaskHandle,&woken);xTaskNotifyFromISR(eventTaskHandle,NOTIFY_DMA_DONE_BIT,eSetBits,&woken);portYIELD_FROM_ISR(woken);}
void HAL_GPIO_EXTI_Callback(uint16_t pin){if(pin==GPIO_PIN_0){BaseType_t woken=pdFALSE;xTaskNotifyFromISR(eventTaskHandle,NOTIFY_BUTTON_BIT,eSetBits,&woken);portYIELD_FROM_ISR(woken);}}
void freertos_task_notify_start(void){xTaskCreate(AdcTask,"Adc",256U,NULL,2U,&adcTaskHandle);xTaskCreate(EventTask,"Event",256U,NULL,3U,&eventTaskHandle);configASSERT(adcTaskHandle!=NULL);configASSERT(eventTaskHandle!=NULL);vTaskStartScheduler();}
