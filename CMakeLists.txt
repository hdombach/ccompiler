cmake_minimum_required(VERSION 3.0)
project(ccompiler)
set(CMAKE_VERBOSE_MAKEFILE on)

set(COMMON_DIRS src/ast src/sem src/util)

set(COMMON_SOURCES "src/preprocessor.c" "src/token.c" "src/tokenizer.c")

foreach(COMMON_DIR ${COMMON_DIRS})
	file(GLOB TMP_SOURCES "${COMMON_DIR}/*.c")

	list(APPEND COMMON_SOURCES ${TMP_SOURCES})
endforeach()

set(CCOMPILER_SOURCES src/main.c src/argParser.c ${COMMON_SOURCES})

add_executable(ccompiler ${CCOMPILER_SOURCES})
target_link_libraries(ccompiler m)

file(GLOB TEST_SOURCES "src/tests/*.c")
list(APPEND TEST_SOURCES ${COMMON_SOURCES})

add_executable(all_tests ${TEST_SOURCES})
target_link_libraries(all_tests m)
add_test(NAME all_tests COMMAND build/all_tests)
