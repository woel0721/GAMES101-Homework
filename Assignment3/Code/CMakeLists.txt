cmake_minimum_required(VERSION 3.16)

# Use the standalone macOS toolchain for local Makefile/Ninja builds.
# /usr/bin/c++ and /usr/bin/make dispatch through the selected Xcode app,
# which may require accepting a new license after an Xcode update.
set(_a3_clt "/Library/Developer/CommandLineTools")
if(CMAKE_HOST_APPLE AND NOT CMAKE_TOOLCHAIN_FILE
   AND NOT CMAKE_GENERATOR STREQUAL "Xcode"
   AND EXISTS "${_a3_clt}/usr/bin/clang++")
    if(NOT CMAKE_CXX_COMPILER OR CMAKE_CXX_COMPILER MATCHES "^/usr/bin/(c\\+\\+|clang\\+\\+|g\\+\\+)$")
        set(CMAKE_CXX_COMPILER "${_a3_clt}/usr/bin/clang++" CACHE FILEPATH "C++ compiler" FORCE)
    endif()
    if(CMAKE_CXX_COMPILER STREQUAL "${_a3_clt}/usr/bin/clang++")
        set(ENV{DEVELOPER_DIR} "${_a3_clt}")
        if(NOT CMAKE_OSX_SYSROOT)
            set(CMAKE_OSX_SYSROOT "${_a3_clt}/SDKs/MacOSX.sdk" CACHE PATH "macOS SDK" FORCE)
        endif()
        if(CMAKE_GENERATOR STREQUAL "Unix Makefiles"
           AND (NOT CMAKE_MAKE_PROGRAM OR CMAKE_MAKE_PROGRAM STREQUAL "/usr/bin/make"))
            set(CMAKE_MAKE_PROGRAM "${_a3_clt}/usr/bin/make" CACHE FILEPATH "Make program" FORCE)
        endif()
    endif()
endif()

project(Rasterizer LANGUAGES CXX)

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui)

add_executable(Rasterizer main.cpp rasterizer.hpp rasterizer.cpp global.hpp Triangle.hpp Triangle.cpp Texture.hpp Texture.cpp Shader.hpp OBJ_Loader.h)
target_compile_features(Rasterizer PRIVATE cxx_std_17)
target_include_directories(Rasterizer PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Rasterizer PRIVATE Eigen3::Eigen ${OpenCV_LIBRARIES})
#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)
