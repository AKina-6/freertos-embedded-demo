# CAN Message Model 学习笔记

- Standard CAN Identifier：11 bit。
- Classic CAN DLC 最大为 8。
- CAN 是多主机广播总线。
- 更低数值的标准 ID 在仲裁中通常具有更高优先级。
- Filter 用于让节点只接收关心的消息。
- STM32 bxCAN 是控制器；实际差分物理层仍需 CAN Transceiver。
- 实物必须继续核对 Bit Rate、Sample Point、时钟、终端电阻、节点拓扑与引脚映射。
