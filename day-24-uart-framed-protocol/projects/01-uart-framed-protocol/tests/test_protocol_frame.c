#include <assert.h>
#include <string.h>
#include "protocol/protocol_frame.h"
int main(void){protocol_frame_t tx={0x33U,7U,4U,{1U,2U,3U,4U}},rx;uint8_t raw[PROTOCOL_MAX_FRAME_SIZE];size_t n=protocol_frame_encode(&tx,raw,sizeof(raw));assert(n==9U);assert(protocol_frame_decode(raw,n,&rx));assert(rx.command==tx.command&&rx.sequence==tx.sequence&&rx.length==tx.length);assert(memcmp(rx.payload,tx.payload,tx.length)==0);raw[n-1U]^=1U;assert(!protocol_frame_decode(raw,n,&rx));return 0;}
