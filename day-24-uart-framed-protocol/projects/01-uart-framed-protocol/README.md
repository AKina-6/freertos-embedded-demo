# Project 01：UART Framed Protocol、CRC 与流式 Parser

帧格式：

```text
SOF | CMD | SEQ | LEN | PAYLOAD | CRC8
 A5    1     1     1     N        1 byte
```

重点验证：帧编码/解码、CRC8、噪声前导、拆包、粘包、错误帧恢复，以及 STM32 `HAL_UARTEx_ReceiveToIdle_DMA()` 流式接收集成。

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/uart_protocol_sim
```
