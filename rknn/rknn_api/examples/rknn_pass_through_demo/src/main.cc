// Copyright (c) 2021 by Rockchip Electronics Co., Ltd. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "quant_utils.h"
#include "rknn_api.h"

using namespace std;
using namespace cv;

/*-------------------------------------------
                  Functions
-------------------------------------------*/

static void printRKNNTensor(rknn_tensor_attr *attr)
{
    printf("index=%d name=%s n_dims=%d dims=[%d %d %d %d] n_elems=%d size=%d "
           "fmt=%d type=%d qnt_type=%d fl=%d zp=%d scale=%f\n",
           attr->index, attr->name, attr->n_dims, attr->dims[3], attr->dims[2],
           attr->dims[1], attr->dims[0], attr->n_elems, attr->size, 0, attr->type,
           attr->qnt_type, attr->fl, attr->zp, attr->scale);
}

static unsigned char *load_model(const char *filename, int *model_size)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == nullptr)
    {
        printf("fopen %s fail!\n", filename);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    int model_len = ftell(fp);
    unsigned char *model = (unsigned char *)malloc(model_len);
    fseek(fp, 0, SEEK_SET);
    if (model_len != fread(model, 1, model_len, fp))
    {
        printf("fread %s fail!\n", filename);
        free(model);
        return NULL;
    }
    *model_size = model_len;
    if (fp)
    {
        fclose(fp);
    }
    return model;
}

static int GetElementByte(rknn_tensor_attr *in_attr)
{
    int byte = 0;
    switch (in_attr->type)
    {
    case RKNN_TENSOR_FLOAT32:
        byte = 4;
        break;
    case RKNN_TENSOR_FLOAT16:
    case RKNN_TENSOR_INT16:
        byte = 2;
        break;
    case RKNN_TENSOR_INT8:
    case RKNN_TENSOR_UINT8:
        byte = 1;
        break;
    default:
        break;
    }
    return byte;
}

static int ProcessInput(cv::Mat &img, void **dst_buf, rknn_tensor_attr *in_attr,
                        std::vector<float> mean, std::vector<float> scale,
                        bool isReorder210, bool isNCHW)
{
    // check
    if (3 != img.channels() || 3 != mean.size() || img.empty())
    {
        return -1;
    }

    // RGB2BGR
    if (isReorder210 == true)
    {
        printf("perform RGB2BGR\n");
        cv::cvtColor(img, img, COLOR_RGB2BGR);
    }
    img.convertTo(img, CV_32FC3);

    int HW = img.cols * img.rows;
    int C = img.channels();
    int ele_count = HW * C;
    int ele_bytes = GetElementByte(in_attr);
    printf("total element count = %d, bytes per element = %d\n", ele_count,
           ele_bytes);

    // normalize
    printf("perform normalize\n");
    cv::Mat norm_out;
    std::vector<cv::Mat> rgbChannels(3);
    cv::split(img, rgbChannels);
    for (int i = 0; i < mean.size(); i++)
    {
        rgbChannels[i] = (rgbChannels[i] - mean[i]) / scale[i];
    }
    cv::merge(rgbChannels, norm_out);

    // quantize
    printf("perform quantize\n");
    uint8_t *qnt_buf = (uint8_t *)malloc(ele_count * ele_bytes);
    switch (in_attr->type)
    {
    case RKNN_TENSOR_FLOAT32:
        memcpy(qnt_buf, norm_out.data, ele_count * ele_bytes);
        break;
    case RKNN_TENSOR_FLOAT16:
        f32_to_f16((uint16_t *)(qnt_buf), (float *)(norm_out.data), ele_count);
        break;
    case RKNN_TENSOR_INT16:
    case RKNN_TENSOR_INT8:
    case RKNN_TENSOR_UINT8:
        switch (in_attr->qnt_type)
        {
        case RKNN_TENSOR_QNT_DFP:
            qnt_f32_to_dfp(qnt_buf, in_attr->type, in_attr->fl,
                           (float *)(norm_out.data), ele_count);
            break;
        case RKNN_TENSOR_QNT_AFFINE_ASYMMETRIC:
            qnt_f32_to_affine(qnt_buf, in_attr->type, in_attr->zp, in_attr->scale,
                              (float *)(norm_out.data), ele_count);
            break;
        case RKNN_TENSOR_QNT_NONE:
            qnt_f32_to_none(qnt_buf, in_attr->type, (float *)(norm_out.data),
                            ele_count);
        default:
            break;
        }
        break;
    default:
        break;
    }

    // NHWC ==> NCHW
    if (isNCHW)
    {
        printf("perform NHWC to NCHW\n");
        uint8_t *nchw_buf = (uint8_t *)malloc(ele_count * ele_bytes);
        uint8_t *dst_ptr = nchw_buf;
        uint8_t *src_ptr = qnt_buf;
        for (int i = 0; i < C; ++i)
        {
            src_ptr = qnt_buf + i * ele_bytes;
            dst_ptr = nchw_buf + i * HW * ele_bytes;
            for (int j = 0; j < HW; ++j)
            {
                // dst_ptr[i*HW+j] = src_ptr[j*C+i];
                memcpy(dst_ptr, src_ptr, ele_bytes);
                src_ptr += C * ele_bytes;
                dst_ptr += ele_bytes;
            }
        }
        *dst_buf = nchw_buf;
        free(qnt_buf);
    }
    else
    {
        *dst_buf = qnt_buf;
    }

    return 0;
}

