# Zero copy demo

In specific case, the number of input data copies can be reduced to 0, that is, zero copy. For example, when the RKNN model is asymmetric quantization, the quantization data type is uint8, the mean value of the 3 channels is the same integer and the scaling factor is the same, the normalization and quantization can be omitted. 

This demo compares the performance difference between zero-copy and normal process inference.

```txt
a. zero-copy inference uses the "rknn_inputs_map" interface

b. normal inference uses the "rknn_inputs_set" interface.
```

In the "model" directory, models without NHWC suffix have input in NCHW format, and models with NHWC suffix have input in NHWC format. The model with NHWC suffix is ​​exported using rknn-toolkit by adding the following parameters

```python
rknn.config(force_builtin_perm=True)
```

Note: if it shows "**get unvalid input physical address, please extend in/out memory space**", you may need to modify the size of **CMA** in the **kernel's dts**, such as:

```
diff --git a/arch/arm/boot/dts/rv1126.dtsi b/arch/arm/boot/dts/rv1126.dtsi
index f5f0a07..1d0487c 100644
--- a/arch/arm/boot/dts/rv1126.dtsi
+++ b/arch/arm/boot/dts/rv1126.dtsi
@@ -362,11 +362,17 @@
                #size-cells = <1>;
                ranges;
 
+               nouse@0 {
+                       reg = <0x00000000 0x4000>;
+                       no-map;
+               };
+
+
                linux,cma {
                        compatible = "shared-dma-pool";
                        inactive;
                        reusable;
-                       size = <0x800000>;
+                       size = <0x8000000>;
                        linux,cma-default;
                };

```

This patch is only for RV1109/RV1126，if you use RK1808，you  need to select the appropriate dts file to modify.

## Build

modify `GCC_COMPILER` on `build.sh` for target platform, then execute

```sh
./build.sh
```

## Install

Copy install/rknn_zero_copy_demo to the devices under /userdata/.

- If you use rockchip's evb board, you can use the following way:

Connect device and push the program and rknn model to `/userdata`

```sh
adb push install/rknn_zero_copy_demo /userdata/
```

- If your board has sshd service, you can use scp or other methods to copy the program and rknn model to the board.

## Run

```sh
adb shell
cd /userdata/rknn_zero_copy_demo/
```

- RK1808/RK1806

```sh
./run_rk180x.sh
```

- RV1109/RV1126

```sh
./run_rv1109_rv1126.sh
```
