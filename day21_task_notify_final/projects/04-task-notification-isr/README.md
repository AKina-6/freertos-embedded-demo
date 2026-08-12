# Project 04：Direct-to-Task Notification 与 ISR

## 核心机制

- `vTaskNotifyGiveFromISR()` + `ulTaskNotifyTake()`：计数型通知
- `xTaskNotifyFromISR(..., eSetBits, ...)` + `xTaskNotifyWait()`：Bit 型通知

## 典型结构

```text
ADC/DMA ISR -> Counting Notification -> AdcTask
Button/UART ISR -> Bit Notification -> EventTask
```

## 选择建议

| 需求 | 推荐 |
|---|---|
| ISR 唤醒单个 Task | Task Notification |
| DMA 完成计数 | Task Notification |
| 多事件 Bit 通知一个 Task | Task Notification |
| 传结构体数据 | Queue |
| 保护共享资源 | Mutex |
| 多 Task 等待系统状态 | Event Group |

## 自动测试

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/task_notification_sim
```
