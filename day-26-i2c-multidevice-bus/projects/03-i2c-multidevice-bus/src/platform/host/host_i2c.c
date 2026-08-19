#include "host_i2c.h"
#include <string.h>

i2c_bus_status_t host_i2c_write(void *context, uint8_t address_7bit, const uint8_t *data, size_t length)
{
    host_i2c_t *h=(host_i2c_t*)context; (void)address_7bit; (void)data; (void)length;
    if (!h) return I2C_BUS_ERROR;
    h->writes++;
    i2c_bus_status_t s = h->next_write_status;
    h->next_write_status = I2C_BUS_OK;
    return s;
}

i2c_bus_status_t host_i2c_read(void *context, uint8_t address_7bit, uint8_t *data, size_t length)
{
    host_i2c_t *h=(host_i2c_t*)context; (void)address_7bit;
    if (!h || !data) return I2C_BUS_ERROR;
    h->reads++;
    i2c_bus_status_t s = h->next_read_status;
    h->next_read_status = I2C_BUS_OK;
    if (s == I2C_BUS_OK) memcpy(data, h->read_data, length);
    return s;
}

bool host_i2c_recover(void *context)
{
    host_i2c_t *h=(host_i2c_t*)context;
    if (!h) return false;
    h->recover_calls++;
    h->scl_pulses += 9U;
    return true;
}
