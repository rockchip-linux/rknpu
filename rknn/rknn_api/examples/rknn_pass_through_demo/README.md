# Example of pass through input data


## Model Source

### Original model
The model used in this example come from the TensorFlow Lite offical model zoo:
https://github.com/tensorflow/models/blob/master/research/slim/nets/mobilenet_v1.md

### Convert to RKNN model
Please refer to the example in the RKNN Toolkit project to generate the RKNN model:
https://github.com/rockchip-linux/rknn-toolkit/tree/master/examples/tflite/mobilenet_v1

Note:
- Change the target platform of the RKNN model through the target paramter of test script.
- Modify the test scipt to adjust the quantization data type. Set the parameter `do_quantization` of the `build` interface to False to generate a floating-point RKNN model. Modify the `quantized_dtype` parameter in the `config` interface to generate quantized RKNN models of different data types.

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

Copy install/rknn_pass_through_demo to the devices under /userdata/.

- If you use rockchip's evb board, you can use the following way:

Connect device and push the program and rknn model to `/userdata`

```
adb push install/rknn_pass_through_demo /userdata/
```

- If your board has sshd service, you can use scp or other methods to copy the program and rknn model to the board.

## Run

```
adb shell
cd /userdata/rknn_pass_through_demo/
```

- RK1808/RK1806
```
./run_rk180x.sh int8
```

- RV1109/RV1126
```
./run_rv1109_rv1126.sh int8
```

## Expected results

This example will print the TOP5 labels and corresponding scores of the test image classification results.
- The float RKNN model inference commands and reference results are as follows：
```
./run_rv1109_rv1126.sh fp

 --- Top5 ---
156: 0.948730
155: 0.046509
205: 0.003265
284: 0.000794
260: 0.000104
```
- The int16 quantized RKNN model inference commands and reference results are as follows:
```
./run_rv1109_rv1126.sh int16

 --- Top5 ---
156: 0.948242
155: 0.047150
205: 0.003283
284: 0.000778
260: 0.000107
```
- The int8 quantized RKNN model inference commands and reference results are as follows:
```
./run_rv1109_rv1126.sh int8

 --- Top5 ---
156: 0.723633
155: 0.266357
205: 0.003798
284: 0.002304
260: 0.001088
```
- The uint8 quantized RKNN model inference commands and reference results are as follows:
```
./run_rv1109_rv1126.sh uint8

 --- Top5 ---
156: 0.865723
155: 0.083862
205: 0.012428
284: 0.005913
194: 0.001842
```

1. The label index with the highest score is 156, the corresponding label is `Pekinese, Pekingese, Peke`.
2. Because the image library used is different, the preprocessing methods such as scaling are also slightly different, so the inference results of C Demo will be slightly different from those of Python Demo.
3. There will be some differences in the results obtained by different quantized data types.
4. Different platforms, different versions of tools and drivers may have slightly different results.


## Notes

- Please use the correct cross-compilation tool, otherwise the following error may be reported when running the demo:
  ```
  ./run_rv1109_rv1126.sh: line 9: ./rknn_pass_through_demo: cannot execute binary file: Exec format error
  ```