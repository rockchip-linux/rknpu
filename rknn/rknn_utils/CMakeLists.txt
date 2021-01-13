cmake_minimum_required(VERSION 3.4.1)

project(rknn_matmul_sample_linux)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (CMAKE_C_COMPILER MATCHES "aarch64")
  set(LIB_ARCH lib64)
else()
  set(LIB_ARCH lib)
endif()

# rknn_utils
include_directories(${CMAKE_SOURCE_DIR}/librknn_utils/include)
set(RKNN_UTILS_LIB ${CMAKE_CURRENT_SOURCE_DIR}/librknn_utils/${LIB_ARCH}/librknn_utils.so)

add_executable(rknn_matmul_sample
  example/matmul_sample.c
)

target_link_libraries(rknn_matmul_sample
  "-Wl,--allow-shlib-undefined" ${RKNN_UTILS_LIB}
)

# install target
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/rknn_matmul_sample)
install(TARGETS rknn_matmul_sample DESTINATION bin)
install(PROGRAMS ${RKNN_UTILS_LIB} DESTINATION lib)
