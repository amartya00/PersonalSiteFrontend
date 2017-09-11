# CMakeLists file for subdir: headers
cmake_minimum_required(VERSION 3.0)

include_directories(${GEN_CPP_2})

add_custom_target(HeadersDummy
    SOURCES
    "Service.h"
    "TestClient.h"
    "Auth.h"
    "Utils.h"
    "Exceptions.h")

