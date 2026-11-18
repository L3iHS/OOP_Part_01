cmake_minimum_required(VERSION 3.20)
project(oop_lab3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_SANITIZERS "Включить AddressSanitizer и UBSan" OFF)

add_library(temperature_ops STATIC
    src/temperature.cpp
    src/temperature_set.cpp
)
target_include_directories(temperature_ops PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(lab_03 src/main.cpp)
target_link_libraries(lab_03 PRIVATE temperature_ops)

set(lab_targets temperature_ops lab_03)

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

    add_executable(lab_03_tests
        tests/test_temperature.cpp
        tests/test_lifecycle.cpp
        tests/test_operations.cpp
    )
    target_link_libraries(lab_03_tests PRIVATE temperature_ops GTest::gtest_main)
    list(APPEND lab_targets lab_03_tests)

    include(GoogleTest)
    gtest_discover_tests(lab_03_tests DISCOVERY_TIMEOUT 20)
endif()

foreach(target IN LISTS lab_targets)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    if(ENABLE_SANITIZERS)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${target} PRIVATE
                -fsanitize=address,undefined -fno-omit-frame-pointer
            )
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
        else()
            message(FATAL_ERROR "Для ENABLE_SANITIZERS требуется GCC или Clang")
        endif()
    endif()
endforeach()
