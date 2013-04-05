# cotire OsmPbf2sqlite project
set (PROJECT OsmPbf2sqlite)

#set(BOOST_ROOT /home/di/e/boost_1_53_0)

add_executable(${PROJECT} OsmPbf2sqlite.cpp CdbDictionary.cpp ConsoleOutput.cpp CtabKeyValue.cpp DB.cpp
SqLite3MemAloc.cpp
stdafx.cpp
ThreadLoader.cpp
CdbDictionary.h
ConsoleOutput.h
CtabKeyValue.h
DB.h
resource.h
SqLite3MemAloc.h
stdafx.h
targetver.h
ThreadLoader.h
DLIB/MyString.cpp
DLIB/MyString.h
DLIB/SQLite3DB.cpp
DLIB/SQLite3DB.h
DLIB/SQLite3Table.cpp
DLIB/SQLite3Table.h
DLIB/Stopwatch.cpp
DLIB/Stopwatch.h
DLIB/TAllocatorAddOnly.h
DLIB/ThreadManager.cpp
DLIB/ThreadManager.h
DLIB/TSlicedArray.h
OSMPBF/BlobHeader.cpp
OSMPBF/BlobHeader.h
OSMPBF/ChangeSet.h
OSMPBF/DenseNodes.h
OSMPBF/Headerblock.cpp
OSMPBF/Headerblock.h
OSMPBF/Info.h
OSMPBF/Node.h
OSMPBF/osmpbf.h
OSMPBF/PrimitiveBlock.h
OSMPBF/PrimitiveGroup.h
OSMPBF/Relation.h
OSMPBF/StringTable.h
OSMPBF/Way.h
PBFRO/PbfFBytes.cpp
PBFRO/PbfFBytes.h
PBFRO/PbfField.cpp
PBFRO/PbfField.h
PBFRO/PbfInputStream.cpp
PBFRO/PbfInputStream.h
PBFRO/PbfMessage.cpp
PBFRO/PbfMessage.h
PBFRO/Pbfro.h
PBFRO/svector.h
PBFRO/TFar.h
PBFRO/TFsint.h
PBFRO/TFuint.h
)

#для boost
set (BOOST_COMPONENTS
	thread
	filesystem)

set (Boost_USE_STATIC_LIBS ON)

set (Boost_USE_MULTITHREADED ON)

find_package (Threads)

find_package (Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries (${PROJECT} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})



if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "-Wall")
endif()

cotire(${PROJECT})

# cotire sets the following properties
get_target_property(_unitySource ${PROJECT} COTIRE_CXX_UNITY_SOURCE)
get_target_property(_prefixHeader ${PROJECT} COTIRE_CXX_PREFIX_HEADER)
get_target_property(_precompiledHeader ${PROJECT} COTIRE_CXX_PRECOMPILED_HEADER)
get_target_property(_unityTargetName ${PROJECT} COTIRE_UNITY_TARGET_NAME)

if (_unitySource)
	message(STATUS "${PROJECT} unity source: ${_unitySource}")
endif()
if (_prefixHeader)
	message(STATUS "${PROJECT} prefix header: ${_prefixHeader}")
endif()
if (_precompiledHeader)
	message(STATUS "${PROJECT} precompiled header: ${_precompiledHeader}")
endif()
if (TARGET ${_unityTargetName})
	message(STATUS "${PROJECT} unity target: ${_unityTargetName}")
endif()

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( ${PROJECT} ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

 target_link_libraries (${PROJECT}  sqlite3)

if (MSVC)
	target_link_libraries (${PROJECT}  Ws2_32.lib )
	add_definitions(-DUNICODE -D_UNICODE -D_CRT_SECURE_NO_WARNINGS)
endif()
