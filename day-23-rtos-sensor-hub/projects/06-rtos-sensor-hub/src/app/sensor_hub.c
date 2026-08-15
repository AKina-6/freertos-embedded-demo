#include "app/sensor_hub.h"
#include <stddef.h>

void sensor_hub_init(sensor_hub_t *hub)
{
    if (!hub) return;
    hub->head=hub->tail=hub->count=hub->high_watermark=0U;
    hub->produced=hub->consumed=hub->dropped_oldest=0U;
    hub->event_bits=0U;
    hub->heartbeat_count=0U;
    hub->button_notify_count=0U;
    hub->uart_lock_count=0U;
    hub->uart_write_count=0U;
    hub->telemetry_count=0U;
    hub->has_latest=false;
}

void sensor_hub_set_event(sensor_hub_t *hub, uint32_t bits)
{
    if (!hub) return;
    hub->event_bits |= bits;
}

bool sensor_hub_all_ready(const sensor_hub_t *hub)
{
    const uint32_t all = HUB_EVENT_SENSOR_READY | HUB_EVENT_DISPLAY_READY | HUB_EVENT_STORAGE_READY;
    return hub && ((hub->event_bits & all) == all);
}

void sensor_hub_timer_heartbeat(sensor_hub_t *hub)
{
    if (!hub) return;
    hub->heartbeat_count++;
    hub->event_bits |= HUB_EVENT_HEARTBEAT;
}

void sensor_hub_button_notify(sensor_hub_t *hub)
{
    if (!hub) return;
    hub->button_notify_count++;
    hub->event_bits |= HUB_EVENT_BUTTON;
}

bool sensor_hub_produce(sensor_hub_t *hub, const sensor_sample_t *sample)
{
    if (!hub || !sample) return false;
    if (hub->count == SENSOR_QUEUE_CAPACITY) {
        hub->tail = (hub->tail + 1U) % SENSOR_QUEUE_CAPACITY;
        hub->count--;
        hub->dropped_oldest++;
    }
    hub->queue[hub->head] = *sample;
    hub->head = (hub->head + 1U) % SENSOR_QUEUE_CAPACITY;
    hub->count++;
    hub->produced++;
    if (hub->count > hub->high_watermark) hub->high_watermark = hub->count;
    return true;
}

bool sensor_hub_consume(sensor_hub_t *hub, sensor_sample_t *sample)
{
    if (!hub || !sample || hub->count == 0U) return false;
    *sample = hub->queue[hub->tail];
    hub->tail = (hub->tail + 1U) % SENSOR_QUEUE_CAPACITY;
    hub->count--;
    hub->consumed++;
    hub->latest = *sample;
    hub->has_latest=true;
    return true;
}

bool sensor_hub_uart_log(sensor_hub_t *hub, const char *message)
{
    if (!hub || !message) return false;
    hub->uart_lock_count++;
    hub->uart_write_count++;
    hub->telemetry_count++;
    return true;
}
