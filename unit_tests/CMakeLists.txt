cmake_minimum_required(VERSION 3.5)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Set paths
set(RELATIVE_ROOT_DIR "..")
set(RELATIVE_PRJ_DIR ".")
set(RELATIVE_TESTED_PRJ_DIR "../src")
get_filename_component(ROOT_DIR ${RELATIVE_ROOT_DIR}  ABSOLUTE)
get_filename_component(PRJ_DIR ${RELATIVE_PRJ_DIR}  ABSOLUTE)
get_filename_component(TESTED_PRJ_DIR ${RELATIVE_TESTED_PRJ_DIR}  ABSOLUTE)

# Google test lib
set (GOOGLE_TEST_DIR ${PRJ_DIR}/googletest)
include_directories(${GOOGLE_TEST_DIR})

add_definitions(-DGTEST_LANGUAGE_CXX11)
add_definitions(-DGTEST_HAS_PTHREAD=1)

set(GTEST_SRC ${GOOGLE_TEST_DIR}/googletest)
set(GMOCK_SRC ${GOOGLE_TEST_DIR}/googlemock)
set(GTEST_FILES ${GTEST_SRC}/src/gtest-all.cc)
set(GTEST_INC_FILES ${GTEST_SRC} ${GTEST_SRC}/include)
list(APPEND GTEST_FILES ${GMOCK_SRC}/src/gmock-all.cc)
list(APPEND GTEST_INC_FILES ${GMOCK_SRC} ${GMOCK_SRC}/include)
include_directories(${GTEST_INC_FILES})
set (GTEST_LIB google_test)
add_library(${GTEST_LIB} STATIC ${GTEST_FILES})

# Includes
include_directories(${PRJ_DIR})
include_directories(${TESTED_PRJ_DIR})

# Compilator
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# GCov flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -fprofile-arcs -ftest-coverage --coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -g -Wall -fprofile-arcs -ftest-coverage --coverage -fno-inline -fno-inline-small-functions -fno-default-inline")

# Project
project(tkm32f499_unit_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test case files
set(SRC_DIR "${PRJ_DIR}/src")
file (GLOB_RECURSE H_FILES ${SRC_DIR}/*.h )
file (GLOB_RECURSE C_FILES ${SRC_DIR}/*.c)
file (GLOB_RECURSE CPP_FILES ${SRC_DIR}/*.cc )

# Testing files
set (TESTING_FILES
    ${TESTED_PRJ_DIR}/timer/soft_timer.h
    ${TESTED_PRJ_DIR}/timer/soft_timer.cc
)

set (SOURCE_FILES ${C_FILES} ${H_FILES} ${CPP_FILES}  ${TESTING_FILES})
add_executable(${PROJECT_NAME} ${SOURCE_FILES}  )

#target_link_libraries(${PROJECT_NAME} ${GTEST_LIB_LIST} )
target_link_libraries(${PROJECT_NAME}   ${GTEST_LIB} pthread)

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

