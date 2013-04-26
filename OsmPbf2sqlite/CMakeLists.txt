cmake_minimum_required(VERSION 2.8)

#set(Boost_DEBUG ON)

set (PROJECT OsmPbf2sqlite)

####################################
#user specified build options
####################################
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ELSE(NOT CMAKE_BUILD_TYPE)
    MESSAGE( "Compiling as ${CMAKE_BUILD_TYPE}" )
ENDIF(NOT CMAKE_BUILD_TYPE)

PROJECT(${PROJECT})

SET(${PROJECT}_VERSION 0.1)
SET(PROJECT_NAME ${PROJECT} )
SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)

#кофигурация sqlite3 берётся из файла config.h
add_definitions(-D_HAVE_SQLITE_CONFIG_H)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


SET(C_SRC sqlite3.c)
#stdafx.cpp

SET(CPP_SRC
	CdbDictionary.cpp
	ConsoleOutput.cpp
	CtabKeyValue.cpp
	DB.cpp
	OsmPbf2sqlite.cpp
	SqLite3MemAloc.cpp
	ThreadLoader.cpp
	DLIB/MyString.cpp
	DLIB/SQLite3DB.cpp
	DLIB/SQLite3Table.cpp
	DLIB/ThreadManager.cpp
	OSMPBF/BlobHeader.cpp
	OSMPBF/Headerblock.cpp
	PBFRO/PbfFBytes.cpp
	PBFRO/PbfField.cpp
	PBFRO/PbfInputStream.cpp
	PBFRO/PbfMessage.cpp
)

SET(HPP_SRC
	CdbDictionary.h
	config.h
	ConsoleOutput.h
	CtabKeyValue.h
	CTabNode_Cash.h
	CTabNodeInfo_Cash.h
	CTabRelation_Cash.h
	CTabWay_Cash.h
	DB.cpp
	sqlite3.h
	SqLite3MemAloc.h
	stdafx.h
	targetver.h
	ThreadLoader.h
	TTable_cash.h
	DLIB/MyString.h
	DLIB/SQLite3DB.h
	DLIB/SQLite3Table.h
	DLIB/stdafx.h
	DLIB/TAllocatorAddOnly.h
	DLIB/ThreadManager.h
	DLIB/TSlicedArray.h
	OSMPBF/BlobHeader.h
	OSMPBF/ChangeSet.h
	OSMPBF/DenseNodes.h
	OSMPBF/Headerblock.h
	OSMPBF/Info.h
	OSMPBF/Node.h
	OSMPBF/osmpbf.h
	OSMPBF/PrimitiveBlock.h
	OSMPBF/PrimitiveGroup.h
	OSMPBF/Relation.h
	OSMPBF/stdafx.h
	OSMPBF/StringTable.h
	OSMPBF/Way.h
	PBFRO/PbfFBytes.h
	PBFRO/PbfField.h
	PBFRO/PbfInputStream.h
	PBFRO/PbfMessage.h
	PBFRO/Pbfro.h
	PBFRO/svector.h
	PBFRO/TFar.h
	PBFRO/TFsint.h
	PBFRO/TFuint.h
)


ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" CPP_SRC )
add_executable(${PROJECT} ${HPP_SRC} ${CPP_SRC} ${C_SRC})

#add_precompiled_header2(${PROJECT} stdafx.h )

#для boost
set (BOOST_COMPONENTS
    system
	thread
	filesystem
	timer
	chrono
	date_time
	)


set (Boost_USE_STATIC_LIBS ON)
set (Boo st_USE_MULTITHREADED ON)



#target_link_libraries (${PROJECT}  sqlite3)

# set (BOOST_ADDITIONAL_VERSIONS 1.53)
# set (BOOST_COMPILER_VERSION -vc90)
# set (BOOST_COMPILER_VERSION -vc90)
# set(_boost_COMPILER "-vc90")

# Guesses Boost's compiler prefix used in built library names
# Returns the guess by setting the variable pointed to by _ret


find_package (Boost 1.53 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	target_link_libraries (${PROJECT} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
endif(Boost_FOUND)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( ${PROJECT} ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

find_package (Threads)

if(WIN32)
	target_link_libraries (${PROJECT}  Ws2_32)
else()
    MESSAGE(STATUS "ispolzuem rt")
	target_link_libraries (${PROJECT}  rt)
endif()

#MESSAGE(STATUS "cdir=${CMAKE_SOURCE_DIR}")
#ADD_PRECOMPILED_HEADER(${PROJECT} "${CMAKE_SOURCE_DIR}/stdafx.h" 1)

