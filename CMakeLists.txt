cmake_minimum_required(VERSION 3.3)
project(calculator3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -pedantic-errors")

set(SOURCE_FILES
        main.c
        common.c common.h
        tree.c tree.h
        parse.c parse.h
        calculate.c calculate.h
        hashtable.c hashtable.h
        SPList.c SPList.h
        SPListElement.c SPListElement.h
        graph.c graph.h
        test.c)

add_executable(calculator3 ${SOURCE_FILES})