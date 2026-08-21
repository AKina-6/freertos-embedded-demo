# Project 05：CAN Message Model、Standard ID、DLC、Filter 与多节点通信

## 今日目标

- 经典 CAN Standard ID：11 bit，`0x000~0x7FF`
- DLC：`0~8`
- ID/Mask Filter
- 多节点广播模型
- 总线仲裁
- STM32F103 bxCAN 发送、Filter、RX FIFO 示例

## 仲裁

多个节点同时发送 `0x080 / 0x105 / 0x120` 时，标准数据帧在相同条件下数值更小的 ID 赢得仲裁：

```text
0x080 → 0x105 → 0x120
```

这是 CAN 位级仲裁的结果，不能把它理解成软件里额外维护的 priority 字段。

## Filter

```text
(frame_id & mask) == (filter_id & mask)
```

例如 `ID=0x100, MASK=0x7F0` 匹配 `0x100~0x10F`。

## 硬件

STM32 bxCAN 需要外部 CAN transceiver，例如合适电压域的 CAN 收发器。MCU CAN_TX/CAN_RX 不能直接接 CANH/CANL。总线两端还需按实际拓扑配置 120 Ω 终端电阻。

## Host 验证

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/can_bus_sim
```
