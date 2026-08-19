#ifndef I2C_MANAGER_H
#define I2C_MANAGER_H
#include <stdbool.h>
#include <stdint.h>
#include "i2c/i2c_bus.h"

typedef struct {
    i2c_bus_t *bus;
    uint8_t retry_limit;
    uint32_t transaction_success;
    uint32_t retry_count;
    uint32_t recovery_attempts;
} i2c_manager_t;

void i2c_manager_init(i2c_manager_t *manager, i2c_bus_t *bus, uint8_t retry_limit);
i2c_bus_status_t i2c_manager_write(i2c_manager_t *manager, uint8_t address_7bit, const uint8_t *data, uint8_t length);
i2c_bus_status_t i2c_manager_read(i2c_manager_t *manager, uint8_t address_7bit, uint8_t *data, uint8_t length);
#endif
