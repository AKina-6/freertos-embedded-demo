#include "spi/spi_transaction.h"
#include <stddef.h>

bool spi_master_init(spi_master_t *master, const spi_bus_t *bus)
{
    if (!master || !bus || !bus->cs_assert || !bus->transfer || !bus->cs_deassert) return false;
    master->bus=*bus; master->transactions=0U; master->transfer_failures=0U;
    master->cs_assert_count=0U; master->cs_deassert_count=0U; return true;
}

bool spi_master_transfer(spi_master_t *master, const uint8_t *tx, uint8_t *rx, size_t length, uint32_t timeout_ms)
{
    if (!master || !tx || !rx || length==0U) return false;
    if (!master->bus.cs_assert(master->bus.context)) return false;
    master->cs_assert_count++;
    bool ok = master->bus.transfer(master->bus.context,tx,rx,length,timeout_ms);
    master->bus.cs_deassert(master->bus.context);
    master->cs_deassert_count++;
    master->transactions++;
    if (!ok) master->transfer_failures++;
    return ok;
}
