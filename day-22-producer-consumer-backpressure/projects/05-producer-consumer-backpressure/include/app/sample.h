#ifndef SAMPLE_H
#define SAMPLE_H
#include <stdint.h>
typedef struct {
    uint32_t sequence;
    uint32_t timestamp_ms;
    int32_t value;
} sample_t;
#endif
