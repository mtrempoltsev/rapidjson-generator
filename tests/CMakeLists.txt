project(${RAPIDJSON_GENERATOR_TEST_PROJECT})

set(TEST_SOURCES
    test_generator.cpp
    test_json.cpp
    test_parser.cpp
    main.cpp)

add_executable(${RAPIDJSON_GENERATOR_TEST_PROJECT}
    ${SHARED_SOURCES}
    ${TEST_SOURCES})

target_link_libraries(${RAPIDJSON_GENERATOR_TEST_PROJECT}
    gtest)
