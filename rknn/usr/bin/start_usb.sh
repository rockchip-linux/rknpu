#!/bin/sh

case "$1" in
adb)
	killall adbd > /dev/null 2>&1
	killall start_rknn.sh > /dev/null 2>&1
	killall rknn_server > /dev/null 2>&1

	echo "none" > /sys/kernel/config/usb_gadget/rockchip/UDC

	umount /sys/kernel/config
	umount /dev/usb-ffs/ntb > /dev/null 2>&1

	mkdir -p /dev/usb-ffs -m 0770
	mkdir -p /dev/usb-ffs/adb -m 0770
	mount -t configfs none /sys/kernel/config
	mkdir /sys/kernel/config/usb_gadget/rockchip  -m 0770
	echo 0x2207 > /sys/kernel/config/usb_gadget/rockchip/idVendor
	# ADD_IDPRODUCT
	echo 0x0006 > /sys/kernel/config/usb_gadget/rockchip/idProduct
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/strings/0x409   -m 0770
	echo "0123456789ABCDEF" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/serialnumber
	echo "rockchip"  > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/manufacturer
	echo "rk3xxx"  > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/product
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/configs/b.1  -m 0770
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409  -m 0770
	echo 500 > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/MaxPower

	# INIT_FUNCTIONS
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/functions/ffs.adb
	rm -f /sys/kernel/config/usb_gadget/rockchip/configs/b.1/f1
	ln -s /sys/kernel/config/usb_gadget/rockchip/functions/ffs.adb /sys/kernel/config/usb_gadget/rockchip/configs/b.1/f1

	# INIT_CONFIG
	echo "adb" > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409/configuration

	# START_APP_BEFORE_UDC
	mkdir -p /dev/usb-ffs/adb
	mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb
	export service_adb_tcp_port=5555
	adbd&
	sleep 1 &&

	UDC=`ls /sys/class/udc/| awk '{print $1}'`
	echo $UDC > /sys/kernel/config/usb_gadget/rockchip/UDC
	# START_APP_AFTER_UDC

	rm -rf /dev/usb-ffs/ntb

	;;
ntb)
	killall adbd > /dev/null 2>&1
	killall start_rknn.sh > /dev/null 2>&1
	killall rknn_server > /dev/null 2>&1

	echo "none" > /sys/kernel/config/usb_gadget/rockchip/UDC

	umount /sys/kernel/config
	umount /dev/usb-ffs/adb > /dev/null 2>&1

	mkdir -p /dev/usb-ffs -m 0770
	mkdir -p /dev/usb-ffs/ntb -m 0770
	mount -t configfs none /sys/kernel/config
	mkdir -p /sys/kernel/config/usb_gadget/rockchip  -m 0770
	echo 0x2207 > /sys/kernel/config/usb_gadget/rockchip/idVendor
	# ADD_IDPRODUCT
	echo 0x1808 > /sys/kernel/config/usb_gadget/rockchip/idProduct
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/strings/0x409   -m 0770
	echo "0123456789ABCDEF" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/serialnumber
	echo "rockchip"  > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/manufacturer
	echo "rk3xxx"  > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/product
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/configs/b.1  -m 0770
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409  -m 0770
	echo 500 > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/MaxPower

	# INIT_FUNCTIONS
	mkdir -p /sys/kernel/config/usb_gadget/rockchip/functions/ffs.ntb
	rm -f /sys/kernel/config/usb_gadget/rockchip/configs/b.1/f1
	ln -s /sys/kernel/config/usb_gadget/rockchip/functions/ffs.ntb /sys/kernel/config/usb_gadget/rockchip/configs/b.1/f1

	# INIT_CONFIG
	echo "ntb" > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409/configuration

	# START_APP_BEFORE_UDC
	mkdir -p /dev/usb-ffs/ntb
	mount -o uid=2000,gid=2000 -t functionfs ntb /dev/usb-ffs/ntb

	rm -rf /dev/usb-ffs/adb

	;;
*)
	echo "Usage: $0 {adb|ntb}"
	exit 1
esac

exit 0
