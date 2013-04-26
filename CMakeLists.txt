# deep125 project

cmake_minimum_required(VERSION 2.8.5)

project (DLOsmPbf2sqlite)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

#INCLUDE(PCHSupport)

INCLUDE(PCH_MSVC)

INCLUDE(PrecompiledHeader)

add_subdirectory(OsmPbf2sqlite)
