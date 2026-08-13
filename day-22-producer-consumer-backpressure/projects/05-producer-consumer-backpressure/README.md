# Project 05：Producer-Consumer、Ring Buffer 与 Backpressure

## 项目目标

理解生产者速度高于消费者时系统会发生什么，并实现：

- Producer / Consumer；
- 固定容量 Ring Buffer；
- High Watermark；
- Drop Newest；
- Drop Oldest；
- Producer Throttling；
- FreeRTOS Queue Backpressure。

## 基本模型

```text
Producer -> Buffer/Queue -> Consumer
```

如果：

```text
Producer = 100 ms / sample
Consumer = 250 ms / sample
```

积压会逐渐增长，最终 Queue 满。

## Backpressure

Backpressure 表示下游处理能力不足时，上游需要感知并采取措施。常见策略：

1. 阻塞 Producer，等待空间；
2. 降低 Producer 采样速率；
3. 丢弃最新数据；
4. 丢弃最旧数据；
5. 扩大 Buffer；
6. 提升 Consumer 吞吐。

## Ring Buffer

固定容量为 8：

```text
head -> 下一次写入
 tail -> 下一次读取
count -> 当前元素数
```

所有 Push/Pop 都是 O(1)。

### Drop Newest

Buffer 满后拒绝新样本。适合历史数据更重要的场景。

### Drop Oldest

Buffer 满后覆盖最旧样本。适合实时传感器、UI、遥测等“最新状态更重要”的场景。

## FreeRTOS Queue

```c
xQueueSend(
    sampleQueue,
    &sample,
    pdMS_TO_TICKS(20)
);
```

队列满时最多等待 20 ms，这就是一种有限阻塞 Backpressure。

监控：

```c
uxQueueMessagesWaiting(sampleQueue);
uxQueueSpacesAvailable(sampleQueue);
```

## Host 测试

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/producer_consumer_sim
```
