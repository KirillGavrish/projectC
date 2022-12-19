cmake_minimum_required(VERSION 3.0)
project(camera)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE src/)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
