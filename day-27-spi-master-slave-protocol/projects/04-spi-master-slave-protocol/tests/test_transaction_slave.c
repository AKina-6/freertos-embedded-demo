#include <assert.h>
#include "spi/spi_transaction.h"
#include "spi/spi_slave_model.h"
#include "host_spi.h"
int main(void){
 spi_slave_model_t s; spi_slave_model_init(&s); spi_packet_t req={SPI_CMD_WRITE_CONFIG,1U,1U,{77U}},resp;
 assert(spi_slave_model_handle(&s,&req,&resp)==SPI_STATUS_OK); assert(s.config_value==77U);
 host_spi_t h={0}; spi_bus_t bus={&h,host_spi_cs_assert,host_spi_transfer,host_spi_cs_deassert}; spi_master_t m;
 assert(spi_master_init(&m,&bus)); uint8_t tx[2]={0xAA,0x55},rx[2]; assert(spi_master_transfer(&m,tx,rx,2U,10U));
 assert(rx[0]==0x55 && rx[1]==0xAA); assert(m.cs_assert_count==1U && m.cs_deassert_count==1U);
 h.fail_next_transfer=true; assert(!spi_master_transfer(&m,tx,rx,2U,10U)); assert(!h.cs_active); assert(m.transfer_failures==1U); return 0; }
