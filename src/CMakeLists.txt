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
    TestClient
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
    )

install(TARGETS Server TestClient DESTINATION bin)
