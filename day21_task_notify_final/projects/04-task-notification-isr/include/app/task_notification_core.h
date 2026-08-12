#ifndef TASK_NOTIFICATION_CORE_H
#define TASK_NOTIFICATION_CORE_H
#include <stdbool.h>
#include <stdint.h>
typedef struct { uint32_t count,bits,give_count,take_count,bit_set_count; } task_notification_core_t;
void task_notification_core_init(task_notification_core_t *n);
void task_notification_core_give(task_notification_core_t *n);
uint32_t task_notification_core_take(task_notification_core_t *n, bool clear_on_exit);
void task_notification_core_set_bits(task_notification_core_t *n, uint32_t bits);
uint32_t task_notification_core_wait_bits(task_notification_core_t *n, uint32_t wait_bits, uint32_t clear_bits);
#endif
