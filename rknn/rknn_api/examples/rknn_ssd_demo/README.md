## Build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## Install

Copy install/rknn_ssd_demo to the devices under /userdata/.

- If you use rockchip's evb board, you can use the following way:

Connect device and push the program and rknn model to `/userdata`

```
adb push install/rknn_ssd_demo /userdata/
```

- If your board has sshd service, you can use scp or other methods to copy the program and rknn model to the board.


## Run

```
adb shell
cd /userdata/rknn_ssd_demo/
```

- RK1808/RK1806
```
./rknn_ssd_demo model/ssd_inception_v2_rk180x.rknn model/road.bmp
```

- RV1109/RV1126
```
./rknn_ssd_demo model/ssd_inception_v2_rv1109_rv1126.rknn model/road.bmp
```
