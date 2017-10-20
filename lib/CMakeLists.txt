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

link_directories(${LINK_FOLDER})

file(GLOB GENERATED "${GEN_CPP_2}/*.cpp")

add_library(ServerLib SHARED
    "Service.cc"

    "utils/Utils.cc"

    "exceptions/Exceptions.cc"

    "datastructures/UserItem.cc"

    $<TARGET_OBJECTS:GeneratedObjects>
    )



target_link_libraries(
    ServerLib
    ${AUTH}
    ${GAPI}
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
    ${CURL})

install(TARGETS ServerLib DESTINATION lib)
