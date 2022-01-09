set(NAME test.${PROJECT_NAME})

enable_testing()


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set(NAME test.${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

set(TEST_HEADERS

        )

set(TEST_SOURCES
        main.cpp
        MongoDB.Test.cpp
        )

add_executable(${NAME}
        ${TEST_SOURCES}
        ${TEST_HEADERS}
        )

target_link_libraries(${NAME}
        gtest
        pthread
        ${PROJECT_NAME}lib
        pthread
        ${CURL_LIBRARY}
        ${PROTOBUF_LIBRARIES}
        Drogon::Drogon
        mongo::bsoncxx_shared
        mongo::mongocxx_shared
        )
