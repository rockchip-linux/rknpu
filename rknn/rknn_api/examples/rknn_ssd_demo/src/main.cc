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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

#undef cimg_display
#define cimg_display 0
#include "CImg/CImg.h"

#include "rknn_api.h"

#include "ssd.h"

using namespace std;
using namespace cimg_library;

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
    const int img_width = 300;
    const int img_height = 300;
    const int img_channels = 3;

    rknn_context ctx;
    int ret;
    int model_len = 0;
    unsigned char *model;

    const char *model_path = argv[1];
    const char *img_path = argv[2];

    if (argc != 3)
    {
        printf("Usage:%s model image\n", argv[0]);
        return -1;
    }
    if (strstr(img_path, ".jpg") != NULL || strstr(img_path, ".png") != NULL)
    {
        printf("Error: read %s failed! only support .bmp format image\n",img_path);
        return -1;
    }

    // Load RKNN Model
    printf("Loading model ...\n");
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

    // Load image, only support .bmp format
    CImg<unsigned char> img(img_path);
    if (img.height() != img_height || img.width() != img_width)
    {
        img = img.resize(img_width, img_height);
    }
    unsigned char *input_data = NULL;
    input_data = load_image(img_path, &input_attrs[0]);
    if (!input_data)
    {
        return -1;
    }

    // Set Input Data
    rknn_input inputs[1];
    memset(inputs, 0, sizeof(inputs));
    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].size = input_attrs[0].size;
    inputs[0].fmt = RKNN_TENSOR_NHWC;
    inputs[0].buf = input_data;

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
    rknn_output outputs[2];
    memset(outputs, 0, sizeof(outputs));
    outputs[0].want_float = 1;
    outputs[1].want_float = 1;
    ret = rknn_outputs_get(ctx, io_num.n_output, outputs, NULL);
    if (ret < 0)
    {
        printf("rknn_outputs_get fail! ret=%d\n", ret);
        return -1;
    }

    // Post Process
    detect_result_group_t detect_result_group;
    postProcessSSD((float *)(outputs[0].buf), (float *)(outputs[1].buf), img_height, img_width, &detect_result_group);
    // Release rknn_outputs
    rknn_outputs_release(ctx, 2, outputs);

    // Draw Objects
    const unsigned char blue[] = {0, 0, 255};
    for (int i = 0; i < detect_result_group.count; i++)
    {
        detect_result_t *det_result = &(detect_result_group.results[i]);
        printf("%s @ (%d %d %d %d) %f\n",
               det_result->name,
               det_result->box.left, det_result->box.top, det_result->box.right, det_result->box.bottom,
               det_result->prop);
        int x1 = det_result->box.left;
        int y1 = det_result->box.top;
        int x2 = det_result->box.right;
        int y2 = det_result->box.bottom;
        //draw box
        img.draw_rectangle(x1, y1, x2, y2, blue, 1, ~0U);
        img.draw_text(x1, y1 - 12, det_result->name, blue);
    }
    img.save("./out.bmp");

    // Release
    if (ctx >= 0)
    {
        rknn_destroy(ctx);
    }
    if (model)
    {
        free(model);
    }
    if (input_data)
    {
        stbi_image_free(input_data);
    }
    return 0;
}
