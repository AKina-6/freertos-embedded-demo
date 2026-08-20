# Day 27 上传说明
目标仓库：`embedded-communication-lab`

```bash
cd projects/04-spi-master-slave-protocol
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/spi_protocol_sim
```

STM32：配置 SPI1 Mode 0、PA5/PA6/PA7，PB0 为软件 CS；调用 `HAL_SPI_TransmitReceive()` 完成全双工事务。

```bash
git pull
git add .
git commit -m "feat: add SPI master slave protocol and chip select transaction demo"
git push
```
