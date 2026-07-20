# FreeRTOS Embedded Demo

基于 STM32F103C8T6 的 FreeRTOS 学习仓库，用于理解任务调度、任务间通信、资源保护和实时系统设计。

## 内容

```text
examples/
├── 01-task-create/
├── 02-priority-delay/
├── 03-queue/
├── 04-semaphore/
├── 05-mutex/
├── 06-event-timer/
└── 07-multitask-monitor/
docs/
learning-log/
```

## 学习目标

- 理解任务状态和抢占式调度
- 使用队列传递传感器数据
- 使用信号量完成中断同步
- 使用互斥锁保护 I²C、UART 等共享资源
- 使用事件组协调多个系统状态
