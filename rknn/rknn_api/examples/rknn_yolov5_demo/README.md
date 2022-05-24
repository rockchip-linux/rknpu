# Yolo-v5 demo

## 导出rknn模型

### 使用官方onnx模型

1. 使用yolov5官方仓库导出模型，链接：https://github.com/ultralytics/yolov5, 该demo创建时yolov5的最新节点sha码为: c5360f6e7009eb4d05f14d1cc9dae0963e949213

2. 在yolov5工程的根目录下导出已训练好的yolov5模型，如yolov5s.onnx.

   ```sh
   python export.py --weights yolov5s.pt --img 640 --batch 1 --opset 12
   ```

   注：yolov5工程需要使用pytorch 1.8.0 或 1.9.0 版本才能正常导出。

3. 使用onnx-simplifier工具优化yolov5的onnx模型,安装和优化命令如下:

   ```sh
   ## 如果已安装onnx-simplifier,跳过这句
   pip install onnx-simplifier 
   
   python -m onnxsim yolov5s.onnx  yolov5s.onnx
   ```

### 运行python推理

1. rknn-toolkit提供了一个转好的yolov5s.onnx，如果从yolov5官网重新导出了优化的onnx模型, 则进入rknn-toolkit目录，将导出的onnx模型复制到examples/onnx/yolov5目录下yolov5s.onnx，再执行命令:

   ```sh
   cd examples/onnx/yolov5
   python test.py
   ```

### 使用rk预训练模型

由于官方的yolov5s模型中包含了Slice层/Swish层/大kernel_size的MaxPooling层，NPU执行效率不高。我们建议开发者使用NPU友好的算子替换官网的结构，下面给出两个参考网络结构：

1. 改进结构1

```txt
a. 将Focus层改成Conv层
b. 将Swish激活函数改成Relu激活函数
```

我们Demo中提供了一个高性能版本的rknn模型：`model/yolov5s-640-640.rknn`。对应的onnx模型是`convert_rknn_demo/yolov5/onnx_models/yolov5s_rm_transpose.onnx`，该模型是预测80类coco数据集的yolov5s改进结构，将Slice层训练和导出onnx模型过程请参考https://github.com/airockchip/yolov5.git

转换rknn模型的步骤如下：

```sh
cd convert_rknn_demo/yolov5/
python onnx2rknn.py
```

2. 改进结构2

```txt
a. 将Focus层改成Conv层
b. 将Swish激活函数改成Relu激活函数
c. 将大kernel_size的MaxPooling改成3x3 MaxPooling Stack结构
```

训练和导出onnx模型过程请参考请参考https://github.com/EASY-EAI/yolov5

## 注意事项：

1. 使用rknn-toolkit版本大于等于1.7.0。

2. 本Demo只支持8比特非对称量化的rknn模型推理。

3. 切换成自己训练的模型时，请注意对齐anchor等后处理参数，否则会导致后处理解析出错。

4. 官网和rk预训练模型都是检测80类的目标，如果自己训练的模型,自行更改include/postprocess.h中的OBJ_CLASS_NUM以及NMS_THRESH,BOX_THRESH后处理参数后再编译。

5. 测试代码导出模型的时候指定了输出节点['378', '439', '500']，分别为原模型的第2、3、4输出节点的去掉 三个Reshape 后面的层（不包含Reshape层），
对应输出的shape是[1,255,80,80],[1,255,40,40],[1,255,20,20]。
对于自己训练的模型输出节点的顺序和shape的要求必须是[1,？,80,80]，[1,？,40,40]，[1,？,20,20]，C代码后处理才能正确处理。

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
