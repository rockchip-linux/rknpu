## build

```
mkdir build && cd build
cmake ..
make
make install
```

## install

```
adb push install/rknn_mobilenet_demo /userdata/
```

## run
```
adb shell
cd /userdata/rknn_mobilenet_demo/
./rknn_mobilenet_demo mobilenet_v1.rknnn dog_224x224.jpg
```
