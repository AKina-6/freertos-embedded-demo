# Project 04：SPI Master-Slave Protocol、Full-Duplex Transfer 与 Chip Select

## 目标
- SPI 全双工事务
- CS 拉低/拉高生命周期
- Master 统一事务封装
- 简单 Command/Sequence/Length/Payload/Checksum 协议
- Slave 命令处理模型
- Timeout 后确保 CS 被释放

## SPI 事务
```text
CS ↓
Master TX: b0 b1 b2 b3
Slave  TX: r0 r1 r2 r3
CS ↑
```
每产生一个 SCK，MOSI 与 MISO 同时移位，因此 SPI 天然是 Full-Duplex。

## 协议
```text
SOF | CMD | SEQ | LEN | PAYLOAD | CHECKSUM
5A
```

## STM32 默认配置
- STM32F103C8T6
- SPI1
- PA5 SCK
- PA6 MISO
- PA7 MOSI
- PB0 Software CS
- Mode 0: CPOL=0, CPHA=0
- MSB First

## Host 验证
```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/spi_protocol_sim
```
