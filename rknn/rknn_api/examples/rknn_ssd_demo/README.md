# Example of ssd_mobilenet_v1

## Model Source

### Original model
The model comes from TensorFlow's official model zoo:
https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/tf1_detection_zoo.md

### Convert to RKNN model
Please refer to the example in the RKNN Toolkit project to generate the RKNN model:
https://github.com/rockchip-linux/rknn-toolkit/tree/master/examples/tensorflow/ssd_mobilenet_v1

Note:
- Change the target platform of the RKNN model through the target paramter of test script.

### Export pre-compiled RKNN model
Please refer to the example in the RKNN Toolkit project to export the pre-compiled RKNN model:
https://github.com/rockchip-linux/rknn-toolkit/tree/master/examples/common_function_demos/export_rknn_precompile_model

Note:
- Please specify the correct target platform when exporting the pre-compiled RKNN model.

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
./rknn_ssd_demo model/ssd_mobilenet_v1_rk180x.rknn model/road.bmp
```

- RV1109/RV1126
```
./rknn_ssd_demo model/ssd_mobilenet_v1_rv1109_rv1126.rknn model/road.bmp
```

## Expected results

The test result should be similar to picutre `ref_detect_result.bmp`.  
Reference labels, coordinates, and scores:
```
person @ (16 125 57 205) 0.988352
person @ (208 114 256 220) 0.972152
person @ (113 118 151 189) 0.950763
car @ (143 132 214 174) 0.888194
bicycle @ (173 157 280 233) 0.855247
car @ (137 124 162 145) 0.791216
person @ (81 133 91 157) 0.573479
car @ (0 136 12 154) 0.463060
person @ (96 135 108 163) 0.426520
```

- Different platforms, different versions of tools and drivers may have slightly different results.
