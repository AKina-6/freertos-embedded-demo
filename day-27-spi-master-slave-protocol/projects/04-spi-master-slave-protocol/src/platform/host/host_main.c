#include <stdio.h>
#include "spi/spi_protocol.h"
#include "spi/spi_transaction.h"
#include "spi/spi_slave_model.h"
#include "host_spi.h"

int main(void)
{
    spi_slave_model_t slave; spi_slave_model_init(&slave);
    spi_packet_t req={SPI_CMD_WRITE_CONFIG,1U,1U,{0x2AU}}, resp;
    spi_status_t st=spi_slave_model_handle(&slave,&req,&resp);
    printf("slave_write_config status=%u config=%u seq=%u\n",st,slave.config_value,resp.sequence);

    req.command=SPI_CMD_READ_DATA; req.sequence=2U; req.length=0U;
    st=spi_slave_model_handle(&slave,&req,&resp);
    printf("slave_read_data status=%u data=0x%02X%02X requests=%lu\n",st,resp.payload[1],resp.payload[2],(unsigned long)slave.request_count);

    uint8_t encoded[SPI_PROTOCOL_MAX_FRAME];
    size_t n=spi_protocol_encode(&resp,encoded,sizeof(encoded));
    printf("encoded_response len=%lu checksum=0x%02X\n",(unsigned long)n,encoded[n-1U]);

    host_spi_t host={0};
    spi_bus_t bus={&host,host_spi_cs_assert,host_spi_transfer,host_spi_cs_deassert};
    spi_master_t master; spi_master_init(&master,&bus);
    uint8_t tx[]={0x9FU,0x00U,0x00U,0x00U}, rx[4]={0};
    bool ok=spi_master_transfer(&master,tx,rx,4U,100U);
    printf("full_duplex ok=%s rx=%02X %02X %02X %02X cs=%lu/%lu transfers=%lu\n",ok?"yes":"no",rx[0],rx[1],rx[2],rx[3],(unsigned long)master.cs_assert_count,(unsigned long)master.cs_deassert_count,(unsigned long)host.transfer_count);

    host.fail_next_transfer=true;
    ok=spi_master_transfer(&master,tx,rx,4U,100U);
    printf("timeout_case ok=%s failures=%lu host_timeouts=%lu cs_active=%s\n",ok?"yes":"no",(unsigned long)master.transfer_failures,(unsigned long)host.timeout_count,host.cs_active?"yes":"no");
    return 0;
}
