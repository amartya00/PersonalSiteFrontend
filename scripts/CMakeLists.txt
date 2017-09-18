# CMake files for scripts
cmake_minimum_required(VERSION 3.0)

project(PersonalSite_Frontend)

add_custom_target(
  Scripts
  SOURCES
  "${SCRIPTS}/frontend-server"
  )
