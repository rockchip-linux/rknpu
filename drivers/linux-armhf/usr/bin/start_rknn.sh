#!/bin/sh

export RKNN_SERVER_PLUGINS='/usr/lib/npu/rknn/plugins/'

while true
do
  sleep 1
  rknn_server #>/dev/null 2>&1
done
