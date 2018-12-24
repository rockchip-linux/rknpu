import numpy as np
import cv2
from util.nms import nms
from util.bbox_transform import bbox_transform_inv, clip_boxes

from rknn.api import RKNN

np.set_printoptions(threshold=np.inf)

CLASSES = ('__background__',
           'aeroplane', 'bicycle', 'bird', 'boat',
           'bottle', 'bus', 'car', 'cat', 'chair',
           'cow', 'diningtable', 'dog', 'horse',
           'motorbike', 'person', 'pottedplant',
           'sheep', 'sofa', 'train', 'tvmonitor')

NUM_CLS = 21
NUM_BBOX = 84
NUM_ROI = 300

CONF_THRESH = 0.8
NMS_THRESH = 0.3


def vis_detections(im, class_name, dets, thresh):
    """Draw detected bounding boxes."""

    inds = np.where(dets[:, -1] >= thresh)[0]
    if len(inds) == 0:
        return

    for i in inds:
        bbox = dets[i, :4]
        score = dets[i, -1]
        p1 = bbox[0:2]
        p2 = bbox[2:4]
        p3 = p1.astype(np.int)
        p3[1] = p3[1] - 5

        cv2.rectangle(im, tuple(p1.astype(np.int)), tuple(p2.astype(np.int)), (255, 0, 0), thickness=2)
        cv2.putText(im, class_name, tuple(p3), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255))

if __name__ == '__main__':
    img = cv2.imread('./horse_800x600.jpg')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    cls_shape = [NUM_ROI, NUM_CLS]  # [300, 21]
    bbox_shape = [NUM_ROI, NUM_BBOX]  # [300, 84]
    roi_shape = [1, 1, NUM_ROI, 5]  # [1, 1, 300, 5]
    iminfo_shape = [1, 1, 1, 4]

    # Create RKNN object
    rknn = RKNN()

    print('--> Load RKNN model')
    ret = rknn.load_rknn('./faster_rcnn_zf.rknn')
    if ret != 0:
        print('Load RKNN model failed')
        exit(ret)
    print('done')

    # Inference
    print('--> Running model')
    outputs = rknn.inference(inputs=[img])
    print('done')

    img2 = cv2.imread('./horse_800x600.jpg')

    # post process
    cls_data = outputs[3]
    bbox_data = outputs[2]
    roi_data = outputs[0]
    iminfo_data = np.loadtxt('./im_info.txt', dtype=np.float32)

    cls_data = np.reshape(cls_data, cls_shape)
    bbox_data = np.reshape(bbox_data, bbox_shape)
    roi_data = np.squeeze(np.reshape(roi_data, roi_shape))
    iminfo_data = np.squeeze(np.reshape(iminfo_data, iminfo_shape))

    boxes = roi_data[:, 1:5] / iminfo_data[2]

    box_deltas = bbox_data
    pred_boxes = bbox_transform_inv(boxes, box_deltas)
    pred_boxes = clip_boxes(pred_boxes, img2.shape)

    for cls_ind, cls in enumerate(CLASSES[1:]):
        cls_ind += 1  # because we skipped background
        cls_boxes = pred_boxes[:, 4 * cls_ind:4 * (cls_ind + 1)]
        cls_scores = cls_data[:, cls_ind]

        dets = np.hstack((cls_boxes, cls_scores[:, np.newaxis])).astype(np.float32)

        keep = nms(cls, dets, NMS_THRESH)

        dets = dets[keep, :]

        vis_detections(img2, cls, dets, thresh=CONF_THRESH)

    cv2.imwrite("result.jpg", img2)
    print("Post process ok, create the result picture.")


