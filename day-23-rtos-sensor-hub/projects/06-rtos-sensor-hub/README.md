# Project 06：RTOS Sensor Hub 综合项目

## 目标
把 Day 18-22 的机制组合成一个完整 FreeRTOS 架构：

```text
SensorTask -> Queue -> ProcessingTask -> UART(Mutex)
     |                         |
     |                         -> OLED/UI
     |
     -> Backpressure

Software Timer -> Event Group -> Monitor/Startup
Button EXTI -> Task Notification -> UiTask
Flash/Display/Sensor Ready -> Event Group -> StartupTask
```

## 使用到的 FreeRTOS 机制
- Task：Sensor / Processing / Startup / UI
- Queue：传递传感器 Sample
- Mutex：保护共享 UART
- Event Group：系统 Ready 与 Heartbeat
- Software Timer：1 s Heartbeat
- Task Notification：按键 ISR 唤醒 UI Task
- Backpressure：Queue 满时丢弃最旧 Sample

## 默认周期
| 模块 | 周期 |
|---|---:|
| SensorTask | 500 ms |
| Heartbeat Timer | 1000 ms |
| ProcessingTask | Queue 驱动 |
| UiTask | Notification 驱动 |

## Host 验证
```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/sensor_hub_sim
```
