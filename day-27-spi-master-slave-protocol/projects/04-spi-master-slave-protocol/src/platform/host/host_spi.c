#include "host_spi.h"

bool host_spi_cs_assert(void *context)
{
    host_spi_t *h=(host_spi_t*)context;
    if (!h || h->cs_active) return false;
    h->cs_active=true; h->assert_count++; return true;
}
void host_spi_cs_deassert(void *context)
{
    host_spi_t *h=(host_spi_t*)context;
    if (!h) return;
    h->cs_active=false;
    h->deassert_count++;
}
bool host_spi_transfer(void *context,const uint8_t *tx,uint8_t *rx,size_t length,uint32_t timeout_ms)
{
    host_spi_t *h=(host_spi_t*)context;
    if (!h || !h->cs_active || !tx || !rx) return false;
    if (h->fail_next_transfer || timeout_ms==0U) { h->fail_next_transfer=false; h->timeout_count++; return false; }
    for (size_t i=0U;i<length;++i) rx[i]=(uint8_t)(tx[i]^0xFFU);
    h->transfer_count++; return true;
}
