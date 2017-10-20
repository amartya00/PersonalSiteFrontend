# CMakeLists file for subdir: /home/amartya/Projects/PersonalSite
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
cmake_minimum_required(VERSION 3.0)

# Project settings
project(PersonalSite_Frontend)
enable_testing()

set(SERVICE_FILE  ${CMAKE_CURRENT_SOURCE_DIR}/thrift/Service.thrift)
set(HEADERS       ${CMAKE_CURRENT_SOURCE_DIR}/headers)
set(AUTH_HEADERS  /usr/include/auth/)
set(LIB           ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SRC           ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SCRIPTS       ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
set(CONFIGS       ${CMAKE_CURRENT_SOURCE_DIR}/config)
set(GEN_LIB       ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(GEN_CPP_2_OUT ${CMAKE_CURRENT_BINARY_DIR})
set(GEN_CPP_2     ${GEN_CPP_2_OUT}/gen-cpp2)

set(PACKAGE_CACHE ${PACKAGE_CACHE})

set(CMAKE_BUILD_RPATH ${PACKAGE_CACHE})

# Find the required libraries
find_library(AUTH           NAMES auth                 PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BOOST_SYSTEM   NAMES boost_system         PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(PTHREAD        NAMES pthread              PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(FOLLY          NAMES folly                PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(THRIFT         NAMES thrift               PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(THRIFTCPP2     NAMES thriftcpp2           PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(GFLAGS         NAMES gflags               PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(THRIFTPROTOCOL NAMES thriftprotocol       PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(ZSTD           NAMES zstd                 PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(CRYPTO         NAMES crypto               PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(AWS_SDK_CORE   NAMES aws-cpp-sdk-core     PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(DYNAMODB       NAMES aws-cpp-sdk-dynamodb PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(JSONCPP        NAMES jsoncpp              PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(CURL           NAMES curl                 PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(GAPI           NAMES gapiwrapper          PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(GLOG           NAMES glog                 PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(SSL            NAMES ssl                  PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(PYTHON27       NAMES python2.7            PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(BOOST_PYTHON   NAMES boost_python         PATHS ${PACKAGE_CACHE}/lib NO_DEFAULT_PATH NO_CMAKE_SYSTEM_PATH)

find_library(AUTH           NAMES auth                 PATHS ${PACKAGE_CACHE}/lib)
find_library(BOOST_SYSTEM   NAMES boost_system         PATHS ${PACKAGE_CACHE}/lib)
find_library(PTHREAD        NAMES pthread              PATHS ${PACKAGE_CACHE}/lib)
find_library(FOLLY          NAMES folly                PATHS ${PACKAGE_CACHE}/lib)
find_library(THRIFT         NAMES thrift               PATHS ${PACKAGE_CACHE}/lib)
find_library(THRIFTCPP2     NAMES thriftcpp2           PATHS ${PACKAGE_CACHE}/lib)
find_library(GFLAGS         NAMES gflags               PATHS ${PACKAGE_CACHE}/lib)
find_library(THRIFTPROTOCOL NAMES thriftprotocol       PATHS ${PACKAGE_CACHE}/lib)
find_library(ZSTD           NAMES zstd                 PATHS ${PACKAGE_CACHE}/lib)
find_library(CRYPTO         NAMES crypto               PATHS ${PACKAGE_CACHE}/lib)
find_library(AWS_SDK_CORE   NAMES aws-cpp-sdk-core     PATHS ${PACKAGE_CACHE}/lib)
find_library(DYNAMODB       NAMES aws-cpp-sdk-dynamodb PATHS ${PACKAGE_CACHE}/lib)
find_library(JSONCPP        NAMES jsoncpp              PATHS ${PACKAGE_CACHE}/lib)
find_library(CURL           NAMES curl                 PATHS ${PACKAGE_CACHE}/lib)
find_library(GAPI           NAMES gapiwrapper          PATHS ${PACKAGE_CACHE}/lib)
find_library(GLOG           NAMES glog                 PATHS ${PACKAGE_CACHE}/lib)
find_library(SSL            NAMES ssl                  PATHS ${PACKAGE_CACHE}/lib)
find_library(PYTHON27       NAMES python2.7            PATHS ${PACKAGE_CACHE}/lib)
find_library(BOOST_PYTHON   NAMES boost_python         PATHS ${PACKAGE_CACHE}/lib)

message(STATUS "Using ${AUTH}")
message(STATUS "Using ${BOOST_SYSTEM}")
message(STATUS "Using ${PTHREAD}")
message(STATUS "Using ${FOLLY}")
message(STATUS "Using ${THRIFT}")
message(STATUS "Using ${THRIFTCPP2}")
message(STATUS "Using ${GFLAGS}")
message(STATUS "Using ${THRIFTPROTOCOL}")
message(STATUS "Using ${ZSTD}")
message(STATUS "Using ${CRYPTO}")
message(STATUS "Using ${AWS_SDK_CORE}")
message(STATUS "Using ${DYNAMODB}")
message(STATUS "Using ${JSONCPP}")
message(STATUS "Using ${CURL}")
message(STATUS "Using ${GAPI}")
message(STATUS "Using ${GLOG}")
message(STATUS "Using ${SSL}")
message(STATUS "Using ${PYTHON27}")
message(STATUS "Using ${BOOST_PYTHON}")

# Subprojects
add_subdirectory(src)
add_subdirectory(headers)
add_subdirectory(thrift)
add_subdirectory(lib)
add_subdirectory(config)
add_subdirectory(scripts)
add_subdirectory(snappyyml)

add_custom_target(Dummy
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake)

add_custom_target(
    Format
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/headers"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/FormatFiles.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/src")

# install scripts and config files
file(GLOB scripts "${SCRIPTS}/frontend-server")
file(GLOB configs "${CONFIGS}/*.json")

install(FILES ${scripts} DESTINATION bin PERMISSIONS WORLD_EXECUTE OWNER_EXECUTE GROUP_EXECUTE OWNER_READ WORLD_READ GROUP_READ)
install(FILES ${configs} DESTINATION configs)

