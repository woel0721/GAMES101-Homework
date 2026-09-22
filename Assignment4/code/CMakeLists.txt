cmake_minimum_required(VERSION 3.16)

# Prefer the standalone macOS toolchain, as in Assignment3.
set(_a4_clt "/Library/Developer/CommandLineTools")
if(CMAKE_HOST_APPLE AND NOT CMAKE_TOOLCHAIN_FILE
   AND NOT CMAKE_GENERATOR STREQUAL "Xcode"
   AND EXISTS "${_a4_clt}/usr/bin/clang++")
    if(NOT CMAKE_CXX_COMPILER OR CMAKE_CXX_COMPILER MATCHES "^/usr/bin/(c\\+\\+|clang\\+\\+|g\\+\\+)$")
        set(CMAKE_CXX_COMPILER "${_a4_clt}/usr/bin/clang++" CACHE FILEPATH "C++ compiler" FORCE)
    endif()
    if(CMAKE_CXX_COMPILER STREQUAL "${_a4_clt}/usr/bin/clang++")
        set(ENV{DEVELOPER_DIR} "${_a4_clt}")
        if(NOT CMAKE_OSX_SYSROOT)
            set(CMAKE_OSX_SYSROOT "${_a4_clt}/SDKs/MacOSX.sdk" CACHE PATH "macOS SDK" FORCE)
        endif()
        if(CMAKE_GENERATOR STREQUAL "Unix Makefiles"
           AND (NOT CMAKE_MAKE_PROGRAM OR CMAKE_MAKE_PROGRAM STREQUAL "/usr/bin/make"))
            set(CMAKE_MAKE_PROGRAM "${_a4_clt}/usr/bin/make" CACHE FILEPATH "Make program" FORCE)
        endif()
    endif()
endif()

project(BezierCurve LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui)

add_executable(BezierCurve main.cpp)
target_compile_features(BezierCurve PRIVATE cxx_std_17)
target_include_directories(BezierCurve PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(BezierCurve PRIVATE ${OpenCV_LIBRARIES})
