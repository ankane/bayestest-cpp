cmake_minimum_required(VERSION 3.18)

project(bayestest VERSION 0.1.2 LANGUAGES CXX)

include(GNUInstallDirs)

add_library(bayestest INTERFACE)
add_library(bayestest::bayestest ALIAS bayestest)

target_compile_features(bayestest INTERFACE cxx_std_11)

target_include_directories(
    bayestest
    INTERFACE
    "${PROJECT_SOURCE_DIR}/include/"
)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(BUILD_TESTING "" ON)

    if(BUILD_TESTING)
        add_executable(test test/main.cpp)
        target_link_libraries(test PRIVATE bayestest::bayestest)
        if(NOT MSVC)
            target_compile_options(test PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Werror)
        endif()
    endif()
endif()
