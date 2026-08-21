# Day 28 上传说明

目标仓库：`embedded-communication-lab`

```bash
cd projects/05-can-message-model
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/can_bus_sim

git add .
git commit -m "feat: add CAN standard frame model filters and multi-node arbitration demo"
git push
```
