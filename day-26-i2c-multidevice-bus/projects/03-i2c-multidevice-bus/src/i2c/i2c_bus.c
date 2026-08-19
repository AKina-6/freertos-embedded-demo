#include "i2c/i2c_bus.h"
#include <stddef.h>

static void count_status(i2c_bus_t *bus, i2c_bus_status_t status)
{
    if (status == I2C_BUS_NACK) bus->nack_count++;
    else if (status == I2C_BUS_TIMEOUT) bus->timeout_count++;
    else if (status == I2C_BUS_ERROR) bus->error_count++;
}

bool i2c_bus_init(i2c_bus_t *bus, const i2c_bus_ops_t *ops)
{
    if (!bus || !ops || !ops->write || !ops->read || !ops->recover) return false;
    bus->ops = *ops;
    bus->write_count = bus->read_count = 0U;
    bus->nack_count = bus->timeout_count = bus->error_count = 0U;
    bus->recovery_count = 0U;
    return true;
}

i2c_bus_status_t i2c_bus_write(i2c_bus_t *bus, uint8_t address_7bit, const uint8_t *data, size_t length)
{
    if (!bus || !data || length == 0U || address_7bit > 0x7FU) return I2C_BUS_ERROR;
    bus->write_count++;
    i2c_bus_status_t status = bus->ops.write(bus->ops.context, address_7bit, data, length);
    count_status(bus, status);
    return status;
}

i2c_bus_status_t i2c_bus_read(i2c_bus_t *bus, uint8_t address_7bit, uint8_t *data, size_t length)
{
    if (!bus || !data || length == 0U || address_7bit > 0x7FU) return I2C_BUS_ERROR;
    bus->read_count++;
    i2c_bus_status_t status = bus->ops.read(bus->ops.context, address_7bit, data, length);
    count_status(bus, status);
    return status;
}

bool i2c_bus_recover(i2c_bus_t *bus)
{
    if (!bus) return false;
    const bool ok = bus->ops.recover(bus->ops.context);
    if (ok) bus->recovery_count++;
    else bus->error_count++;
    return ok;
}