static int rknn_GetTop
    (
    float *pfProb,
    float *pfMaxProb,
    uint32_t *pMaxClass,
    uint32_t outputCount,
    uint32_t topNum
    )
{
    uint32_t i, j;

    #define MAX_TOP_NUM 20
    if (topNum > MAX_TOP_NUM) return 0;

    memset(pfMaxProb, 0, sizeof(float) * topNum);
    memset(pMaxClass, 0xff, sizeof(float) * topNum);

    for (j = 0; j < topNum; j++)
    {
        for (i=0; i<outputCount; i++)
        {
            if ((i == *(pMaxClass+0)) || (i == *(pMaxClass+1)) || (i == *(pMaxClass+2)) ||
                (i == *(pMaxClass+3)) || (i == *(pMaxClass+4)))
            {
                continue;
            }

            if (pfProb[i] > *(pfMaxProb+j))
            {
                *(pfMaxProb+j) = pfProb[i];
                *(pMaxClass+j) = i;
            }
        }
    }

    return 1;
}

/*-------------------------------------------
                  Main Function
-------------------------------------------*/
int main(int argc, char **argv)
{
    const int MODEL_IN_WIDTH = 224;
    const int MODEL_IN_HEIGHT = 224;
    const int MODEL_IN_CHANNELS = 3;
    bool isNCHW = false;
    // mobilenet v2 mean/scale in rknn.config()
    bool isReorder210 = true; /* reorder= '2 1 0' */
    std::vector<float> mean({103.94, 116.78, 123.68});
    std::vector<float> scale({58.82, 58.82, 58.82});

    rknn_context ctx;
    int ret;
    int model_len = 0;
    unsigned char *model;

    const char *model_path = argv[1];
    const char *img_path = argv[2];

    // Load image
    cv::Mat orig_img = imread(img_path, cv::IMREAD_COLOR);
    if (!orig_img.data)
    {
        printf("cv::imread %s fail!\n", img_path);
        return -1;
    }

    cv::Mat img = orig_img.clone();
    if (orig_img.cols != MODEL_IN_WIDTH || orig_img.rows != MODEL_IN_HEIGHT)
    {
        printf("resize %d %d to %d %d\n", orig_img.cols, orig_img.rows,
               MODEL_IN_WIDTH, MODEL_IN_HEIGHT);
        cv::resize(orig_img, img, cv::Size(MODEL_IN_WIDTH, MODEL_IN_HEIGHT), (0, 0),
                   (0, 0), cv::INTER_LINEAR);
    }

    cv::cvtColor(img, img, COLOR_BGR2RGB);

    // Load RKNN Model
    model = load_model(model_path, &model_len);
    ret = rknn_init(&ctx, model, model_len, 0);
    if (ret < 0)
    {
        printf("rknn_init fail! ret=%d\n", ret);
        return -1;
    }

    // Get Model Input Output Info
    rknn_input_output_num io_num;
    ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
    if (ret != RKNN_SUCC)
    {
        printf("rknn_query fail! ret=%d\n", ret);
        return -1;
    }
    printf("model input num: %d, output num: %d\n", io_num.n_input,
           io_num.n_output);

    printf("input tensors:\n");
    rknn_tensor_attr input_attrs[io_num.n_input];
    memset(input_attrs, 0, sizeof(input_attrs));
    for (int i = 0; i < io_num.n_input; i++)
    {
        input_attrs[i].index = i;
        ret = rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, &(input_attrs[i]),
                         sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC)
        {
            printf("rknn_query fail! ret=%d\n", ret);
            return -1;
        }
        printRKNNTensor(&(input_attrs[i]));
    }

    printf("output tensors:\n");
    rknn_tensor_attr output_attrs[io_num.n_output];
    memset(output_attrs, 0, sizeof(output_attrs));
    for (int i = 0; i < io_num.n_output; i++)
    {
        output_attrs[i].index = i;
        ret = rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, &(output_attrs[i]),
                         sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC)
        {
            printf("rknn_query fail! ret=%d\n", ret);
            return -1;
        }
        printRKNNTensor(&(output_attrs[i]));
    }

    // rknn model need nchw layout input
    if (input_attrs[0].fmt == RKNN_TENSOR_NCHW)
    {
        isNCHW = true;
    }

    // process input when pass_through = 1
    void *in_data = NULL;
    ProcessInput(img, &in_data, &(input_attrs[0]), mean, scale, isReorder210,
                 isNCHW);

    // Set Input Data
    rknn_input inputs[1];
    memset(inputs, 0, sizeof(inputs));
    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].size = img.cols * img.rows * img.channels();
    inputs[0].fmt = RKNN_TENSOR_NCHW;
    inputs[0].buf = in_data;
    inputs[0].pass_through = 1;

    ret = rknn_inputs_set(ctx, io_num.n_input, inputs);
    if (ret < 0)
    {
        printf("rknn_input_set fail! ret=%d\n", ret);
        return -1;
    }

    // Run
    printf("rknn_run\n");
    ret = rknn_run(ctx, nullptr);
    if (ret < 0)
    {
        printf("rknn_run fail! ret=%d\n", ret);
        return -1;
    }

    // Get Output
    rknn_output outputs[1];
    memset(outputs, 0, sizeof(outputs));
    outputs[0].want_float = 1;
    ret = rknn_outputs_get(ctx, 1, outputs, NULL);
    if (ret < 0)
    {
        printf("rknn_outputs_get fail! ret=%d\n", ret);
        return -1;
    }

    // Post Process
    for (int i = 0; i < io_num.n_output; i++)
    {
		uint32_t MaxClass[5];
		float fMaxProb[5];
		float *buffer = (float *)outputs[i].buf;
		uint32_t sz = outputs[i].size/4;

		rknn_GetTop(buffer, fMaxProb, MaxClass, sz, 5);

		printf(" --- Top5 ---\n");
		for(int i=0; i<5; i++)
		{
			printf("%3d: %8.6f\n", MaxClass[i], fMaxProb[i]);
		}
	}

    // Release rknn_outputs
    rknn_outputs_release(ctx, 1, outputs);

    // Release
    if (ctx >= 0)
    {
        rknn_destroy(ctx);
    }
    if (model)
    {
        free(model);
    }
    return 0;
}
