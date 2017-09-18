# CMAKE file for snaps folder
cmake_minimum_required(VERSION 3.0)

add_custom_target(Snap
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml)
