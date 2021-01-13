#!/bin/bash

chip_dir="rk180x"
./rknn_batch_inference_demo  model/${chip_dir}/mobilenet_v1_rk1808_batch_4.rknn  model/dog_224x224.jpg