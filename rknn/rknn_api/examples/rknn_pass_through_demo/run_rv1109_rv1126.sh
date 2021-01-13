#!/bin/bash

chip_dir="rv1109_rv1126"
case $1 in
	uint8)
	./rknn_pass_through_demo  model/${chip_dir}/mobilenet_v2_UINT8_rv1109_rv1126.rknn  model/dog_224x224.jpg
	;;
	int8)
	./rknn_pass_through_demo  model/${chip_dir}/mobilenet_v2_INT8_rv1109_rv1126.rknn  model/dog_224x224.jpg
	;;
	int16)
	./rknn_pass_through_demo  model/${chip_dir}/mobilenet_v2_INT16_rv1109_rv1126.rknn  model/dog_224x224.jpg
	;;
	fp)
	./rknn_pass_through_demo  model/${chip_dir}/mobilenet_v2_FP_rv1109_rv1126.rknn model/dog_224x224.jpg
	;;
	*)
	echo './run.sh uint8 int8 int16 fp'
	;;
esac


