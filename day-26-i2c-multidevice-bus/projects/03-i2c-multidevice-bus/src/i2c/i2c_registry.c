#include "i2c/i2c_registry.h"
#include <stddef.h>

void i2c_registry_init(i2c_registry_t *registry)
{
    if (!registry) return;
    registry->count = 0U;
    registry->duplicate_rejects = 0U;
}

const i2c_device_t *i2c_registry_find(const i2c_registry_t *registry, uint8_t address_7bit)
{
    if (!registry) return NULL;
    for (size_t i=0U; i<registry->count; ++i) {
        if (registry->devices[i].address_7bit == address_7bit) return &registry->devices[i];
    }
    return NULL;
}

bool i2c_registry_add(i2c_registry_t *registry, uint8_t address_7bit, const char *name)
{
    if (!registry || !name || address_7bit > 0x7FU || registry->count >= I2C_REGISTRY_MAX_DEVICES) return false;
    if (i2c_registry_find(registry, address_7bit)) {
        registry->duplicate_rejects++;
        return false;
    }
    registry->devices[registry->count].address_7bit = address_7bit;
    registry->devices[registry->count].name = name;
    registry->count++;
    return true;
}
