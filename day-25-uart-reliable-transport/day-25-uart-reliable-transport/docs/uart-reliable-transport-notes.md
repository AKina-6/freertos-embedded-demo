# UART Reliable Transport 学习笔记

ACK 确认某个 Sequence 的请求。Timeout 可能来自请求丢失、ACK 丢失、处理过慢或对端掉线，因此采用有限重试。无限重试会造成永久阻塞和链路拥塞。

可靠请求的基本事务：`Send → WAIT_ACK → ACK/Timeout → Success/Retry/Failed`。接收端应使用 Sequence 去重，避免 ACK 丢失导致同一副作用命令被重复执行。
