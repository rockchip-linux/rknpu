#!/bin/bash

chip_dir="rk180x"
echo '********************* run non-zero-copy model ***************************************'
./rknn_zero_copy_demo  model/${chip_dir}/yolov5s_relu_${chip_dir}.rknn  model/bus.bmp 0
echo '********************* run non-zero-copy model(enable output optimize) ***************'
./rknn_zero_copy_demo  model/${chip_dir}/yolov5s_relu_${chip_dir}_out_opt.rknn  model/bus.bmp 0
echo '********************* run zero-copy model     ***************************************'
./rknn_zero_copy_demo  model/${chip_dir}/yolov5s_relu_${chip_dir}_NHWC.rknn  model/bus.bmp 1



