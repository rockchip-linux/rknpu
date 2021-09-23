from rknn.api import RKNN

ONNX_MODEL = './onnx_models/yolov5s_rm_transpose.onnx'
# platform="rk1808"
platform = "rv1109"
RKNN_MODEL = 'yolov5s_relu_{}_out_opt.rknn'.format(platform)


if __name__ == '__main__':

    add_perm = False # 如果设置成True,则将模型输入layout修改成NHWC
    # Create RKNN object
    rknn = RKNN(verbose=True)

    # pre-process config
    print('--> config model')
    rknn.config(batch_size=1, mean_values=[[0, 0, 0]], std_values=[[255, 255, 255]], reorder_channel='0 1 2', target_platform=[platform], 
                force_builtin_perm=add_perm, output_optimize=1)
    print('done')

    # Load tensorflow model
    print('--> Loading model')
    ret = rknn.load_onnx(model=ONNX_MODEL)
    if ret != 0:
        print('Load resnet50v2 failed!')
        exit(ret)
    print('done')

    # Build model
    print('--> Building model')
    ret = rknn.build(do_quantization=True, dataset='./dataset.txt')
    if ret != 0:
        print('Build resnet50 failed!')
        exit(ret)
    print('done')

    # rknn.export_rknn_precompile_model(RKNN_MODEL)
    rknn.export_rknn(RKNN_MODEL)

    rknn.release()
