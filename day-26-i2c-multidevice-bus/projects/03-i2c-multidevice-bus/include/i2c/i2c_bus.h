#ifndef I2C_BUS_H
#define I2C_BUS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    I2C_BUS_OK = 0,
    I2C_BUS_NACK,
    I2C_BUS_TIMEOUT,
    I2C_BUS_ERROR
} i2c_bus_status_t;

typedef struct {
    void *context;
    i2c_bus_status_t (*write)(void *context, uint8_t address_7bit, const uint8_t *data, size_t length);
    i2c_bus_status_t (*read)(void *context, uint8_t address_7bit, uint8_t *data, size_t length);
    bool (*recover)(void *context);
} i2c_bus_ops_t;

typedef struct {
    i2c_bus_ops_t ops;
    uint32_t write_count;
    uint32_t read_count;
    uint32_t nack_count;
    uint32_t timeout_count;
    uint32_t error_count;
    uint32_t recovery_count;
} i2c_bus_t;

bool i2c_bus_init(i2c_bus_t *bus, const i2c_bus_ops_t *ops);
i2c_bus_status_t i2c_bus_write(i2c_bus_t *bus, uint8_t address_7bit, const uint8_t *data, size_t length);
i2c_bus_status_t i2c_bus_read(i2c_bus_t *bus, uint8_t address_7bit, uint8_t *data, size_t length);
bool i2c_bus_recover(i2c_bus_t *bus);
#endif
