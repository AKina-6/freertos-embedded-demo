#ifndef I2C_REGISTRY_H
#define I2C_REGISTRY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define I2C_REGISTRY_MAX_DEVICES 8U

typedef struct {
    uint8_t address_7bit;
    const char *name;
} i2c_device_t;

typedef struct {
    i2c_device_t devices[I2C_REGISTRY_MAX_DEVICES];
    size_t count;
    uint32_t duplicate_rejects;
} i2c_registry_t;

void i2c_registry_init(i2c_registry_t *registry);
bool i2c_registry_add(i2c_registry_t *registry, uint8_t address_7bit, const char *name);
const i2c_device_t *i2c_registry_find(const i2c_registry_t *registry, uint8_t address_7bit);
#endif
