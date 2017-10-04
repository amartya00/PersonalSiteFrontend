# CMakeLists file for subdir: libs
cmake_minimum_required(VERSION 3.0)
add_compile_options(-fPIC)

include_directories(
    ${GEN_CPP_2}
    ${HEADERS}
    ${AUTH_HEADERS}
    /usr/local/lib/
    /home/amartya/Projects/HelloWorldService/fbthrift/
    )

file(GLOB GENERATED "${GEN_CPP_2}/*.cpp")

add_library(ServerLib SHARED
    "Service.cc"

    "utils/Utils.cc"

    "exceptions/Exceptions.cc"

    $<TARGET_OBJECTS:GeneratedObjects>
    )

target_link_libraries(
    ServerLib
    auth
    boost_system
    pthread
    glog
    folly
    thrift
    thriftcpp2
    gflags
    thriftprotocol
    zstd
    crypto
    aws-cpp-sdk-core
    aws-cpp-sdk-dynamodb
    jsoncpp
    curl)

install(TARGETS ServerLib DESTINATION lib)
