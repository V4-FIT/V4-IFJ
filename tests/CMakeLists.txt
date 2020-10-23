FILE(GLOB_RECURSE SOURCES "*.c")
FILE(GLOB_RECURSE IFJ20_SRC ${CMAKE_SOURCE_DIR}/src/*.c)
list(REMOVE_ITEM IFJ20_SRC ${CMAKE_SOURCE_DIR}/src/main.c)

include_directories(../criterion/include/)
include_directories(../src/)

add_executable(tests ${SOURCES} ${IFJ20_SRC})

if(MSVC)
	target_link_libraries(tests ${CMAKE_SOURCE_DIR}/criterion/lib/criterion.lib)
	add_custom_command(TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/criterion/bin/criterion.dll"
        $<TARGET_FILE_DIR:tests>)
else()
	target_link_libraries(tests ${CMAKE_SOURCE_DIR}/criterion/lib/libcriterion.so.3)
endif()

add_test(NAME Unit_Tests COMMAND tests)
