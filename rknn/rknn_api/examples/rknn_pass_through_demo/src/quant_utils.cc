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

#include "quant_utils.h"
#include "rknn_api.h"

inline static int32_t __clip(float val, float min, float max)
{
    float f = val <= min ? min : (val >= max ? max : val);
    // return (int32_t)((f > 0.0) ? (f + 0.5) : (f - 0.5));    //四舍五入
    return f;
}

void f32_to_f16(uint16_t *f16, float *f32, int num)
{
    float *src = f32;
    uint16_t *dst = f16;
    int i = 0;

    for (; i < num; i++)
    {
        float in = *src;

        uint32_t fp32 = *((uint32_t *)&in);
        uint32_t t1 = (fp32 & 0x80000000u) >> 16; /* sign bit. */
        uint32_t t2 = (fp32 & 0x7F800000u) >> 13; /* Exponent bits */
        uint32_t t3 = (fp32 & 0x007FE000u) >> 13; /* Mantissa bits, no rounding */
        uint32_t fp16 = 0u;

        if (t2 >= 0x023c00u)
        {
            fp16 = t1 | 0x7BFF; /* Don't round to infinity. */
        }
        else if (t2 <= 0x01c000u)
        {
            fp16 = t1;
        }
        else
        {
            t2 -= 0x01c000u;
            fp16 = t1 | t2 | t3;
        }

        *dst = (uint16_t)fp16;

        src++;
        dst++;
    }
}

void qnt_f32_to_dfp(uint8_t *qnt, uint8_t type, int8_t fl, float *f32,
                    int num)
{
    float *src_ptr = f32;
    int i = 0;
    float dst_val = 0.0;

    switch (type)
    {
    case RKNN_TENSOR_INT8:
        for (; i < num; i++)
        {
            dst_val = (fl > 0) ? ((*src_ptr) * ((float)(1 << fl)))
                               : ((*src_ptr) / (float)(1 << -fl));
            *((int8_t *)qnt) = (int8_t)__clip(dst_val, -128, 127);
            src_ptr++;
            qnt++;
        }
        break;
    case RKNN_TENSOR_UINT8:
        for (; i < num; i++)
        {
            dst_val = (fl > 0) ? ((*src_ptr) * ((float)(1 << fl)))
                               : ((*src_ptr) / (float)(1 << -fl));
            *qnt = (uint8_t)__clip(dst_val, 0, 255);
            src_ptr++;
            qnt++;
        }
        break;
    case RKNN_TENSOR_INT16:
        for (; i < num; i++)
        {
            dst_val = (fl > 0) ? ((*src_ptr) * ((float)(1 << fl)))
                               : ((*src_ptr) / (float)(1 << -fl));
            *((int16_t *)qnt) = (int16_t)__clip(dst_val, -32768, 32767);
            src_ptr++;
            qnt += 2;
        }
        break;
    default:
        break;
    }
}

void qnt_f32_to_affine(uint8_t *qnt, uint8_t type, uint8_t zp, float scale,
                       float *f32, int num)
{
    float *src_ptr = f32;
    int i = 0;
    float dst_val = 0.0;

    switch (type)
    {
    case RKNN_TENSOR_INT8:
        for (; i < num; i++)
        {
            dst_val = ((*src_ptr) / scale) + zp;
            *((int8_t *)qnt) = (int8_t)__clip(dst_val, -128, 127);
            src_ptr++;
            qnt++;
        }
        break;
    case RKNN_TENSOR_UINT8:
        for (; i < num; i++)
        {
            dst_val = ((*src_ptr) / scale) + zp;
            *qnt = (uint8_t)__clip(dst_val, 0, 255);
            src_ptr++;
            qnt++;
        }
        break;
    case RKNN_TENSOR_INT16:
        for (; i < num; i++)
        {
            dst_val = ((*src_ptr) / scale) + zp;
            *((int16_t *)qnt) = (int16_t)__clip(dst_val, -32768, 32767);
            src_ptr++;
            qnt += 2;
        }
        break;
    default:
        break;
    }
}

void qnt_f32_to_none(uint8_t *qnt, uint8_t type, float *f32, int num)
{
    float *src_ptr = f32;
    int i = 0;

    switch (type)
    {
    case RKNN_TENSOR_INT8:
        for (; i < num; i++)
        {
            *((int8_t *)qnt) = (int8_t)__clip(*src_ptr, -128, 127);
            src_ptr++;
            qnt++;
        }
        break;
    case RKNN_TENSOR_UINT8:
        for (; i < num; i++)
        {
            *qnt = (uint8_t)__clip(*src_ptr, 0, 255);
            src_ptr++;
            qnt++;
        }
        break;
    case RKNN_TENSOR_INT16:
        for (; i < num; i++)
        {
            *((int16_t *)qnt) = (int16_t)__clip(*src_ptr, -32768, 32767);
            src_ptr++;
            qnt += 2;
        }
        break;
    default:
        break;
    }
}