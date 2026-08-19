# I²C 多设备与总线恢复学习笔记

## 地址冲突

同一 I²C Bus 上两个设备不能同时使用相同固定地址，否则 Master 无法区分响应者。

解决方式包括：

- 使用器件地址选择脚；
- 更换地址不同的器件；
- I²C multiplexer；
- 使用独立 I²C 控制器。

## NACK

常见原因：

- 地址不存在；
- 设备忙；
- 设备未上电；
- 时序或上拉问题。

NACK 不等于总线一定卡死，所以本项目不会对每次 NACK 都执行 9 脉冲恢复。

## Timeout / BUSY stuck

更可能需要 Bus Recovery。

## SCL 9 Pulses

思想是给可能卡在字节传输中间的 Slave 足够时钟，使其释放 SDA。之后 Master 再产生 STOP 并重新初始化 I²C 外设。
