cmake_minimum_required(VERSION 3.16)
project(books)

if (NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif ()

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(DEPENDENCIES
        CURL
        Protobuf
        Drogon
        mongocxx
        bsoncxx
        )


foreach (DEPENDENCE IN LISTS DEPENDENCIES)
    find_package(${DEPENDENCE} REQUIRED)
    if (${DEPENDENCE}_FOUND)
        message("-- " ${DEPENDENCE} " is FOUND")
    endif ()
endforeach ()


set(PROTOS
        proto/data.proto
        )

set(HEADERS
        include/util.hpp
        include/MongoDB.hpp
        include/Singleton.hpp
        include/Controller.hpp
        include/Demo.hpp
        )

set(SOURCES
        src/MongoDB.cpp
        src/Controller.cpp
        src/Demo.cpp
        src/util.cpp
        )

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})

add_library(${PROJECT_NAME}lib
        ${HEADERS}
        ${SOURCES}
        ${PROTO_HDRS}
        ${PROTO_SRCS}
        )

add_executable(${PROJECT_NAME}
        ./app/main.cpp
        )

target_link_libraries(${PROJECT_NAME}
        ${PROJECT_NAME}lib
        )

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CURL_INCLUDE_DIR}
        ${PROTOBUF_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${LIBMONGOCXX_INCLUDE_DIR}
        ${LIBBSONCXX_INCLUDE_DIR}
        /usr/local/include/mongocxx/v_noabi
        /usr/local/include/bsoncxx/v_noabi
        /usr/local/include/libmongoc-1.0
        /usr/local/include/libbson-1.0
)

target_link_libraries(${PROJECT_NAME}
        pthread
        ${CURL_LIBRARY}
        ${PROTOBUF_LIBRARIES}
        Drogon::Drogon
        mongo::bsoncxx_shared
        mongo::mongocxx_shared
        )

add_subdirectory(test)
