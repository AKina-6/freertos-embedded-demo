#include <stdio.h>
#include "i2c/i2c_bus.h"
#include "i2c/i2c_registry.h"
#include "i2c/i2c_manager.h"
#include "host_i2c.h"

int main(void)
{
    host_i2c_t host = {0};
    host.read_data[0]=0x12U; host.read_data[1]=0x34U;
    i2c_bus_ops_t ops = {&host, host_i2c_write, host_i2c_read, host_i2c_recover};
    i2c_bus_t bus; i2c_bus_init(&bus,&ops);

    i2c_registry_t registry; i2c_registry_init(&registry);
    printf("add_bh1750=%s\n", i2c_registry_add(&registry,0x23U,"BH1750")?"ok":"fail");
    printf("add_oled=%s\n", i2c_registry_add(&registry,0x3CU,"SSD1306")?"ok":"fail");
    printf("duplicate_0x23=%s\n", i2c_registry_add(&registry,0x23U,"OTHER")?"ok":"rejected");

    i2c_manager_t mgr; i2c_manager_init(&mgr,&bus,1U);

    uint8_t cmd=0x10U;
    i2c_bus_status_t s1=i2c_manager_write(&mgr,0x23U,&cmd,1U);
    printf("write_bh1750 status=%d writes=%lu retries=%lu\n",(int)s1,(unsigned long)host.writes,(unsigned long)mgr.retry_count);

    host.next_read_status = I2C_BUS_TIMEOUT;
    uint8_t data[2]={0};
    i2c_bus_status_t s2=i2c_manager_read(&mgr,0x23U,data,2U);
    printf("read_after_timeout status=%d data=%02X%02X reads=%lu recoveries=%lu scl_pulses=%lu retries=%lu\n",
           (int)s2,data[0],data[1],(unsigned long)host.reads,(unsigned long)host.recover_calls,
           (unsigned long)host.scl_pulses,(unsigned long)mgr.retry_count);

    host.next_write_status = I2C_BUS_NACK;
    i2c_bus_status_t s3=i2c_manager_write(&mgr,0x3CU,&cmd,1U);
    printf("write_after_nack status=%d nack_count=%lu recoveries=%lu retries=%lu\n",
           (int)s3,(unsigned long)bus.nack_count,(unsigned long)host.recover_calls,(unsigned long)mgr.retry_count);

    printf("summary devices=%lu duplicate_rejects=%lu success=%lu bus_recovery_count=%lu\n",
           (unsigned long)registry.count,(unsigned long)registry.duplicate_rejects,
           (unsigned long)mgr.transaction_success,(unsigned long)bus.recovery_count);
    return 0;
}
