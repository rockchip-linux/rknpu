## build

```
mkdir build && cd build
cmake ..
make
make install
```

## install

```
adb push install/rknn_ssd_demo /userdata/
```

## run
```
adb shell
cd /userdata/rknn_ssd_demo/
./rknn_ssd_demo ssd_inception_v2.rknnn road.bmp
```
