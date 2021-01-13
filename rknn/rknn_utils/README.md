## build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_matmul_sample/lib/librknn_utils.so /usr/lib
adb push install/rknn_matmul_sample /userdata/
```

## run

```
adb shell
cd /userdata/rknn_matmul_sample/
```

- /rk180x/rv1109/rv1126
```
./bin/rknn_matmul_sample 256 4096
```
