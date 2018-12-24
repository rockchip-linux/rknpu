import numpy as np

import re
import math
import random
import cv2

from rknn.api import RKNN

INPUT_SIZE = 300

NUM_RESULTS = 1917
NUM_CLASSES = 91

Y_SCALE = 10.0
X_SCALE = 10.0
H_SCALE = 5.0
W_SCALE = 5.0


def expit(x):
    return 1. / (1. + math.exp(-x))


def load_box_priors():
    box_priors_ = []
    fp = open('./box_priors.txt', 'r')
    ls = fp.readlines()
    for s in ls:
        aList = re.findall('([-+]?\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?', s)
        for ss in aList:
            aNum = float((ss[0]+ss[2]))
            box_priors_.append(aNum)
    fp.close()

    box_priors = np.array(box_priors_)
    box_priors = box_priors.reshape(4, NUM_RESULTS)

    return box_priors


if __name__ == '__main__':

    # Create RKNN object
    rknn = RKNN()

    # Config for Model Input PreProcess
    rknn.config(channel_mean_value='128 128 128 128', reorder_channel='0 1 2')

    # Load TensorFlow Model
    print('--> Loading model')
    rknn.load_tensorflow(tf_pb='./ssd_mobilenet_v1_coco_2017_11_17.pb',
                         inputs=['FeatureExtractor/MobilenetV1/MobilenetV1/Conv2d_0/BatchNorm/batchnorm/mul_1'],
                         outputs=['concat', 'concat_1'],
                         input_size_list=[[INPUT_SIZE, INPUT_SIZE, 3]])
    print('done')

    # Build Model
    print('--> Building model')
    rknn.build(do_quantization=True, dataset='./dataset.txt')
    print('done')

    # Export RKNN Model
    rknn.export_rknn('./ssd_mobilenet_v1_coco.rknn')

    # Direct Load RKNN Model
    # rknn.load_rknn('./ssd_mobilenet_v1_coco.rknn')

    # Set inputs
    orig_img = cv2.imread('./road.bmp')
    img = cv2.cvtColor(orig_img, cv2.COLOR_BGR2RGB)
    img = cv2.resize(img, (INPUT_SIZE, INPUT_SIZE), interpolation=cv2.INTER_CUBIC)

    # Inference
    print('--> Running model')
    outputs = rknn.inference(inputs=[img])
    print('done')
    print('inference result: ', outputs)

    predictions = outputs[1].reshape((1, NUM_RESULTS, 4))
    outputClasses = outputs[0].reshape((1, NUM_RESULTS, NUM_CLASSES))

    box_priors = load_box_priors()

    # Post Process
    for i in range(0, NUM_RESULTS):
        ycenter = predictions[0][i][0] / Y_SCALE * box_priors[2][i] + box_priors[0][i]
        xcenter = predictions[0][i][1] / X_SCALE * box_priors[3][i] + box_priors[1][i]
        h = math.exp(predictions[0][i][2] / H_SCALE) * box_priors[2][i]
        w = math.exp(predictions[0][i][3] / W_SCALE) * box_priors[3][i]

        ymin = ycenter - h / 2.
        xmin = xcenter - w / 2.
        ymax = ycenter + h / 2.
        xmax = xcenter + w / 2.

        predictions[0][i][0] = ymin
        predictions[0][i][1] = xmin
        predictions[0][i][2] = ymax
        predictions[0][i][3] = xmax

        topClassScore = -1000
        topClassScoreIndex = -1

        # Skip the first catch-all class.
        for j in range(1, NUM_CLASSES):
            score = expit(outputClasses[0][i][j]);

            if score > topClassScore:
                topClassScoreIndex = j
                topClassScore = score
            if topClassScore > 0.4:
                detection = (
                    predictions[0][i][1] * INPUT_SIZE,
                    predictions[0][i][0] * INPUT_SIZE,
                    predictions[0][i][3] * INPUT_SIZE,
                    predictions[0][i][2] * INPUT_SIZE)
                xmin = detection[0]
                ymin = detection[1]
                xmax = detection[2]
                ymax = detection[3]
                # print("%d @ (%d, %d) (%d, %d) score=%f" % (topClassScoreIndex, xmin, ymin, xmax, ymax, topClassScore))
                cv2.rectangle(orig_img, (int(xmin), int(ymin)), (int(xmax), int(ymax)),
                              (random.random()*255, random.random()*255, random.random()*255), 3)

    cv2.imwrite("out.jpg", orig_img)

    # Evaluate Perf on Simulator
    rknn.eval_perf(inputs=[img], is_print=True)

    # Release RKNN Context
    rknn.release()
