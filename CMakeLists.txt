# deep125 project

# Uncomment and change if non-system compiler is needed
#SET(CMAKE_C_COMPILER /gpfs/soft/gcc/4.7.2/bin/gcc)
#SET(CMAKE_CXX_COMPILER /gpfs/soft/gcc/4.7.2/bin/g++)

# Uncomment and change if non-statndard boost build is used 
#set(Boost_DEBUG 1)
#set(BOOST_ROOT /gpfs/soft/boost)
#set(Boost_NO_SYSTEM_PATHS ON)
#set(Boost_COMPILER -gcc47)

cmake_minimum_required(VERSION 2.8.5)

project (DLOsmPbf2sqlite)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

#INCLUDE(PCHSupport)

INCLUDE(PCH_MSVC)

INCLUDE(PrecompiledHeader)

add_subdirectory(OsmPbf2sqlite)
