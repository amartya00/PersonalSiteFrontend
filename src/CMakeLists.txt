# CMakeLists file for subdir: src
cmake_minimum_required(VERSION 3.0)

file(GLOB GENERATED "${GEN_CPP_2}/*.cpp")

include_directories(
    ${GEN_CPP_2}
    ${HEADERS}
    /usr/local/lib/
    /home/amartya/Projects/HelloWorldService/fbthrift/
    )

add_executable(Server
    "Server.cc")

add_executable(TestClient
    "TestClient.cc"
    ${GENERATED})

target_link_libraries(
    Server
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
    )

target_link_libraries(
    TestClient
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
    )
