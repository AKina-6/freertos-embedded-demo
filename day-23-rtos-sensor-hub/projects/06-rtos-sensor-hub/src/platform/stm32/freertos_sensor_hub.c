/* Integration sketch for STM32F103 + FreeRTOS. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>

#define EVT_SENSOR_READY  (1UL << 0)
#define EVT_DISPLAY_READY (1UL << 1)
#define EVT_STORAGE_READY (1UL << 2)
#define EVT_HEARTBEAT     (1UL << 3)
#define NOTIFY_BUTTON     (1UL << 0)

typedef struct {
    uint32_t sequence;
    float temperature_c;
    float light_lux;
} sensor_sample_t;

extern UART_HandleTypeDef huart1;
static QueueHandle_t sampleQueue;
static SemaphoreHandle_t uartMutex;
static EventGroupHandle_t systemEvents;
static TimerHandle_t heartbeatTimer;
static TaskHandle_t uiTaskHandle;

static void uart_log(const char *text)
{
    if (xSemaphoreTake(uartMutex, pdMS_TO_TICKS(50U)) == pdTRUE) {
        HAL_UART_Transmit(&huart1,(uint8_t*)text,(uint16_t)strlen(text),100U);
        xSemaphoreGive(uartMutex);
    }
}

static void HeartbeatCallback(TimerHandle_t timer)
{
    (void)timer;
    xEventGroupSetBits(systemEvents, EVT_HEARTBEAT);
}

static void SensorTask(void *arg)
{
    (void)arg; uint32_t seq=0U; TickType_t last=xTaskGetTickCount();
    xEventGroupSetBits(systemEvents, EVT_SENSOR_READY);
    for (;;) {
        sensor_sample_t sample={++seq,25.0F,300.0F};
        if (xQueueSend(sampleQueue,&sample,pdMS_TO_TICKS(20U)) != pdTRUE) {
            sensor_sample_t drop;
            (void)xQueueReceive(sampleQueue,&drop,0U);
            (void)xQueueSend(sampleQueue,&sample,0U); /* drop oldest backpressure */
        }
        vTaskDelayUntil(&last,pdMS_TO_TICKS(500U));
    }
}

static void ProcessingTask(void *arg)
{
    (void)arg; sensor_sample_t sample; char line[96];
    for (;;) {
        if (xQueueReceive(sampleQueue,&sample,portMAX_DELAY) == pdTRUE) {
            int n=snprintf(line,sizeof(line),"SEQ=%lu T=%.1f L=%.1f\r\n",
                           (unsigned long)sample.sequence,(double)sample.temperature_c,(double)sample.light_lux);
            if (n>0) uart_log(line);
        }
    }
}

static void StartupTask(void *arg)
{
    (void)arg;
    EventBits_t bits=xEventGroupWaitBits(systemEvents,
        EVT_SENSOR_READY|EVT_DISPLAY_READY|EVT_STORAGE_READY,
        pdFALSE,pdTRUE,pdMS_TO_TICKS(5000U));
    if ((bits & (EVT_SENSOR_READY|EVT_DISPLAY_READY|EVT_STORAGE_READY)) ==
        (EVT_SENSOR_READY|EVT_DISPLAY_READY|EVT_STORAGE_READY)) {
        uart_log("SYSTEM READY\r\n");
    }
    vTaskDelete(NULL);
}

static void UiTask(void *arg)
{
    (void)arg; uint32_t bits;
    xEventGroupSetBits(systemEvents, EVT_DISPLAY_READY);
    for (;;) {
        if (xTaskNotifyWait(0U,NOTIFY_BUTTON,&bits,portMAX_DELAY)==pdTRUE) {
            if (bits & NOTIFY_BUTTON) { /* page switch */ }
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0) {
        BaseType_t woken=pdFALSE;
        xTaskNotifyFromISR(uiTaskHandle,NOTIFY_BUTTON,eSetBits,&woken);
        portYIELD_FROM_ISR(woken);
    }
}

void freertos_sensor_hub_start(void)
{
    sampleQueue=xQueueCreate(8U,sizeof(sensor_sample_t));
    uartMutex=xSemaphoreCreateMutex();
    systemEvents=xEventGroupCreate();
    heartbeatTimer=xTimerCreate("Heartbeat",pdMS_TO_TICKS(1000U),pdTRUE,NULL,HeartbeatCallback);
    configASSERT(sampleQueue && uartMutex && systemEvents && heartbeatTimer);

    xEventGroupSetBits(systemEvents, EVT_STORAGE_READY); /* after settings load */
    xTaskCreate(SensorTask,"Sensor",256U,NULL,2U,NULL);
    xTaskCreate(ProcessingTask,"Process",256U,NULL,3U,NULL);
    xTaskCreate(StartupTask,"Startup",192U,NULL,4U,NULL);
    xTaskCreate(UiTask,"UI",192U,NULL,2U,&uiTaskHandle);
    xTimerStart(heartbeatTimer,0U);
    vTaskStartScheduler();
}
