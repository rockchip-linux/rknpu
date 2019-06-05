## build

```
mkdir build && cd build
cmake ..
make
make install
```

## install

```
adb push install/rknn_yolo_demo /userdata/
```

## run
```
adb shell
cd /userdata/rknn_yolo_demo/
./rknn_yolo_demo yolov3.rknn dog.jpg
```
