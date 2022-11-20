cmake_minimum_required(VERSION 3.16)

project(quasirandom LANGUAGES CXX)

include(CTest)


if(NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release)$")
    message("Unrecognized build type: ${CMAKE_BUILD_TYPE}. Defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
    set(WARN_FLAGS "-permissive- -W4 -w44263 -w44265 -w44388 -w44471")
    set(CMAKE_CXX_FLAGS "-std:c++20 -Zc:rvalueCast ${WARN_FLAGS} -TP -MP -MD -EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -Z7 -RTC1 -Od")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O2 -Oi -Gy -Qpar")
else()
    set(WARN_FLAGS "-Wall -Wextra -Wpedantic -Wshadow -Wnull-dereference -fpermissive")
    set(CMAKE_CXX_FLAGS "-std=c++20 ${WARN_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-Og")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
endif()


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.hpp")

add_library(quasirandom INTERFACE ${SOURCES} ${HEADERS})


file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/test/*.cpp")

add_executable(unit_tests ${TEST_SOURCES})
target_include_directories(unit_tests PRIVATE "${CMAKE_SOURCE_DIR}/src")

find_package(Catch2 REQUIRED)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain quasirandom)

add_test(NAME unit_tests COMMAND unit_tests)