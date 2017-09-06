# CMakeLists file for subdir: libs
cmake_minimum_required(VERSION 3.0)

include_directories(
    ${GEN_CPP_2}
    ${HEADERS}
    /usr/local/lib/
    /home/amartya/Projects/HelloWorldService/fbthrift/
    )

file(GLOB GENERATED "${GEN_CPP_2}/*.cpp")

add_library(ServerLib SHARED
    "Service.cc"
    "auth/getUidFromToken.cc"
    "utils/Utils.cc"
    ${GENERATED}
    )

add_library(TestClientLib SHARED
    "TestClient.cc"
    ${GENERATED})

target_link_libraries(
    ServerLib
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
    aws-cpp-sdk-core)

target_link_libraries(
    TestClientLib
    boost_system
    pthread
    glog
    folly
    thrift
    thriftcpp2
    gflags
    thriftprotocol
    zstd
    crypto)

add_dependencies(ServerLib BuildServiceDefs)
add_dependencies(TestClientLib BuildServiceDefs)

