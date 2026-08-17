#include <assert.h>
#include <string.h>
#include "protocol/protocol_frame.h"
#include "protocol/stream_parser.h"
int main(void){protocol_frame_t a={0x10U,1U,2U,{0xAAU,0x55U}},b={0x20U,2U,1U,{0x42U}};uint8_t ra[PROTOCOL_MAX_FRAME_SIZE],rb[PROTOCOL_MAX_FRAME_SIZE];size_t na=protocol_frame_encode(&a,ra,sizeof(ra)),nb=protocol_frame_encode(&b,rb,sizeof(rb));uint8_t s[64];size_t p=0U;s[p++]=0x99U;memcpy(&s[p],ra,na);p+=na;memcpy(&s[p],rb,nb);p+=nb;stream_parser_t parser;stream_parser_init(&parser);protocol_frame_t out[2];size_t n=stream_parser_feed(&parser,s,p,out,2U);assert(n==2U);assert(out[0].command==0x10U&&out[1].command==0x20U);stream_parser_init(&parser);bool got=false;protocol_frame_t split;for(size_t i=0;i<na;++i)if(stream_parser_feed_byte(&parser,ra[i],&split))got=true;assert(got&&split.sequence==1U);return 0;}
