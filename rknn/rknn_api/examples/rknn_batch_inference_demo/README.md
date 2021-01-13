## build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_batch_inference_demo /userdata/
```

## run

```
adb shell
cd /userdata/rknn_batch_inference_demo/
```

- rk180x
```
./run_rk180x.sh
```

- rv1109/rv1126
```
./run_rv1109_rv1126.sh
```
