## Build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## Install

Copy install/rknn_mobilenet_demo to the devices under /userdata/.

- If you use rockchip's evb board, you can use the following way:

Connect device and push the program and rknn model to `/userdata`

```
adb push install/rknn_mobilenet_demo /userdata/
```

If your board has sshd service, you can use scp or other methods to copy the program and rknn model to the board.

## Run

```
adb shell
cd /userdata/rknn_mobilenet_demo/
```

- RK1808/RK1806
```
./run_rk180x.sh
```

- RV1109/RV1126
```
./run_rv1109_rv1126.sh
```
