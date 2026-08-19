#include <assert.h>
#include "i2c/i2c_registry.h"
int main(void){
    i2c_registry_t r; i2c_registry_init(&r);
    assert(i2c_registry_add(&r,0x23U,"BH1750"));
    assert(i2c_registry_add(&r,0x3CU,"SSD1306"));
    assert(!i2c_registry_add(&r,0x23U,"DUP"));
    assert(r.count==2U); assert(r.duplicate_rejects==1U);
    assert(i2c_registry_find(&r,0x3CU)!=0);
    return 0;
}
