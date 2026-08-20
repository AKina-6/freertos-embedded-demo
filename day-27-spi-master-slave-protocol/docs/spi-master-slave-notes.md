# SPI Master-Slave 学习笔记

## Full-Duplex
SPI 每个时钟同时发送和接收一位。即使 Master 只想读，也要发送 dummy bytes 来产生 SCK。

## Chip Select
一个完整逻辑事务通常要求 CS 在整个命令期间保持有效：
```text
CS low -> command/address/data -> CS high
```
不应在一个原子事务中间随意抬高 CS。

## 多 Slave
SCK/MOSI/MISO 可共享，但每个 Slave 通常需要独立 CS。

## Timeout
无论传输成功或失败，都要执行 CS deassert，避免外设长期停留在半个事务中。

## Mode
CPOL/CPHA 必须与 Slave 匹配，否则采样边沿错误会导致数据整体错位。
