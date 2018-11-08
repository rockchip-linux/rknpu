#!/bin/sh

killall start_rknn.sh
killall rknn_server
start_rknn.sh &
