# CMakeLists file for subdir: /home/amartya/Projects/PersonalSite/thrift
cmake_minimum_required(VERSION 3.0)
add_compile_options(-fPIC)

execute_process(COMMAND "python" "-m" "thrift_compiler.main" "--gen" "cpp2" ${SERVICE_FILE} "-o" "${GEN_CPP_2_OUT}")

file(GLOB GENERATED "${GEN_CPP_2}/*.cpp")
add_library(
    GeneratedObjects
    OBJECT
    ${GENERATED})
