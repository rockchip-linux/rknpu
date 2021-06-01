#!/bin/bash

chip_dir="rv1109_rv1126"
echo '********************* run zero-copy model     *********************'
./rknn_zero_copy_demo  model/${chip_dir}/yolov5s_relu_rv1109_rv1126_NHWC.rknn  model/test1.bmp 1

echo '********************* run non-zero-copy model *********************'
./rknn_zero_copy_demo  model/${chip_dir}/yolov5s_relu_rv1109_rv1126.rknn  model/test1.bmp 0



