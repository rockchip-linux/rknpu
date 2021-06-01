#!/bin/bash
###
 # @Author: Chifred
 # @Date: 2021-01-28 11:27:49
 # @LastEditTime: 2021-06-01 11:21:12
 # @Editors: Chifred
 # @Description: TODO
### 

chip_dir="rv1109_rv1126"
./rknn_yolov5_demo  model/${chip_dir}/yolov5s_relu_rv1109_rv1126_out_opt.rknn  model/test2.bmp


