#ifndef RKNN_RUNTIME_H
#define RKNN_RUNTIME_H

/**
 * @brief RKNN上下文句柄
 */
typedef unsigned long long rknn_context_t;

/**
 * @brief 创建RKNN上下文句柄
 * 
 * @return rknn_context_t RKNN上下文句柄
 */
rknn_context_t RKNNCreateContext();

/**
 * @brief 释放RKNN上下文句柄
 * 
 * @param context RKNN上下文句柄
 * @return int 0: 执行成功， <0: 执行失败
 */
int RKNNReleaseContext(rknn_context_t context);

/**
 * @brief 加载并编译RKNN模型
 * 
 * @param context RKNN上下文句柄
 * @param model_path RKNN模型路径
 * @return int 0: 执行成功， <0: 执行失败
 */
int RKNNBuildGraph(rknn_context_t context, const char* model_path);

/**
 * @brief 直接通过Buffer加载并编译RKNN模型
 * 
 * @param context RKNN上下文句柄
 * @param rknn_model RKNN模型数据
 * @param size RKNN模型数据大小
 * @return int 0: 执行成功， <0: 执行失败
 */
int RKNNBuildGraphFromBuf(rknn_context_t context, const char* rknn_model, int size);

/**
 * @brief 设置模型输入数据
 * 
 * @param context RKNN上下文句柄
 * @param index 输入索引
 * @param in_data 输入数据
 * @param size 输入数据大小
 * @return int 0: 执行成功， <0: 执行失败
 */
int RKNNSetInput(rknn_context_t context, int index, const char* in_data, int size);

/**
 * @brief 获取模型输出数据数目
 * 
 * @param context RKNN上下文句柄
 * @return int 输出数据数目
 */
int RKNNGetOutputNum(rknn_context_t context);

/**
 * @brief 获取模型输出数据的大小
 * 
 * @param context RKNN上下文句柄
 * @param index 输出数据索引
 * @return int 输出数据的大小
 */
int RKNNGetOutputSize(rknn_context_t context, int index);

/**
 * @brief 获取模型输出数据
 * 
 * @param context RKNN上下文句柄
 * @param index 输出数据索引
 * @param out_data 输出数据存放数组
 * @param out_size 输出数据大小
 * @return int 0: 执行成功， <0: 执行失败
 */
int RKNNGetOutput(rknn_context_t context, int index, float* out_data, int out_size);

/**
 * @brief 运行模型
 * 
 * @param context RKNN上下文句柄
 * @return int 0: 执行成功， <0: 执行失败
 */
int RKNNRun(rknn_context_t context);

#endif //RKNN_RUNTIME_H
