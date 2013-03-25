# cotire OsmPbf2sqlite project

add_executable(OsmPbf2sqlite OsmPbf2sqlite.cpp 
CdbDictionary.cpp
ConsoleOutput.cpp
CtabKeyValue.cpp
DB.cpp
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

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set_target_properties(OsmPbf2sqlite PROPERTIES COMPILE_FLAGS "-Wall")
endif()

cotire(OsmPbf2sqlite)

# cotire sets the following properties
get_target_property(_unitySource OsmPbf2sqlite COTIRE_CXX_UNITY_SOURCE)
get_target_property(_prefixHeader OsmPbf2sqlite COTIRE_CXX_PREFIX_HEADER)
get_target_property(_precompiledHeader OsmPbf2sqlite COTIRE_CXX_PRECOMPILED_HEADER)
get_target_property(_unityTargetName OsmPbf2sqlite COTIRE_UNITY_TARGET_NAME)

if (_unitySource)
	message(STATUS "OsmPbf2sqlite unity source: ${_unitySource}")
endif()
if (_prefixHeader)
	message(STATUS "OsmPbf2sqlite prefix header: ${_prefixHeader}")
endif()
if (_precompiledHeader)
	message(STATUS "OsmPbf2sqlite precompiled header: ${_precompiledHeader}")
endif()
if (TARGET ${_unityTargetName})
	message(STATUS "OsmPbf2sqlite unity target: ${_unityTargetName}")
endif()

 target_link_libraries (OsmPbf2sqlite  sqlite3.lib zlib.lib)

 if (MSVC)
	target_link_libraries (OsmPbf2sqlite  Ws2_32.lib )
	add_definitions(-DUNICODE -D_UNICODE -D_CRT_SECURE_NO_WARNINGS)
endif()
