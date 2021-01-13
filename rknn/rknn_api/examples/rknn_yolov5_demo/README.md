# Yolo-v5 demo
This Demo shows yolo-v5 RKNN model object detection. We refer to repository:https://github.com/ultralytics/yolov5. After simply modifying the original pytorch model, retrain and export the onnx model.

RKNN model is ​​exported using rknn-toolkit by adding the following parameters
```
rknn.config(output_optimize=1)
```

## build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_yolov5_demo /userdata/
```

## run

```
adb shell
cd /userdata/rknn_yolov5_demo/
```

- rk180x
```
./run_rk180x.sh
```

- rv1109/rv1126
```
./run_rv1109_rv1126.sh
```
