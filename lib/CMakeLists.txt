# CMakeLists file for subdir: libs
cmake_minimum_required(VERSION 3.0)
add_compile_options(-fPIC)

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
    "auth/AuthUserItem.cc"
    "auth/Auth.cc"
    "utils/Utils.cc"
    "exceptions/Exceptions.cc"
    ${GENERATED}
    )

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
    aws-cpp-sdk-core
    aws-cpp-sdk-dynamodb
    jsoncpp
    curl)

add_dependencies(ServerLib BuildServiceDefs)


