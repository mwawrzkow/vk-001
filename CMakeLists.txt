cmake_minimum_required(VERSION 3.14)
project(vk-001)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

set(SRC src/main.cpp)
set(HPP include/main.hpp)


add_executable(${PROJECT_NAME} ${SRC} ${HPP})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
