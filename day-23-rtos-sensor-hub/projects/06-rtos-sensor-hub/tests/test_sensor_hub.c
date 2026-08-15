#include <assert.h>
#include "app/sensor_hub.h"
int main(void)
{
    sensor_hub_t h; sensor_hub_init(&h);
    sensor_hub_set_event(&h,HUB_EVENT_SENSOR_READY|HUB_EVENT_DISPLAY_READY);
    assert(!sensor_hub_all_ready(&h));
    sensor_hub_set_event(&h,HUB_EVENT_STORAGE_READY);
    assert(sensor_hub_all_ready(&h));
    for (uint32_t i=1;i<=10;i++) {
        sensor_sample_t s={i,i*10U,20.0F,100.0F};
        assert(sensor_hub_produce(&h,&s));
    }
    assert(h.count==8U);
    assert(h.dropped_oldest==2U);
    sensor_sample_t out;
    assert(sensor_hub_consume(&h,&out));
    assert(out.sequence==3U);
    sensor_hub_timer_heartbeat(&h);
    sensor_hub_button_notify(&h);
    assert((h.event_bits & HUB_EVENT_HEARTBEAT)!=0U);
    assert((h.event_bits & HUB_EVENT_BUTTON)!=0U);
    assert(sensor_hub_uart_log(&h,"ok"));
    assert(h.uart_lock_count==1U && h.uart_write_count==1U);
    return 0;
}
