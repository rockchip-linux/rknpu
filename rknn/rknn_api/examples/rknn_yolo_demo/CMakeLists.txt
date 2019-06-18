cmake_minimum_required(VERSION 3.4.1)

project(rknn_yolo_demo_linux)

set(CMAKE_SYSTEM_NAME Linux)

set(GCC_COMPILER_PATH ${CMAKE_SOURCE_DIR}/../../../../../../prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu)

set(CMAKE_C_COMPILER ${GCC_COMPILER_PATH}/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${GCC_COMPILER_PATH}/bin/aarch64-linux-gnu-g++)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# rknn api
set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/../libs/librknn_api)
include_directories(${RKNN_API_PATH}/include)
set(RKNN_API_LIB ${RKNN_API_PATH}/lib64/librknn_api.so)

# opencv
set(OPENCV_PATH ${CMAKE_SOURCE_DIR}/../libs/opencv)
include_directories(${OPENCV_PATH}/include)
file(GLOB OPENCV_LIBS ${OPENCV_PATH}/lib64/*.so*)

set(CMAKE_INSTALL_RPATH "lib")

add_executable(rknn_yolo_demo
        src/main.cc
        src/yolov3_post_process.cc
        )

target_link_libraries(rknn_yolo_demo
	${RKNN_API_LIB}
	${OPENCV_LIBS}
)

# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/rknn_yolo_demo)
install(TARGETS rknn_yolo_demo DESTINATION ./)
install(FILES model/yolov3.rknn DESTINATION ./)
install(FILES model/dog.jpg DESTINATION ./)
install(PROGRAMS ${RKNN_API_LIB} DESTINATION lib)
install(PROGRAMS ${OPENCV_LIBS} DESTINATION lib)
