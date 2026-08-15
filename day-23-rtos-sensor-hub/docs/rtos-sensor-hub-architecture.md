# RTOS Sensor Hub 架构笔记

## 数据路径
```text
Producer(Task) -> Queue -> Consumer(Task)
```
Queue 负责解耦采样和处理速度；满队列时执行 Drop Oldest。

## 同步路径
```text
Startup Conditions -> Event Group -> StartupTask
```
多个模块 Ready 后才进入系统运行态。

## ISR 路径
```text
GPIO EXTI -> Task Notification -> UiTask
```
ISR 只通知，业务在 Task 执行。

## 共享资源
```text
多个 Task -> Mutex -> UART
```
避免日志交错，并利用 FreeRTOS Mutex 的 Priority Inheritance。

## 周期事件
```text
Software Timer -> Heartbeat Event Bit
```
Timer Callback 只设置 Event Bit，复杂工作交给 Task。
