#include <assert.h>
#include <string.h>
#include "spi/spi_protocol.h"
int main(void){
 spi_packet_t a={SPI_CMD_WRITE_CONFIG,7U,2U,{0x11U,0x22U}},b; uint8_t raw[SPI_PROTOCOL_MAX_FRAME];
 size_t n=spi_protocol_encode(&a,raw,sizeof(raw)); assert(n==7U); assert(spi_protocol_decode(raw,n,&b));
 assert(b.command==a.command && b.sequence==7U && b.length==2U); assert(memcmp(a.payload,b.payload,2U)==0);
 raw[n-1U]^=1U; assert(!spi_protocol_decode(raw,n,&b)); return 0; }
