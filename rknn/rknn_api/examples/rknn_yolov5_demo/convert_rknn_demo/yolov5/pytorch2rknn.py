from rknn.api import RKNN

MODEL = './models/yolov5s_relu.pt'
# platform="rk1808"
platform = "rv1109"
RKNN_MODEL = 'yolov5s_relu_{}_out_opt.rknn'.format(platform)
# PRECOM_MODEL = 'yolov5s_relu_{}_out_opt_precompile.rknn'.format(platform)


if __name__ == '__main__':

    add_perm = False # 如果设置成True,则将模型输入layout修改成NHWC
    # Create RKNN object
    rknn = RKNN(verbose=True)

    # Set model config
    print('--> config model')
    rknn.config(batch_size=1, mean_values=[[0, 0, 0]], std_values=[[255, 255, 255]], reorder_channel='0 1 2', target_platform=[platform], 
                force_builtin_perm=add_perm, 
                output_optimize=1
                )
    print('done')

    # Load ONNX model
    print('--> Loading model')
    ret = rknn.load_pytorch(model=MODEL, input_size_list=[[3,640,640]])
    if ret != 0:
        print('Load pytorch model failed!')
        exit(ret)
    print('done')

    # Build model
    print('--> Building model')
    ret = rknn.build(do_quantization=True, dataset='./dataset.txt')
    if ret != 0:
        print('Build RKNN model failed!')
        exit(ret)
    print('done')

    # Save RKNN model
    rknn.export_rknn(RKNN_MODEL)

    # rknn.init_runtime(target='rv1109', rknn2precompile=True)
    # rknn.export_rknn_precompile_model(PRECOM_MODEL)

    rknn.release()
