cmake_minimum_required(VERSION 3.16)
project(oop_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(array_ops STATIC src/array_ops.cpp)
target_include_directories(array_ops PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(lab1 main.cpp)
target_link_libraries(lab1 PRIVATE array_ops)

set(lab_targets array_ops lab1)

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)

    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()

    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
        URL_HASH SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(lab1_tests tests/test_funcs.cpp tests/test_variant.cpp)
    target_link_libraries(lab1_tests PRIVATE array_ops GTest::gtest_main)
    list(APPEND lab_targets lab1_tests)

    include(GoogleTest)
    gtest_discover_tests(lab1_tests)
endif()

foreach(target IN LISTS lab_targets)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()
