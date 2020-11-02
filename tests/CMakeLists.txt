# V4-IFJ/tests/CMakeLists.txt
FILE(GLOB_RECURSE SOURCES "*.cpp")
FILE(GLOB_RECURSE IFJ20_SRC ${CMAKE_SOURCE_DIR}/src/*.c)
list(REMOVE_ITEM IFJ20_SRC ${CMAKE_SOURCE_DIR}/src/main.c)

find_package(GTest REQUIRED)

include_directories(
	${GTEST_INCLUDE_DIRS}
	../src
	../src/container
)

add_executable(tests ${SOURCES} ${IFJ20_SRC})
target_link_libraries(tests ${GTEST_BOTH_LIBRARIES})

add_test(NAME Unit_Tests COMMAND tests)

set_property(TARGET tests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
