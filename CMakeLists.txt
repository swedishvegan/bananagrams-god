cmake_minimum_required(VERSION 3.7...3.27)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(bananagrams-god)

message("Compiler being used: ${CMAKE_CXX_COMPILER_ID}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "/EHsc")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "-O3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "-O3")
endif()

add_executable(bananagrams-god Source.cpp)

file(GLOB_RECURSE BANANAGRAMS_SOURCES
	*.hpp
	util/*.hpp
	util/*.cpp
)

target_sources(bananagrams-god PRIVATE ${BANANAGRAMS_SOURCES})
