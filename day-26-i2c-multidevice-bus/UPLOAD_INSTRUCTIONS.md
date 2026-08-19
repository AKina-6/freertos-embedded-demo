# Day 26 上传说明

目标仓库：`embedded-communication-lab`

```bash
cd projects/03-i2c-multidevice-bus
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/i2c_multidevice_sim
```

STM32 集成时重点核对：

1. 所有设备使用 7-bit 地址记录；
2. HAL 调用处统一左移一位；
3. 确认 SDA/SCL 上拉电阻；
4. Timeout/Error 后才能考虑 Bus Recovery；
5. SCL 9 pulse 的 GPIO 代码需按实际板卡配置实现。

Git：

```bash
git pull
git add .
git commit -m "feat: add I2C multi-device bus manager and recovery demo"
git push
```
