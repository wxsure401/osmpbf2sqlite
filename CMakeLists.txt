# cotire example project

cmake_minimum_required(VERSION 2.8.5)

project (CotireOsmPbf2sqlite)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include(cotire)

add_subdirectory(OsmPbf2sqlite)
