/**                                                                                                                                          
 * Example: Calculate (x-y)^2 using Matmul API
 */


/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <math.h>
#include "rknn_matmul_api.h"

/*-------------------------------------------
        Macros and Variables
-------------------------------------------*/

double __get_us(struct timeval t) { return (t.tv_sec * 1000000 + t.tv_usec); }

static int gen_random_data(int8_t *x,int len,int min_value,int max_value) {
    srand((unsigned int)time(NULL));
    int range = max_value-min_value+1;
    for (int i = 0;i<len;++i) {
        x[i] = (int)rand()%range+min_value;
    }
    return 0;
}

//sum([K,1]*[K,N]) =[1,N]
//dim_len=K,batch_size=N
static int multiply_sum_cpu(int8_t *x,int8_t *y,int dim_len,int batch_size,float *res){
    for (int i = 0;i<batch_size;++i) {
        float sum = 0.0;
        for(int j = 0;j<dim_len;++j) {
            int offset = j*batch_size+i;
            float mul = ((int32_t)x[j])*((int32_t)y[offset]);
            sum+=mul;
        }
        res[i] = sum;
    }
    return 0;
}

static int l2_distance_cpu(int8_t *x,int8_t *y,int dim_len,int batch_size,float *res){
    for (int i = 0;i<batch_size;++i) {
        float sum = 0.0;
        for(int j = 0;j<dim_len;++j) {
            int offset = j*batch_size+i;
            float mul = ((int)x[j])-((int)y[offset]);
            sum+=mul*mul;
        }
        res[i] = sum;
    }
    return 0;
}

//calculate y^2
static int pre_process_y(int8_t * y,int dim_len, int batch_size, int *square_y) {
    for (int i = 0;i<batch_size;++i) {
        int sum = 0;
        for(int j = 0;j<dim_len;++j) {
            int offset = j*batch_size+i;
            int mul = ((int)y[offset])*((int)y[offset]);
            sum+=mul;
        }
        square_y[i] = sum;
    }
    return 0;
}

static int get_sum(int8_t * x, int *square_y,int dim_len, int batch_size,float *xy) {
    int x_sum = 0;
    for(int j = 0;j<dim_len;++j) {
        x_sum += ((int)x[j])*((int)x[j]);
    }
    for (int i = 0;i<batch_size;++i) {
        xy[i] = x_sum-2*xy[i]+square_y[i];
    }
    return 0;
}

static float cal_relative_err(float *rknn_out,float *cpu_out,int len) {
    float sum = 0.0;
    for(int i = 0;i<len;++i) {
       float err = (fabs(rknn_out[i] - cpu_out[i]))/(cpu_out[i]+0.000000001); 
       sum+=err;
    }
    return sum/(float)len;
}


/*-------------------------------------------
                  Main Functions
-------------------------------------------*/
int main
    (
    int argc,
    char **argv
    )
{
    if(argc != 3)
    {
        printf("Info: Get L2 distance for matrix A: Kx1, matrix B: KxN\n");
        printf("Usage: %s <K> <N>\n",argv[0]);
        return -1;
    }
    int DIM = atoi(argv[1]);
    int BATCH_SIZE = atoi(argv[2]);
    rknn_tensor_type dtype = RKNN_TENSOR_INT8;
    int8_t *x = (int8_t *)malloc(DIM*sizeof(int8_t));
    int8_t *y = (int8_t *)malloc(DIM*BATCH_SIZE*sizeof(int8_t));
    float out_fp32_buf[BATCH_SIZE];
    float res_cpu[BATCH_SIZE];
    int x_size = DIM;
    int y_size = DIM*BATCH_SIZE;
    int test_count = 100;
    float err;
    struct timeval start_time, stop_time;
    
    int *square_y = (int *)malloc(BATCH_SIZE*sizeof(int));
    
    //generate random test data
    gen_random_data(x,x_size,-128,127);
    gen_random_data(y,y_size,-128,127);
    pre_process_y(y,DIM,BATCH_SIZE,square_y);

    rknn_matmul_handle_t handle= rknn_matmul_load(x,y, 1, DIM, BATCH_SIZE, dtype);
    if (handle == NULL)
    {
        fprintf(stderr, "rknn_matmul_load failed.\n");
        return -1;
    }
    
    //loop for matmul
    gettimeofday(&start_time, NULL);
    for(int i = 0;i<test_count;++i) {
        // can set x/y values before run
        // ...
        rknn_matmul_run(handle,out_fp32_buf);
    }
    gettimeofday(&stop_time, NULL);
    printf("run Matmul %d times, average time:%f ms\n", test_count ,(__get_us(stop_time) - __get_us(start_time)) / (1000.0*test_count));
    
    //get matmul err
    int element_num = BATCH_SIZE;
    multiply_sum_cpu(x,y,DIM,BATCH_SIZE,res_cpu);
    //show 10 data
    printf("compare first 10 results (npu vs cpu)\n");
    for(int i = 0;i<(10<BATCH_SIZE?10:BATCH_SIZE); ++i)
    {
        printf("[%d] % -12.2f vs % -8.2f\n",i,out_fp32_buf[i],res_cpu[i]);
    }
    err = cal_relative_err(out_fp32_buf,res_cpu,element_num);
    printf("[Matmul INT8] average relative err = %f\n",err);
    
    //get (x-y)^2 err
    get_sum(x,square_y,DIM,BATCH_SIZE,out_fp32_buf);
    l2_distance_cpu(x,y,DIM,BATCH_SIZE,res_cpu);
    err = cal_relative_err(out_fp32_buf,res_cpu,element_num);
    printf("[L2 distance INT8] average relative err = %f\n",err);

    rknn_matmul_unload(handle);
    if(square_y) {
        free(square_y);
    }
    if(x)
    {
        free(x);
    }
    if(y)
    {
        free(y);
    }

    return 0;    
}

