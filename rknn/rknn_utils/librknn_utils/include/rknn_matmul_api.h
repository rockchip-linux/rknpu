/****************************************************************************
*
*    Copyright (c) 2017 - 2021 by Rockchip Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Rockchip Corporation. This is proprietary information owned by
*    Rockchip Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Rockchip Corporation.
*
*****************************************************************************/


#ifndef _RKNN_MATMUL_H
#define _RKNN_MATMUL_H


/**
 *  Perform specific Matrix Multiply like AxB = C, where,
 *  Matrix A : [input] shape is [K,M], K is the high rank dimension in memory, M is the low rank dimension in memory and must be 1 now!
 *  Matrix B : [input] shape is [K,N], N is the low rank dimension in memory.
 *  Matrix C : [output] shape is [M,N]
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
    the tensor data type.
*/

typedef enum _rknn_tensor_type {
    RKNN_TENSOR_FLOAT32 = 0,                            /* data type is float32. */
    RKNN_TENSOR_FLOAT16,                                /* data type is float16. */
    RKNN_TENSOR_INT8,                                   /* data type is int8. */
    RKNN_TENSOR_UINT8,                                  /* data type is uint8. */
    RKNN_TENSOR_INT16,                                  /* data type is int16. */

    RKNN_TENSOR_TYPE_MAX
} rknn_tensor_type;

#ifdef __arm__
typedef uint32_t rknn_context;
#else
typedef uint64_t rknn_context;
#endif


/*
    Definition of handle for rknn_matmul_run.
*/
typedef struct _rknn_matmul_t{
    void*               A;     
    void*               B;        
    int32_t             M;                  /* the low rank dimension of A */
    int32_t             K;                  /* the high rank dimension of A and B*/
    int32_t             N;                  /* the low rank dimension of B */
    rknn_tensor_type    in_dtype;           /* the input buffer type */
    rknn_context        rknn_ctx;           /* rknn context handle */
} rknn_matmul_t;

typedef rknn_matmul_t* rknn_matmul_handle_t;

/*  rknn_matmul_load

    load input buffer and initial the context.

    [input]:
        void* a                             the pointer of A Matrix buffer, number of element is K*M, alloced by user.
        void* b                             the pointer to B Matrix buffer, number of element is K*N, alloced by user.
        int32_t M                           the low rank dimension of A
        int32_t K                           the high rank dimension of A
        int32_t N                           the low rank dimension of B
        rknn_tensor_type dtype;             the input buffer type, now only support RKNN_TENSOR_UINT8 and RKNN_TENSOR_INT8.
    [output]:
        rknn_matmul_handle_t                the handle of mamul. it will return NULL if failed.
*/
rknn_matmul_handle_t rknn_matmul_load(void *a, void *b, int M, int K, int N, rknn_tensor_type dtype);

/*  rknn_matmul_run

    run Matmul with fixed shape.

    [input]:
        rknn_matmul_handle_t matmul_handle  the handle of mamul, get by rknn_matmul_load.
        float *c                            the pointer of C Matrix buffer, number of element is 256x4096, alloced by user. 
    [output]:
        int                                 error code.
*/
int rknn_matmul_run(rknn_matmul_handle_t matmul_handle, float *c);

/*  rknn_matmul_unload

    destroy the context.

    [input]:
        rknn_matmul_handle_t matmul_handle  the handle of mamul, get by rknn_matmul_load.
    [output]:
        int                                 error code.
*/
int rknn_matmul_unload(rknn_matmul_handle_t matmul_handle);

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_RKNN_MATMUL_H