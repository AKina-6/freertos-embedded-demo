#include "app/task_notification_core.h"
#include <stddef.h>
void task_notification_core_init(task_notification_core_t *n){if(!n)return;n->count=n->bits=n->give_count=n->take_count=n->bit_set_count=0U;}
void task_notification_core_give(task_notification_core_t *n){if(!n)return;n->count++;n->give_count++;}
uint32_t task_notification_core_take(task_notification_core_t *n,bool clear){if(!n||n->count==0U)return 0U;uint32_t v=n->count;n->take_count++;if(clear)n->count=0U;else n->count--;return v;}
void task_notification_core_set_bits(task_notification_core_t *n,uint32_t bits){if(!n)return;n->bits|=bits;n->bit_set_count++;}
uint32_t task_notification_core_wait_bits(task_notification_core_t *n,uint32_t wait,uint32_t clear){if(!n)return 0U;uint32_t m=n->bits&wait;if(m)n->bits&=~clear;return m;}
