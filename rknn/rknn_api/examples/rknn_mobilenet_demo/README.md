## build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_mobilenet_demo /userdata/
```

## run

```
adb shell
cd /userdata/rknn_mobilenet_demo/
```

- rk180x
```
./rknn_mobilenet_demo model/mobilenet_v1_rk180x.rknn model/dog_224x224.jpg
```

- rv1109/rv1126
```
./rknn_mobilenet_demo model/mobilenet_v1_rv1109_rv1126.rknn model/dog_224x224.jpg
```
