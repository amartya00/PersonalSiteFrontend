# CMakeLists file for subdir: /home/amartya/Projects/PersonalSite
cmake_minimum_required(VERSION 3.0)

# Project settings
project(PersonalSite_Frontend)
enable_testing()

set(SERVICE_FILE  ${CMAKE_CURRENT_SOURCE_DIR}/thrift/Service.thrift)
set(HEADERS       ${CMAKE_CURRENT_SOURCE_DIR}/headers)
set(LIB           ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SRC           ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GEN_LIB       ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(GEN_CPP_2_OUT ${CMAKE_CURRENT_BINARY_DIR})
set(GEN_CPP_2     ${GEN_CPP_2_OUT}/gen-cpp2)

# Subprojects
add_subdirectory(src)
add_subdirectory(headers)
add_subdirectory(thrift)
add_subdirectory(lib)
add_subdirectory(config)

message("Folder: ${HEADERS}")

add_custom_target(Dummy
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake)

add_custom_target(
    Format
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/headers" "h"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/lib" "cc"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/src" "cc")

