cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(OPENCV_DIR /home/yzy/3rdparty/opencv-3.4.8)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(Rasterizer main.cpp rasterizer.hpp rasterizer.cpp global.hpp Triangle.hpp Triangle.cpp)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
