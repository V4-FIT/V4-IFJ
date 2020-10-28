cmake_minimum_required(VERSION 3.10)
project(IFJ)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option (ENABLE_TESTS "Build tests along with IFJ20" ON)
if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory (tests)
endif ()

add_subdirectory(src)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ifj20)
else()
    set(CMAKE_C_FLAGS_DEBUG "-g -Wall -pedantic")
    set(CMAKE_C_FLAGS_RELEASE "-O2")
endif()
