#include <stdio.h>
#include "app/sensor_hub.h"

int main(void)
{
    sensor_hub_t hub; sensor_hub_init(&hub);

    sensor_hub_set_event(&hub, HUB_EVENT_SENSOR_READY);
    sensor_hub_set_event(&hub, HUB_EVENT_DISPLAY_READY);
    printf("ready_after_2=%s bits=0x%02lX\n", sensor_hub_all_ready(&hub)?"yes":"no", (unsigned long)hub.event_bits);
    sensor_hub_set_event(&hub, HUB_EVENT_STORAGE_READY);
    printf("ready_after_3=%s bits=0x%02lX\n", sensor_hub_all_ready(&hub)?"yes":"no", (unsigned long)hub.event_bits);

    for (uint32_t i=1U;i<=10U;i++) {
        sensor_sample_t s={i,i*100U,20.0F+(float)i,100.0F+(float)(i*10U)};
        sensor_hub_produce(&hub,&s);
    }
    printf("after_burst queued=%lu high_watermark=%lu produced=%lu dropped=%lu\n",
           (unsigned long)hub.count,(unsigned long)hub.high_watermark,
           (unsigned long)hub.produced,(unsigned long)hub.dropped_oldest);

    sensor_hub_timer_heartbeat(&hub);
    sensor_hub_button_notify(&hub);
    printf("events heartbeat=%lu button=%lu bits=0x%02lX\n",
           (unsigned long)hub.heartbeat_count,(unsigned long)hub.button_notify_count,
           (unsigned long)hub.event_bits);

    sensor_sample_t out;
    while (sensor_hub_consume(&hub,&out)) {
        printf("consume seq=%lu temp=%.1f light=%.1f remaining=%lu\n",
               (unsigned long)out.sequence,(double)out.temperature_c,(double)out.light_lux,(unsigned long)hub.count);
    }

    sensor_hub_uart_log(&hub,"telemetry-1");
    sensor_hub_uart_log(&hub,"telemetry-2");

    printf("summary consumed=%lu uart_locks=%lu uart_writes=%lu telemetry=%lu latest_seq=%lu\n",
           (unsigned long)hub.consumed,(unsigned long)hub.uart_lock_count,
           (unsigned long)hub.uart_write_count,(unsigned long)hub.telemetry_count,
           (unsigned long)(hub.has_latest?hub.latest.sequence:0U));
    return 0;
}
