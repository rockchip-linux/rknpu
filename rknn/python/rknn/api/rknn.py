# -*- coding:utf-8 -*-
import os
import sys
import traceback
import re

from argparse import Namespace
from .rknn_base import RKNNBase
from .rknn_runtime import RKNNRuntime

class RKNN:
    """
    Rockchip NN SDK
    """
    def __init__(self):
        cur_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
        self.rknn_base = RKNNBase(cur_path)

    def load_rknn(self, path):
        """
        Load RKNN model
        :param path: RKNN model file path
        :return: success: 0, failure: -1
        """
        if not os.path.exists(path):
            print('RKNN model file {} not exists!'.format(path))
            return -1
        try:
            ret = self.rknn_base.import_rknn(path)
        except:
            print('Catch exception when loading RKNN model [{}]!'.format(path))
            RKNN._print_traceback(traceback.format_exc())
            return -1
        else:
            if ret != 0:
                print('Load RKNN model [{}] failed!'.format(path))
            return ret

    def init_runtime(self, perf_debug=False):
        """
        Init run time environment. Needed by called before inference or eval performance.
        :param perf_debug: enable dump layer performance.
        :return: success: 0, failure: -1
        """
        try:
            self.rknn_base.init_runtime(target=None, device_id=None, perf_debug=perf_debug)
        except:
            print('Catch exception when init runtime!')
            RKNN._print_traceback(traceback.format_exc())
            return -1

        return 0

    def inference(self, inputs, data_type='uint8', data_format='nhwc', outputs=None):
        """
        Run model inference
        :param inputs: Input data List (ndarray list)
        :param data_type: Input data type
        :param data_format: Input data format
        :param outputs: Output data list, for determine shape and dtype (empty ndarray)
        :return: Output data (ndarray list)
        """
        try:
            results = self.rknn_base.inference(inputs=inputs, data_type=data_type,
                                               data_format=data_format, outputs=outputs)
        except:
            print('Catch exception when inference!')
            RKNN._print_traceback(traceback.format_exc())
            results = None
        finally:
            useless_file = os.path.join(os.getcwd(), 'viv_vir_cl_intrinsic_ImgLS.lib')
            if os.path.exists(useless_file):
                os.remove(useless_file)
            return results


    def eval_perf(self, inputs, data_type='uint8', data_format='nhwc', is_print=True):
        """
        Evaluate model performance
        :param inputs: Input data list (ndarray list)
        :param data_type: Input data type
        :param data_format: Input data format
        :param is_print: Format print perf result
        :return: Performance Result (dict)
        """
        try:
            perfs = self.rknn_base.eval_performance(inputs, data_type, data_format)
            if perfs is None:
                print('Error: Performance information is empty, some mistakes may happen.')
                return perfs
            if is_print:
                print(self.rknn_base.format_perf_detail(detail=perfs))
        except:
            print('Catch exception when evaluating model performance!')
            RKNN._print_traceback(traceback.format_exc())
            return None
        finally:
            useless_file = os.path.join(os.getcwd(), 'viv_vir_cl_intrinsic_ImgLS.lib')
            if os.path.exists(useless_file):
                os.remove(useless_file)

        return perfs

    def get_sdk_version(self):
        """
        Get SDK version
        :return: sdk_version
        """
        try:
            sdk_version = self.rknn_base.get_sdk_version()
        except Exception:
            print('Catch exception when get sdk version')
            RKNN._print_traceback(traceback.format_exc())
            return None

        return sdk_version

    def release(self):
        """
        Release RKNN resource
        :return: None
        """
        self.rknn_base.release()

    @staticmethod
    def _print_traceback(traceback_info):
        reg = re.compile(re.escape(RKNNBase.redirect_src), re.IGNORECASE)
        traceback_info = reg.sub(RKNNBase.redirect_dst, traceback_info)
        print(traceback_info)

