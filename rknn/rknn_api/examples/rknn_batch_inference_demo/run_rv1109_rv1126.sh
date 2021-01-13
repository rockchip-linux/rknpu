#!/bin/bash

chip_dir="rv1109_rv1126"
./rknn_batch_inference_demo  model/${chip_dir}/mobilenet_v1_rv1109_rv1126_batch_4.rknn  model/dog_224x224.jpg