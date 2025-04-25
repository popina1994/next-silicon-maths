cmake_minimum_required(VERSION 3.22)
project(Next-Silicon-Maths LANGUAGES CXX C)

# Set C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system thread iostreams program_options)

option(UNIT_TEST "Build Unit tests" ON)
option(AFL_BUILD "Build AFL" ON)
option(FUZZ_BUILD "Build libFuzz" ON)
set(NEXT_SILICON_ROOT_PATH ${PROJECT_SOURCE_DIR} CACHE STRING "A string passed to the subproject")
set(AFL_C_COMPILER ${AFL_C_COMPILER} CACHE STRING "A string passed to the subproject")
set(AFL_CXX_COMPILER ${AFL_CXX_COMPILER} CACHE STRING "A string passed to the subproject")


enable_testing()

file(GLOB_RECURSE CPP_SOURCES
    ${PROJECT_SOURCE_DIR}/code/src/*.cpp
    ${PROJECT_SOURCE_DIR}/code/src/*.cxx
)

file(GLOB_RECURSE C_SOURCES
    ${PROJECT_SOURCE_DIR}/code/src/*.c
)

add_library(${PROJECT_NAME}-c_lib STATIC ${C_SOURCES})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/code/main.cpp ${CPP_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -O3)

target_include_directories(${PROJECT_NAME}-c_lib PRIVATE  code/header)
target_include_directories(${PROJECT_NAME} PRIVATE  ${Boost_INCLUDE_DIRS} code/header)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-c_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Boost_LIBRARIES}
)

if (UNIT_TEST)
    file(GLOB_RECURSE TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/unit/src/*.cpp
    ${PROJECT_SOURCE_DIR}/test/unit/src/*.cxx
    )
    add_executable(${PROJECT_NAME}-test ${CPP_SOURCES} ${TEST_SOURCES})
    target_link_libraries(${PROJECT_NAME}-test PRIVATE
        ${Boost_LIBRARIES} gtest_main
    )
    target_compile_options(${PROJECT_NAME}-test PRIVATE -O1 -g)
    target_include_directories(${PROJECT_NAME}-test PRIVATE  ${Boost_INCLUDE_DIRS} code/header test/unit/header)
    target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME}-c_lib)

    # Unit tests
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(googletest)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}-test)
endif()

if (FUZZ_BUILD)
    add_executable(${PROJECT_NAME}-fuzz ${PROJECT_SOURCE_DIR}/test/fuzzy/main_fuzzy.cpp ${CPP_SOURCES})
    target_compile_options(${PROJECT_NAME}-fuzz PRIVATE -O1 -g)
    target_compile_options(${PROJECT_NAME}-fuzz PRIVATE -fsanitize=fuzzer -fsanitize=address)
    target_link_libraries(${PROJECT_NAME}-fuzz PRIVATE -fsanitize=fuzzer -fsanitize=address)

    target_include_directories(${PROJECT_NAME}-fuzz PRIVATE  ${Boost_INCLUDE_DIRS} code/header)
    target_link_libraries(${PROJECT_NAME}-fuzz PRIVATE ${PROJECT_NAME}-c_lib)
endif()


if (AFL_BUILD)
    add_subdirectory(test/fuzzy/afl)
endif()