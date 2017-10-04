# CMakeLists file for subdir: src
cmake_minimum_required(VERSION 3.0)

add_compile_options(-fPIC)

file(GLOB GENERATED "${GEN_CPP_2}/*.cpp")

include_directories(
    ${GEN_CPP_2}
    ${HEADERS}
    ${AUTH_HEADERS}
    /usr/local/lib/
    /home/amartya/Projects/HelloWorldService/fbthrift/
    )

add_executable(Server
    "Server.cc")

add_executable(TestClient
    "TestClient.cc"
    $<TARGET_OBJECTS:GeneratedObjects>
    )

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
    aws-cpp-sdk-core
    aws-cpp-sdk-dynamodb
    jsoncpp
    curl
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

install(TARGETS Server TestClient DESTINATION bin)
