# Day 25 上传说明

目标仓库：`embedded-communication-lab`

```bash
cd projects/02-uart-reliable-transport
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/reliable_uart_sim

git pull
git add .
git commit -m "feat: add UART ACK timeout and retransmission state machine"
git push
```
