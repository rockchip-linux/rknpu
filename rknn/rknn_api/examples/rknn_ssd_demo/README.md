## build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_ssd_demo /userdata/
```

## run

```
adb shell
cd /userdata/rknn_ssd_demo/
```

- rk180x
```
./rknn_ssd_demo model/ssd_inception_v2_rk180x.rknn model/road.bmp
```

- rv1109/rv1126
```
./rknn_ssd_demo model/ssd_inception_v2_rv1109_rv1126.rknn model/road.bmp
```
