#ifndef HOST_I2C_H
#define HOST_I2C_H
#include <stdbool.h>
#include <stdint.h>
#include "i2c/i2c_bus.h"

typedef struct {
    i2c_bus_status_t next_write_status;
    i2c_bus_status_t next_read_status;
    uint8_t read_data[8];
    uint32_t recover_calls;
    uint32_t scl_pulses;
    uint32_t writes;
    uint32_t reads;
} host_i2c_t;

i2c_bus_status_t host_i2c_write(void *context, uint8_t address_7bit, const uint8_t *data, size_t length);
i2c_bus_status_t host_i2c_read(void *context, uint8_t address_7bit, uint8_t *data, size_t length);
bool host_i2c_recover(void *context);
#endif
