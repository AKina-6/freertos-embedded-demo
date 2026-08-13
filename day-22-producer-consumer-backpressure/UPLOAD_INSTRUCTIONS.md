# Day 22 上传说明

目标仓库：`freertos-embedded-demo`

```bash
cd projects/05-producer-consumer-backpressure
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/producer_consumer_sim
```

STM32 中创建长度 8 的 `sampleQueue`，Producer 每 100 ms 生产，Consumer 示例故意每 250 ms 消费，以观察积压、Queue Full 和 Backpressure。

```bash
git pull
git add .
git commit -m "feat: add producer consumer ring buffer and backpressure demo"
git push
```
