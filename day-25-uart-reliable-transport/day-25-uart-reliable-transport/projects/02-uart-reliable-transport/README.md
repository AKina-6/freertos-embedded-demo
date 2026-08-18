# Project 02：UART ACK、Timeout 与 Retransmission

## 状态机

```text
IDLE
  ↓ send
WAIT_ACK
  ├─ ACK(seq match) → SUCCESS
  ├─ timeout + retry available → retransmit
  └─ timeout + retry exhausted → FAILED
```

默认示例：Timeout 200 ms，Max Retries 3。最大总发送次数为 `1 initial + 3 retries = 4`。

## ACK 配对
发送 `SEQ=42` 时，仅 `ACK SEQ=42` 完成当前 transaction。旧 ACK、重复 ACK 或其他 Sequence 会被忽略并计数。

## Timeout 与重传
主循环周期调用 `reliable_uart_poll()`。达到 deadline 且还在 `WAIT_ACK` 时执行有限次数重传；达到最大重试次数后进入 `FAILED`。

## 与 Day 24 串联

```text
Application → Reliable Sender → Frame Encoder → UART
UART RX DMA → Streaming Parser → ACK Frame → Reliable Sender
```

## 幂等性
请求已经被对端执行但 ACK 丢失时，Sender 会重发相同 Sequence。接收端应使用 Sequence 去重；对具有副作用的命令尤其重要。

## Host 验证
```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/reliable_uart_sim
```
