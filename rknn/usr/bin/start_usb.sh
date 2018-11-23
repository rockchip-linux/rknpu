#!/bin/sh

configfs_init()
{
    PID=$1
    CONFIG_STRING=$2
    mkdir -p /dev/usb-ffs -m 0770
    mkdir -p /dev/usb-ffs/$CONFIG_STRING -m 0770
    mount -t configfs none /sys/kernel/config
    mkdir -p /sys/kernel/config/usb_gadget/rockchip  -m 0770
    echo 0x2207 > /sys/kernel/config/usb_gadget/rockchip/idVendor
    echo $PID > /sys/kernel/config/usb_gadget/rockchip/idProduct
    mkdir -p /sys/kernel/config/usb_gadget/rockchip/strings/0x409   -m 0770
    echo "0123456789ABCDEF" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/serialnumber
    echo "rockchip"  > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/manufacturer
    echo "rk3xxx"  > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/product
    mkdir -p /sys/kernel/config/usb_gadget/rockchip/configs/b.1  -m 0770
    mkdir -p /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409  -m 0770
    echo 500 > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/MaxPower
    echo \"$CONFIG_STRING\" > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409/configuration
}

function_init()
{
    CONFIG_STRING=$1
    mkdir -p /sys/kernel/config/usb_gadget/rockchip/functions/ffs.$CONFIG_STRING
    rm -f /sys/kernel/config/usb_gadget/rockchip/configs/b.1/ffs.*
    ln -s /sys/kernel/config/usb_gadget/rockchip/functions/ffs.$CONFIG_STRING /sys/kernel/config/usb_gadget/rockchip/configs/b.1/ffs.$CONFIG_STRING
}

case "$1" in
adb)
    killall adbd start_rknn.sh rknn_server > /dev/null 2>&1

    echo "none" > /sys/kernel/config/usb_gadget/rockchip/UDC

    umount /sys/kernel/config
    umount /dev/usb-ffs/ntb > /dev/null 2>&1
    rm -rf /dev/usb-ffs/ntb

    configfs_init 0x0006 adb
    function_init adb

    # START_APP_BEFORE_UDC
    mkdir -p /dev/usb-ffs/adb
    mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb
    export service_adb_tcp_port=5555
    adbd&
    sleep 1

    UDC=`ls /sys/class/udc/| awk '{print $1}'`
    echo $UDC > /sys/kernel/config/usb_gadget/rockchip/UDC
    # START_APP_AFTER_UDC

    start_rknn.sh &

    ;;
ntb)
    killall adbd start_rknn.sh rknn_server > /dev/null 2>&1

    echo "none" > /sys/kernel/config/usb_gadget/rockchip/UDC

    umount /sys/kernel/config
    umount /dev/usb-ffs/adb > /dev/null 2>&1
    rm -rf /dev/usb-ffs/adb

    configfs_init 0x1808 ntb
    function_init ntb

    # START_APP_BEFORE_UDC
    mkdir -p /dev/usb-ffs/ntb
    mount -o uid=2000,gid=2000 -t functionfs ntb /dev/usb-ffs/ntb

    start_rknn.sh &

    ;;
*)
    echo "Usage: $0 {adb|ntb}"
    exit 1
esac

exit 0
