cmake_minimum_required(VERSION 3.9)
project(ccompiler)

# global settings
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

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


# Doxygen stuffs
find_package(Doxygen)
if (DOXYGEN_FOUND)

	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	set(DOXYGEN_HTML_OUTPUT "docs")
	set(DOXYGEN_HTML_EXTRA_STYLESHEET "doxygen-awesome-css/doxygen-awesome.css")
	set(DOXYGEN_HTML_COLORSTYLE "LIGHT")
	set(DOXYGEN_EXCLUDE_PATTERNS ${DOXYGEN_EXCLUDE_PATTERNS} "*doxygen-awesome-css*")

	doxygen_add_docs(docs ALL
		COMMENT "Generating docs with Doxygen")
else()
	message(STATUS "Cannot load doxygen")
endif()
