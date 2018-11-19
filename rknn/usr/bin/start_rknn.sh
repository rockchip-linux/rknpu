#!/bin/sh

export VIV_VX_ENABLE_SWTILING_PHASE1=1
export VIV_VX_ENABLE_SWTILING_PHASE2=1

export VIV_VX_DEBUG_LEVEL=0
export VSI_NN_LOG_LEVEL=0
export RKNN_SERVER_LOGLEVEL=2
export RKNN_SERVER_PLUGINS='/usr/lib/npu/rknn/plugins/'

while true
do
  sleep 1
  rknn_server #>/dev/null 2>&1
done
