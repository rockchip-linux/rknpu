#!/bin/bash

chip_dir="rk180x"
echo 'run non-zero-copy model'
./rknn_zero_copy_demo  model/${chip_dir}/yolov5s_relu_rk180x.rknn  model/test1.jpg 0
echo 'run zero-copy model'
./rknn_zero_copy_demo  model/${chip_dir}/yolov5s_relu_rk180x_NHWC.rknn  model/test1.jpg 1



