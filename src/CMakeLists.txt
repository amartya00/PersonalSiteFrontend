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
    /usr/include/x86_64-linux-gnu/python2.7/
    /usr/include/python2.7
    )

add_executable(Server
    "Server.cc")

add_library(Client SHARED
    "Client.cc"
    $<TARGET_OBJECTS:GeneratedObjects>
    )

target_link_libraries(
    Server
    ServerLib
    ${BOOST_SYSTEM}
    ${PTHREAD}
    ${GLOG}
    ${FOLLY}
    ${THRIFT}
    ${THRIFTCPP2}
    ${GFLAGS}
    ${THRIFTPROTOCOL}
    ${ZSTD}
    ${CRYPTO}
    ${AWS_SDK_CORE}
    ${DYNAMODB}
    ${JSONCPP}
    ${CURL}
    )

target_link_libraries(
    Client
    ${BOOST_SYSTEM}
    ${PTHREAD}
    ${GLOG}
    ${FOLLY}
    ${THRIFT}
    ${THRIFTCPP2}
    ${GFLAGS}
    ${THRIFTPROTOCOL}
    ${ZSTD}
    ${CRYPTO}
    ${SSL}
    ${BOOST_PYTHON}
    ${PYTHON27}
    )

install(TARGETS Server Client DESTINATION bin)
