# CMakeLists file for subdir: /home/amartya/Projects/PersonalSite/thrift
cmake_minimum_required(VERSION 3.0)

add_custom_target(BuildServiceDefs
  SOURCES ${SERVICE_FILE}
  COMMAND "python" "-m" "thrift_compiler.main" "--gen" "cpp2" ${SERVICE_FILE} "-o" "${GEN_CPP_2_OUT}"
  )
