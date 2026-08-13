/* STM32 + FreeRTOS Producer/Consumer example using a FreeRTOS Queue. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f1xx_hal.h"
#include "app/sample.h"
extern ADC_HandleTypeDef hadc1;
static QueueHandle_t sampleQueue;
static volatile uint32_t sequence=1U;
static void ProducerTask(void *argument) {
    (void)argument;
    TickType_t last=xTaskGetTickCount();
    for (;;) {
        sample_t s={sequence++,HAL_GetTick(),(int32_t)HAL_ADC_GetValue(&hadc1)};
        /* Backpressure policy: wait up to 20 ms for queue space. */
        if (xQueueSend(sampleQueue,&s,pdMS_TO_TICKS(20U)) != pdTRUE) {
            /* queue remained full: count/drop/log here */
        }
        vTaskDelayUntil(&last,pdMS_TO_TICKS(100U));
    }
}
static void ConsumerTask(void *argument) {
    (void)argument; sample_t s;
    for (;;) {
        if (xQueueReceive(sampleQueue,&s,portMAX_DELAY)==pdTRUE) {
            /* Process / filter / store / transmit sample. */
            vTaskDelay(pdMS_TO_TICKS(250U)); /* intentionally slower consumer */
        }
    }
}
static void MonitorTask(void *argument) {
    (void)argument;
    for (;;) {
        const UBaseType_t waiting=uxQueueMessagesWaiting(sampleQueue);
        const UBaseType_t free_slots=uxQueueSpacesAvailable(sampleQueue);
        (void)waiting; (void)free_slots;
        vTaskDelay(pdMS_TO_TICKS(500U));
    }
}
void freertos_producer_consumer_start(void) {
    sampleQueue=xQueueCreate(8U,sizeof(sample_t));
    configASSERT(sampleQueue!=NULL);
    xTaskCreate(ProducerTask,"Producer",256U,NULL,2U,NULL);
    xTaskCreate(ConsumerTask,"Consumer",256U,NULL,2U,NULL);
    xTaskCreate(MonitorTask,"Monitor",192U,NULL,1U,NULL);
    vTaskStartScheduler();
}
