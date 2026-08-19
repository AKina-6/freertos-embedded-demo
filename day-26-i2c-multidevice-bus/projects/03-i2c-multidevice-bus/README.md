# Project 03：I²C Multi-device Bus、地址管理与总线故障恢复

## 项目目标

在同一条 I²C 总线上管理多个外设，并处理常见故障：

- 7-bit 地址注册；
- 地址冲突检测；
- 统一 Bus API；
- NACK；
- Timeout；
- Retry；
- Bus Recovery；
- SCL 9 pulse recovery 概念。

## 示例设备

```text
BH1750  : 0x23
SSD1306 : 0x3C
```

如果再次注册 `0x23`，Registry 会拒绝。

## 7-bit 地址

应用层保存：

```text
0x23
```

STM32 HAL 发送时转换：

```c
(uint16_t)(address_7bit << 1)
```

不要在业务层混用 7-bit 地址和已经左移后的 HAL 地址。

## Retry 策略

```text
Transaction
→ OK                : success
→ NACK              : retry
→ Timeout / Error   : recover bus + retry
```

本项目默认 `retry_limit = 1`。

## Bus Recovery

SDA 被从设备异常拉低时，常见恢复思路：

```text
Disable I2C peripheral
→ GPIO open-drain
→ release SDA
→ pulse SCL up to 9 times
→ generate STOP
→ restore AF pins
→ re-init I2C
```

具体 GPIO 操作依赖板卡和引脚配置，因此 STM32 文件中只保留明确集成位置，不伪造具体硬件恢复结果。

## Host 测试

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/i2c_multidevice_sim
```
