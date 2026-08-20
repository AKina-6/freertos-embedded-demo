#ifndef SPI_TRANSACTION_H
#define SPI_TRANSACTION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    void *context;
    bool (*cs_assert)(void *context);
    bool (*transfer)(void *context, const uint8_t *tx, uint8_t *rx, size_t length, uint32_t timeout_ms);
    void (*cs_deassert)(void *context);
} spi_bus_t;

typedef struct {
    spi_bus_t bus;
    uint32_t transactions;
    uint32_t transfer_failures;
    uint32_t cs_assert_count;
    uint32_t cs_deassert_count;
} spi_master_t;

bool spi_master_init(spi_master_t *master, const spi_bus_t *bus);
bool spi_master_transfer(spi_master_t *master, const uint8_t *tx, uint8_t *rx, size_t length, uint32_t timeout_ms);
#endif
