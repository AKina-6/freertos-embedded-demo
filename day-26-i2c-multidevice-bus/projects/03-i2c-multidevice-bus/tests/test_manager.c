#include <assert.h>
#include "i2c/i2c_bus.h"
#include "i2c/i2c_manager.h"
#include "host_i2c.h"
int main(void){
    host_i2c_t host={0}; host.read_data[0]=0xAAU; host.read_data[1]=0x55U;
    i2c_bus_ops_t ops={&host,host_i2c_write,host_i2c_read,host_i2c_recover};
    i2c_bus_t bus; assert(i2c_bus_init(&bus,&ops));
    i2c_manager_t m; i2c_manager_init(&m,&bus,1U);
    host.next_read_status=I2C_BUS_TIMEOUT;
    uint8_t data[2]={0};
    assert(i2c_manager_read(&m,0x23U,data,2U)==I2C_BUS_OK);
    assert(data[0]==0xAAU && data[1]==0x55U);
    assert(host.recover_calls==1U); assert(host.scl_pulses==9U); assert(m.retry_count==1U);
    host.next_write_status=I2C_BUS_NACK; uint8_t cmd=1U;
    assert(i2c_manager_write(&m,0x3CU,&cmd,1U)==I2C_BUS_OK);
    assert(bus.nack_count==1U); assert(host.recover_calls==1U);
    return 0;
}
