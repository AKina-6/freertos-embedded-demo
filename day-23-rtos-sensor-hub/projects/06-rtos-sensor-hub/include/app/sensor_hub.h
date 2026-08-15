#ifndef SENSOR_HUB_H
#define SENSOR_HUB_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SENSOR_QUEUE_CAPACITY 8U
#define HUB_EVENT_SENSOR_READY   (1UL << 0)
#define HUB_EVENT_DISPLAY_READY  (1UL << 1)
#define HUB_EVENT_STORAGE_READY  (1UL << 2)
#define HUB_EVENT_HEARTBEAT      (1UL << 3)
#define HUB_EVENT_BUTTON         (1UL << 4)

typedef struct {
    uint32_t sequence;
    uint32_t timestamp_ms;
    float temperature_c;
    float light_lux;
} sensor_sample_t;

typedef struct {
    sensor_sample_t queue[SENSOR_QUEUE_CAPACITY];
    size_t head, tail, count, high_watermark;
    uint32_t produced, consumed, dropped_oldest;
    uint32_t event_bits;
    uint32_t heartbeat_count;
    uint32_t button_notify_count;
    uint32_t uart_lock_count;
    uint32_t uart_write_count;
    uint32_t telemetry_count;
    sensor_sample_t latest;
    bool has_latest;
} sensor_hub_t;

void sensor_hub_init(sensor_hub_t *hub);
void sensor_hub_set_event(sensor_hub_t *hub, uint32_t bits);
bool sensor_hub_all_ready(const sensor_hub_t *hub);
void sensor_hub_timer_heartbeat(sensor_hub_t *hub);
void sensor_hub_button_notify(sensor_hub_t *hub);
bool sensor_hub_produce(sensor_hub_t *hub, const sensor_sample_t *sample);
bool sensor_hub_consume(sensor_hub_t *hub, sensor_sample_t *sample);
bool sensor_hub_uart_log(sensor_hub_t *hub, const char *message);
#endif
