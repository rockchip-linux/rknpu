cmake_minimum_required(VERSION 3.4.1)

project(rknn_yolo_demo_linux)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER ${CMAKE_SOURCE_DIR}/../../../../../prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/../../../../../prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(RKNPU_PATH ${CMAKE_SOURCE_DIR}/../../../)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/libs/opencv/include
        ${RKNPU_PATH}/rknn/include
)

set(RKNPU_PATH ${CMAKE_SOURCE_DIR}/../../../)

set(link_libs  ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_core.so
               ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_highgui.so
               ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_imgcodecs.so
               ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_imgproc.so
               ${RKNPU_PATH}/rknn/usr/lib/librknn_runtime.so
)

link_directories(${RKNPU_PATH}/drivers)

set(CMAKE_INSTALL_RPATH "lib")

add_executable(rknn_yolo_demo
        src/main.cc
        src/yolov3_post_process.cc
        )

target_link_libraries(rknn_yolo_demo ${link_libs})

# install target and libraries
install(TARGETS rknn_yolo_demo DESTINATION ${CMAKE_SOURCE_DIR}/install/rknn_yolo_demo)
install(FILES model/yolov3.rknn DESTINATION ${CMAKE_SOURCE_DIR}/install/rknn_yolo_demo)
install(FILES model/dog.jpg DESTINATION ${CMAKE_SOURCE_DIR}/install/rknn_yolo_demo)
set(DEPENDENCIES
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_core.so
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_core.so.3.4
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_highgui.so
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_highgui.so.3.4
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_imgcodecs.so
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_imgcodecs.so.3.4
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_imgproc.so
        ${CMAKE_SOURCE_DIR}/libs/opencv/lib64/libopencv_imgproc.so.3.4
        )
foreach(DEPENDENCY ${DEPENDENCIES})
    install(PROGRAMS "${DEPENDENCY}" DESTINATION ${CMAKE_SOURCE_DIR}/install/rknn_yolo_demo/lib)
endforeach()
