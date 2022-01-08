cmake_minimum_required(VERSION 3.16)
project(books)

set(HEADERS
        ./include/Books.hpp
        )

set(SOURCES
        ./src/Books.cpp
        )

add_executable(books
        ./app/main.cpp
        ${HEADERS}
        ${SOURCES}
        )