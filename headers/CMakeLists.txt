# CMakeLists file for subdir: headers
cmake_minimum_required(VERSION 3.0)

include_directories(${GEN_CPP_2})
include_directories(${AUTH_HEADERS})

add_custom_target(HeadersDummy
    SOURCES
    "Service.h"
    "TestClient.h"
    "Utils.h"
    "PersonalSiteExceptions.h"
    "DataStructures.h")


