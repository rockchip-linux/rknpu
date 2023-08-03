# Example of zero copy

## Overview
In specific case, the number of input data copies can be reduced to 0, that is, zero copy. For example, when the RKNN model is asymmetric quantization, the quantization data type is uint8, the mean value of the 3 channels is the same integer and the scaling factor is the same, the normalization and quantization can be omitted. 

This demo compares the performance difference between zero-copy and normal process inference.

```txt
a. zero-copy inference uses the "rknn_inputs_map" interface

b. normal inference uses the "rknn_inputs_set" interface.
```

In the "model" directory, models without NHWC suffix have input in NCHW format, and models with NHWC suffix have input in NHWC format. The model with NHWC suffix is ​​exported using rknn-toolkit by adding the following parameters

```python
rknn.config(force_builtin_perm=True)
```

For detailed model conversion scripts and instructions, please refer to the [rknn_yolov5_demo](../rknn_yolov5_demo/README.md)


## Build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```sh
./build.sh
```

Notice: this model is based on rknn_yolov5_demo



## Install

Copy install/rknn_zero_copy_demo to the devices under /userdata/.

- If you use rockchip's evb board, you can use the following way:

Connect device and push the program and rknn model to `/userdata`

```sh
adb push install/rknn_zero_copy_demo /userdata/
```

- If your board has sshd service, you can use scp or other methods to copy the program and rknn model to the board.

  

## Run

```sh
adb shell
cd /userdata/rknn_zero_copy_demo/
```

- RK1808/RK1806

```sh
./run_rk180x.sh
```

- RV1109/RV1126

```sh
./run_rv1109_rv1126.sh
```

After running, three model would be invoked. 

- The 'zero_copy' would got best performance. 
- The second one was the 'normal model with output optimize'. 
- The 'normal model' would cost most time.


## Expected results

The test result should be similar to picutre `ref_detect_result.bmp`.  
Reference labels, coordinates, and scores:
```
********************* run non-zero-copy model ***************************************
person @ (208 238 287 511) 0.876641
person @ (480 240 559 525) 0.867932
person @ (108 233 230 535) 0.856740
bus @ (91 127 554 466) 0.610052
********************* run non-zero-copy model(enable output optimize) ***************
person @ (208 238 287 511) 0.876641
person @ (480 240 559 525) 0.867932
person @ (108 233 230 535) 0.856740
bus @ (91 127 554 466) 0.610052
********************* run zero-copy model     ***************************************
person @ (208 238 287 511) 0.876641
person @ (480 240 559 525) 0.867932
person @ (108 233 230 535) 0.856740
bus @ (91 127 554 466) 0.610052
```

- The inference results in the three scenarios should be the same.
- The average time-consuming in the three scenarios should be gradually reduced, and the zero-copy time-consuming is the least.
- Different platforms, different versions of tools and drivers may have slightly different results.
