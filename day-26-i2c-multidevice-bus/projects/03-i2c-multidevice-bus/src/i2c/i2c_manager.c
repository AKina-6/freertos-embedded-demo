#include "i2c/i2c_manager.h"
#include <stddef.h>

void i2c_manager_init(i2c_manager_t *manager, i2c_bus_t *bus, uint8_t retry_limit)
{
    if (!manager) return;
    manager->bus = bus;
    manager->retry_limit = retry_limit;
    manager->transaction_success = 0U;
    manager->retry_count = 0U;
    manager->recovery_attempts = 0U;
}

static bool should_recover(i2c_bus_status_t s)
{
    return s == I2C_BUS_TIMEOUT || s == I2C_BUS_ERROR;
}

i2c_bus_status_t i2c_manager_write(i2c_manager_t *manager, uint8_t address_7bit, const uint8_t *data, uint8_t length)
{
    if (!manager || !manager->bus) return I2C_BUS_ERROR;
    i2c_bus_status_t status = I2C_BUS_ERROR;
    for (uint8_t attempt=0U; attempt<=manager->retry_limit; ++attempt) {
        status = i2c_bus_write(manager->bus, address_7bit, data, length);
        if (status == I2C_BUS_OK) { manager->transaction_success++; return status; }
        if (attempt < manager->retry_limit) manager->retry_count++;
        if (should_recover(status)) { manager->recovery_attempts++; (void)i2c_bus_recover(manager->bus); }
    }
    return status;
}

i2c_bus_status_t i2c_manager_read(i2c_manager_t *manager, uint8_t address_7bit, uint8_t *data, uint8_t length)
{
    if (!manager || !manager->bus) return I2C_BUS_ERROR;
    i2c_bus_status_t status = I2C_BUS_ERROR;
    for (uint8_t attempt=0U; attempt<=manager->retry_limit; ++attempt) {
        status = i2c_bus_read(manager->bus, address_7bit, data, length);
        if (status == I2C_BUS_OK) { manager->transaction_success++; return status; }
        if (attempt < manager->retry_limit) manager->retry_count++;
        if (should_recover(status)) { manager->recovery_attempts++; (void)i2c_bus_recover(manager->bus); }
    }
    return status;
}
