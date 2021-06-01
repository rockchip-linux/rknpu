/****************************************************************************
*
*    Copyright (c) 2017 - 2018 by Rockchip Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Rockchip Corporation. This is proprietary information owned by
*    Rockchip Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Rockchip Corporation.
*
*****************************************************************************/

/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

#include "rknn_api.h"

using namespace std;

#define DEMO_INPUT_NUM 4

/*-------------------------------------------
                  Functions
-------------------------------------------*/

static void printRKNNTensor(rknn_tensor_attr *attr)
{
    printf("index=%d name=%s n_dims=%d dims=[%d %d %d %d] n_elems=%d size=%d fmt=%d type=%d qnt_type=%d fl=%d zp=%d scale=%f\n",
           attr->index, attr->name, attr->n_dims, attr->dims[3], attr->dims[2], attr->dims[1], attr->dims[0],
           attr->n_elems, attr->size, 0, attr->type, attr->qnt_type, attr->fl, attr->zp, attr->scale);
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

static unsigned char *load_image(const char *image_path, rknn_tensor_attr *input_attr)
{
    int req_height = 0;
    int req_width = 0;
    int req_channel = 0;

    switch (input_attr->fmt)
    {
    case RKNN_TENSOR_NHWC:
        req_height = input_attr->dims[2];
        req_width = input_attr->dims[1];
        req_channel = input_attr->dims[0];
        break;
    case RKNN_TENSOR_NCHW:
        req_height = input_attr->dims[1];
        req_width = input_attr->dims[0];
        req_channel = input_attr->dims[2];
        break;
    default:
        printf("meet unsupported layout\n");
        return NULL;
    }

    printf("w=%d,h=%d,c=%d, fmt=%d\n", req_width, req_height, req_channel, input_attr->fmt);

    int height = 0;
    int width = 0;
    int channel = 0;

    unsigned char *image_data = stbi_load(image_path, &width, &height, &channel, req_channel);
    if (image_data == NULL)
    {
        printf("load image failed!\n");
        return NULL;
    }

    if (width != req_width || height != req_height)
    {
        unsigned char *image_resized = (unsigned char *)STBI_MALLOC(req_width * req_height * req_channel);
        if (!image_resized)
        {
            printf("malloc image failed!\n");
            STBI_FREE(image_data);
            return NULL;
        }
        if (stbir_resize_uint8(image_data, width, height, 0, image_resized, req_width, req_height, 0, channel) != 1)
        {
            printf("resize image failed!\n");
            STBI_FREE(image_data);
            return NULL;
        }
        STBI_FREE(image_data);
        image_data = image_resized;
    }

    return image_data;
}

/*-------------------------------------------
                  Main Function
-------------------------------------------*/
int main(int argc, char **argv)
{
    const int MODEL_IN_WIDTH = 128;
    const int MODEL_IN_HEIGHT = 128;

    rknn_context ctx;
    int ret;
    int model_len = 0;
    unsigned char *model;

    const char *model_path = argv[1];
    const char *img_path = argv[2];

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
    printf("model input num: %d, output num: %d\n", io_num.n_input, io_num.n_output);

    printf("input tensors:\n");
    rknn_tensor_attr input_attrs[io_num.n_input];
    memset(input_attrs, 0, sizeof(input_attrs));
    for (int i = 0; i < io_num.n_input; i++)
    {
        input_attrs[i].index = i;
        ret = rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, &(input_attrs[i]), sizeof(rknn_tensor_attr));
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
        ret = rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, &(output_attrs[i]), sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC)
        {
            printf("rknn_query fail! ret=%d\n", ret);
            return -1;
        }
        printRKNNTensor(&(output_attrs[i]));
    }

    assert(DEMO_INPUT_NUM == io_num.n_input);
    // Set Input struct
    rknn_input inputs[DEMO_INPUT_NUM];
    memset(inputs, 0, DEMO_INPUT_NUM * sizeof(rknn_input));
    std::vector<unsigned char *> imgs;
    for (int i = 0; i < io_num.n_input; ++i)
    {
        int channel = 0;
        if (input_attrs[i].fmt == RKNN_TENSOR_NCHW)
        {
            channel = input_attrs[i].dims[2];
        }
        else
        {
            channel = input_attrs[i].dims[0];
        }
        //convert single channel to multi channels
        // Load image
        unsigned char *input_data = NULL;
        input_data = load_image(img_path, &input_attrs[0]);
        if (!input_data)
        {
            return -1;
        }
        imgs.push_back(input_data);
        int img_size = input_attrs[0].size;
        inputs[i].index = i;
        inputs[i].type = RKNN_TENSOR_UINT8;
        inputs[i].size = img_size;
        inputs[i].fmt = RKNN_TENSOR_NHWC;
    }
    for (int i = 0; i < io_num.n_input; ++i)
    {
        inputs[i].buf = imgs[i];
    }

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

    float ref_data[5] = {24.482, 20.453, 20.453, 20.453, 20.453};
    float *buffer = (float *)outputs[0].buf;
    printf("first 5 result:\n");
    for (int i = 0; i < 5; ++i)
    {
        printf("[%d]:%.3f vs %.3f\n", i, buffer[i], ref_data[i]);
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
    for (auto d : imgs)
    {
        stbi_image_free(d);
    }

    return 0;
}
