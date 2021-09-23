#!/bin/bash

chip_dir="rv1109_rv1126"
./rknn_yolov5_demo  model/${chip_dir}/yolov5s_relu_rv1109_rv1126_out_opt.rknn  model/bus.bmp


