# Example of rknn batch size

## Model Source

### Original model
The model used in this example come from the TensorFlow Lite offical model zoo:
https://github.com/tensorflow/models/blob/master/research/slim/nets/mobilenet_v1.md

### Convert to RKNN model
Please refer to the example in the RKNN Toolkit project to generate the RKNN model:
hhttps://github.com/rockchip-linux/rknn-toolkit/tree/master/examples/common_function_demos/batch_size
*Note: set `rknn_batch_size` in `build` interface.*


## Build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```
./build.sh
```

## Install

Copy install/rknn_batch_inference_demo to the devices under /userdata/.

- If you use rockchip's evb board, you can use the following way:

Connect device and push the program and rknn model to `/userdata`

```
adb push install/rknn_batch_inference_demo /userdata/
```

If your board has sshd service, you can use scp or other methods to copy the program and rknn model to the board.

## Run

```
adb shell
cd /userdata/rknn_batch_inference_demo/
```

- RK1808/RK1806
```
./run_rk180x.sh
```

- RV1109/RV1126
```
./run_rv1109_rv1126.sh
```

## Expected results

This example will print the TOP5 labels and corresponding scores of the test image classification results. For example, the inference results of this example are as follows:
```
 --- Top5 ---
156: 0.867188
155: 0.084045
205: 0.012451
284: 0.005924
194: 0.001658
 --- Top5 ---
156: 0.867188
155: 0.084045
205: 0.012451
284: 0.005924
194: 0.001658
 --- Top5 ---
156: 0.867188
155: 0.084045
205: 0.012451
284: 0.005924
194: 0.001658
 --- Top5 ---
156: 0.867188
155: 0.084045
205: 0.012451
284: 0.005924
194: 0.001658
```

1. The label index with the highest score is 156, the corresponding label is `Pekinese, Pekingese, Peke`.
2. Because the image library used is different, the preprocessing methods such as scaling are also slightly different, so the inference results of C Demo will be slightly different from those of Python Demo.
3. The top-5 perdictions of 4 inputs should be exactly the same.
4. Different platforms, different versions of tools and drivers may have slightly different results.


## Notes

- Please use the correct cross-compilation tool, otherwise the following error may be reported when running the demo:
  ```
  ./rknn_batch_inference_demo: not found
  ```