# Task Notification 笔记

Task Notification Value 可以作为计数器或位图。`ulTaskNotifyTake(pdFALSE, ...)` 每次减 1；`pdTRUE` 直接清零。`eSetBits` 用于把多个事件合并到一个通知值中。ISR 中使用 `FromISR` API，并配合 `portYIELD_FROM_ISR()`。
