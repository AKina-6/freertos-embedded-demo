# Producer-Consumer 与 Backpressure 学习笔记

## 生产速率与消费速率

若生产率 λ 大于消费率 μ，长期运行时队列必然增长到容量上限。

## Ring Buffer

数组大小固定，head/tail 循环移动，适合嵌入式系统避免动态内存。

## High Watermark

记录历史最大队列占用，可以帮助判断系统是否接近拥塞。

## Backpressure 的本质

Buffer 只能吸收短期突发，无法解决长期 `λ > μ`。长期稳定需要让生产、消费或丢弃策略重新达到平衡。

## FreeRTOS

Queue 自带阻塞等待，因此可直接形成生产者背压。timeout 为 0 表示立即失败；有限 timeout 表示允许短暂等待；`portMAX_DELAY` 表示持续等待。
