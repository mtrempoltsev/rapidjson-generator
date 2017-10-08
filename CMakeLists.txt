cmake_minimum_required(VERSION 3.4)

project(rapidjson-generator)

set(CMAKE_CXX_STANDARD 14)

get_filename_component(RAPIDJSON_GENERATOR_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

option(RAPIDJSON_GENERATOR_BUILD_RAPIDJSON "Builds the rapidjson-1.1.0" ON)
option(RAPIDJSON_GENERATOR_BUILD_GTEST     "Builds the gtest-1.8.0"     ON)
option(RAPIDJSON_GENERATOR_BUILD_TESTS     "Builds the tests"           ON)

set(3RD_PARTY_DIR ${RAPIDJSON_GENERATOR_ROOT}/3rd-party)

if(RAPIDJSON_GENERATOR_BUILD_RAPIDJSON)
        set(RAPIDJSON_BUILD_DOC                 OFF CACHE BOOL "Build rapidjson documentation." FORCE)
        set(RAPIDJSON_BUILD_EXAMPLES            OFF CACHE BOOL "Build rapidjson examples." FORCE)
        set(RAPIDJSON_BUILD_TESTS               OFF CACHE BOOL "Build rapidjson perftests and unittests." FORCE)
        set(RAPIDJSON_BUILD_THIRDPARTY_GTEST    OFF CACHE BOOL "Use gtest installation in `thirdparty/gtest` by default if available" FORCE)
        set(RAPIDJSON_BUILD_CXX11               ON  CACHE BOOL "Build rapidjson with C++11 (gcc/clang)" FORCE)
        set(RAPIDJSON_BUILD_ASAN                OFF CACHE BOOL "Build rapidjson with address sanitizer (gcc/clang)" FORCE)
        set(RAPIDJSON_BUILD_UBSAN               OFF CACHE BOOL "Build rapidjson with undefined behavior sanitizer (gcc/clang)" FORCE)
        set(RAPIDJSON_HAS_STDSTRING             ON  CACHE BOOL "Use std::string" FORCE)

        set(RAPIDJSON_GENERATOR_RAPIDJSON_DIR "${3RD_PARTY_DIR}/rapidjson")
        add_subdirectory("${RAPIDJSON_GENERATOR_RAPIDJSON_DIR}")
        include_directories("${RAPIDJSON_GENERATOR_RAPIDJSON_DIR}/include")
endif()

if(RAPIDJSON_GENERATOR_BUILD_GTEST)
    set(BUILD_GTEST                            ON  CACHE BOOL "Build gtest")
    set(BUILD_GMOCK                            OFF CACHE BOOL "Build gmock")
    set(gtest_force_shared_crt                 ON  CACHE BOOL "" FORCE)

    set(RAPIDJSON_GENERATOR_GTEST_DIR "${3RD_PARTY_DIR}/googletest")
    add_subdirectory(${RAPIDJSON_GENERATOR_GTEST_DIR})
    include_directories(${RAPIDJSON_GENERATOR_GTEST_DIR}/googletest/include)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RAPIDJSON_GENERATOR_ROOT}/bin)

set(HEADERS
    ${RAPIDJSON_GENERATOR_ROOT}/generator.h
    ${RAPIDJSON_GENERATOR_ROOT}/item.h
    ${RAPIDJSON_GENERATOR_ROOT}/parser.h)

set (SHARED_SOURCES
    ${RAPIDJSON_GENERATOR_ROOT}/generator.cpp
    ${RAPIDJSON_GENERATOR_ROOT}/item.cpp
    ${RAPIDJSON_GENERATOR_ROOT}/parser.cpp)

set(SOURCES
    ${SHARED_SOURCES}
    ${RAPIDJSON_GENERATOR_ROOT}/main.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

if(RAPIDJSON_GENERATOR_BUILD_TESTS)
    set(RAPIDJSON_GENERATOR_TEST_PROJECT test-${PROJECT_NAME})
    add_test(NAME ${RAPIDJSON_GENERATOR_TEST_PROJECT} COMMAND ${RAPIDJSON_GENERATOR_TEST_PROJECT})

    add_subdirectory(tests)

    enable_testing()
endif()
