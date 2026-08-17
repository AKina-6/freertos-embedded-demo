# UART Framed Protocol 学习笔记

UART 只提供连续字节流，没有消息边界。应用协议通过 SOF 找起点、LEN 判断长度、CRC 检查完整性、SEQ 支持请求响应配对和后续 ACK/重传。Parser 不能假设一次 UART Receive 等于一帧。
