# Yolo-v5 demo

## 导出rknn模型

请参考 https://github.com/airockchip/rknn_model_zoo/tree/main/models/vision/object_detection/yolov5-pytorch



## 注意事项：

1. 使用rknn-toolkit版本大于等于1.7.0。
2. 本Demo只支持8比特非对称量化的rknn模型推理。
3. 切换成自己训练的模型时，请注意对齐anchor等后处理参数，否则会导致后处理解析出错。
4. 官网和rk预训练模型都是检测80类的目标，如果自己训练的模型,自行更改include/postprocess.h中的OBJ_CLASS_NUM以及NMS_THRESH,BOX_THRESH后处理参数后再编译。
5. 由于硬件限制，该demo的模型默认把 yolov5 模型的后处理部分，移至cpu实现。本demo附带的模型均使用relu为激活函数，相比silu激活函数精度略微下降，推理速度更快。
6. 关于加载时间：model目录下均是预编译rknn模型，加载速度比非预编译rknn模型快。convert_rknn_demo目录下的转换脚本生成非预编译rknn模型，如需重新生成预编译rknn模型，请参考rknn-toolkit的User Guide.


## Aarch64 Linux Demo

### 编译

根据指定平台修改`build.sh`中的交叉编译器所在目录的路径`GCC_COMPILER`，例如修改成

```sh
GCC_COMPILER=~/opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf
```

然后执行：

```sh
./build.sh
```

### 推送执行文件到板子

将 install/rknn_yolov5_demo 拷贝到板子的/userdata/目录.

- 如果使用rockchip的EVB板子，可以使用adb将文件推到板子上：

```sh
adb push install/rknn_yolov5_demo /userdata/
```

如果使用其他板子，可以使用scp等方式将install/rknn_yolov5_demo拷贝到板子的/userdata/目录

## 运行

```sh
adb shell
cd /userdata/rknn_yolov5_demo/
```

- rk180x

```sh
./run_rk180x.sh
```

- rv1109/rv1126

```sh
./run_rv1109_rv1126.sh
```
