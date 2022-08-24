# RKNPU

本工程主要为Rockchip NPU提供驱动、示例等。



## 适用平台

- RK1808/RK1806

- RV1109/RV1126

    

**注意：**    

- **RK3399Pro用户态的库及驱动不在本工程**，请参考：https://github.com/airockchip/RK3399Pro_npu
- RK3566/RK3568/RK3588/RV1103/RV1106请参考：https://github.com/rockchip-linux/rknpu2



## RKNN Toolkit

​    在使用RKNN API进行部署之前，需要使用RKNN Toolkit将原始的模型转化成rknn模型。

- RK1808/RK1806/RV1109/RV1126/RK3399Pro 使用： https://github.com/rockchip-linux/rknn-toolkit
- RK3566/RK3568/RK3588/RV1103/RV1106使用：https://github.com/rockchip-linux/rknn-toolkit2

​    具体的使用说明请参考相应的网址。



## NPU驱动说明

### NPU驱动目录说明

NPU的驱动在$SDK/external/rknpu/drivers/目录下或者https://github.com/rockchip-linux/rknpu/tree/master/drivers

其中的编译、安装规则参考$SDK/buildroot/package/rockchip/rknpu/rknpu.mk

主要目录包括：

```
drivers/
├── common
├── linux-aarch64
├── linux-aarch64-mini
├── linux-armhf
├── linux-armhf-mini
├── linux-armhf-puma
├── linux-armhf-puma-mini
├── npu_ko
```

- linux-aarch64：RK1808 full driver

- linux-aarch64-mini: RK1808 mini driver

- linux-armhf: RK1806  full driver

- linux-armhf-mini: RK1806 mini driver

- linux-armhf-puma: RV1109/RV1126  full driver

- linux-armhf-puma-mini: RV1109/RV1126 mini driver

- npu_ko：NPU内核驱动KO

    在更新驱动时，需要同时更新用户态驱动及内核驱动，不然会产生类似错误:

    ```
    [ 1] HAL user version 6.4.6.5.351518
    [ 2] HAL kernel version 6.4.6.5.351518
    ```

    另外，npu ko与内核配置强相关，有可能会加载不成功，运行程序时，会产生类似错误：
    
    ```
    [ 1] Failed to open device: No such file or directory, Try again...
    [ 2] Failed to open device: No such file or directory, Try again...
    ```
    
    遇到这种情况需要联系开发团队解决。
    
    

### NPU full driver与mini driver的区别

主要包含以下几点：

- Mini driver只支持预编译的rknn模型，如果跑非预编译模型，会出现 RKNN_ERR_MODEL_INVALID的错误，从1.6.0开始，会返回RKNN_ERR_NON_PRE_COMPILED_MODEL_ON_MINI_DRIVER的错误;

- Full driver支持RKNN Toolkit的联机调试功能，mini driver不支持；

- Mini driver 库大小比full driver小很多，以RV1109/RV1126 1.6.0驱动为例，full driver大小为87MB，mini driver大小为7.1MB，可以有效的节省flash大小。

- Mini driver库运行时占用的内存比full driver小。

    

### 手动更新NPU驱动

有时需要手动更新NPU驱动，只要将相关的驱动拷贝到对应的目录就可以。

- 更新RK1808的驱动：

```
adb push drivers/linux-aarch64/   /
adb push drivers/npu_ko/galcore.ko /lib/modules/galcore.ko
```

- 更新RK1808 mini driver：

```
adb push drivers/linux-aarch64-mini/   /
adb push drivers/npu_ko/galcore.ko /lib/modules/galcore.ko
```
- 更新RV1109/RV1126的驱动：

```
adb push drivers/linux-armhf-puma/   /
adb push drivers/npu_ko/galcore_puma.ko /lib/modules/galcore.ko
```

- 更新RV1109/RV1126 mini driver：

```
adb push drivers/linux-armhf-puma-mini/   /
adb push drivers/npu_ko/galcore_puma.ko /lib/modules/galcore.ko
```

注意：adb不同版本的默认行为不一样，有些是直接push目录的，有些是push目录下的文件。因此替换完后，需要检查一下相关的库是否已经替换成功。比如通过md5sum检查对应文件的md5值是否相同。部分板子不支持adb命令，请通过ssh等其他方式拷贝文件到对应目录即可。

**另外，toybrick可能对文件路径进行了调整，因此最好通过toybrick自带的方式进行更新。**



## librknn_api与librknn_runtime的区别

librknn_api是对librknn_runtime的封装，主要是为了减少对其他so的编译依赖，功能上并没有区别。检查驱动版本时，一般以librknn_runtime.so为准。
