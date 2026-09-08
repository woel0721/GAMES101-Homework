cmake_minimum_required(VERSION 3.16)
project(Transformation LANGUAGES CXX)

find_package(Eigen3 REQUIRED NO_MODULE)

add_executable(Transformation main.cpp)
target_compile_features(Transformation PRIVATE cxx_std_17)
target_link_libraries(Transformation PRIVATE Eigen3::Eigen)
