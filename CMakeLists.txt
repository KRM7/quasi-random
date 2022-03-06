cmake_minimum_required(VERSION 3.20)

project(quasirandom LANGUAGES CXX)

include(CTest)

add_executable(qtest ${CMAKE_CURRENT_SOURCE_DIR}/test/quasirand.cpp)

set(WFLAGS -Wall -Wextra -pedantic)
set(OFLAGS -O2 -flto)

target_compile_features(qtest PRIVATE cxx_std_20)
target_compile_options(qtest PRIVATE ${WFLAGS} ${OFLAGS})

find_package(Catch2 REQUIRED)
target_link_libraries(qtest PRIVATE Catch2::Catch2WithMain)

add_test(NAME qtest COMMAND qtest)