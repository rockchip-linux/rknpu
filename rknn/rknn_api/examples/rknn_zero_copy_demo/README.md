# Zero copy demo
In specific case, the number of input data copies can be reduced to 0, that is, zero copy. For example, when the RKNN model is asymmetric quantization, the quantization data type is uint8, the mean value of the 3 channels is the same integer and the scaling factor is the same, the normalization and quantization can be omitted. 

This demo compares the performance difference between zero-copy and normal process inference.

a. zero-copy inference uses the "rknn_inputs_map" interface

b. normal inference uses the "rknn_inputs_set" interface.

In the "model" directory, models without NHWC suffix have input in NCHW format, and models with NHWC suffix have input in NHWC format. The model with NHWC suffix is ​​exported using rknn-toolkit by adding the following parameters
```
rknn.config(force_builtin_perm=True)
```
Note: if it shows "get unvalid input physical address, please extend in/out memory space", you may need to reboot.

## build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_zero_copy_demo /userdata/
```

## run

```
adb shell
cd /userdata/rknn_zero_copy_demo/
```

- rk180x
```
./run_rk180x.sh
```

- rv1109/rv1126
```
./run_rv1109_rv1126.sh
```
