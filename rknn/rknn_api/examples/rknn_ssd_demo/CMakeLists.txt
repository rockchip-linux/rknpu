cmake_minimum_required(VERSION 3.4.1)

project(rknn_ssd_demo_linux)

set(CMAKE_SYSTEM_NAME Linux)

# for aarch64
set(CROSS_COMPILE_TOOL ${CMAKE_SOURCE_DIR}/../../../../../../prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu)

# for armhf
# set(CROSS_COMPILE_TOOL ${CMAKE_SOURCE_DIR}/../../../../../../prebuilts/gcc/linux-x86/arm/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf)

set(CMAKE_C_COMPILER ${CROSS_COMPILE_TOOL}-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE_TOOL}-g++)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (CMAKE_C_COMPILER MATCHES "aarch64")
  set(LIB_ARCH lib64)
else()
  set(LIB_ARCH lib)
endif()

# rknn api
set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/../../librknn_api)
include_directories(${RKNN_API_PATH}/include)
set(RKNN_API_LIB ${RKNN_API_PATH}/${LIB_ARCH}/librknn_api.so)

# opencv
set(OPENCV_PATH ${CMAKE_SOURCE_DIR}/../libs/opencv)
include_directories(${OPENCV_PATH}/include)
file(GLOB OPENCV_LIBS ${OPENCV_PATH}/${LIB_ARCH}/*.so*)

set(CMAKE_INSTALL_RPATH "lib")

add_executable(rknn_ssd_demo
        src/main.cc
        src/ssd.cc
        )

target_link_libraries(rknn_ssd_demo
	${RKNN_API_LIB}
	${OPENCV_LIBS}
)

# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/rknn_ssd_demo)
install(TARGETS rknn_ssd_demo DESTINATION ./)
install(FILES model/ssd_inception_v2.rknn DESTINATION ./)
install(FILES model/road.bmp DESTINATION ./)
install(FILES model/box_priors.txt DESTINATION ./)
install(FILES model/coco_labels_list.txt DESTINATION ./)
install(PROGRAMS ${RKNN_API_LIB} DESTINATION lib)
install(PROGRAMS ${OPENCV_LIBS} DESTINATION lib)
