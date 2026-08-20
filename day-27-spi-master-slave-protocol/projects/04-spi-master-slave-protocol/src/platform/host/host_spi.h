#ifndef HOST_SPI_H
#define HOST_SPI_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    bool cs_active;
    uint32_t assert_count;
    uint32_t deassert_count;
    uint32_t transfer_count;
    uint32_t timeout_count;
    bool fail_next_transfer;
} host_spi_t;

bool host_spi_cs_assert(void *context);
void host_spi_cs_deassert(void *context);
bool host_spi_transfer(void *context,const uint8_t *tx,uint8_t *rx,size_t length,uint32_t timeout_ms);
#endif
