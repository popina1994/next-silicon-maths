cmake_minimum_required(VERSION 3.22)
project(Next-Silicon-Maths LANGUAGES CXX C)

# Set C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wuninitialized -Wmissing-declarations -Wsign-compare)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system thread iostreams program_options)

option(UNIT_TEST "Build Unit tests" ON)
option(AFL_BUILD "Build AFL" OFF)
option(FUZZ_BUILD "Build libFuzz" ON)
set(NEXT_SILICON_ROOT_PATH ${PROJECT_SOURCE_DIR} CACHE STRING "A string passed to the subproject")
set(AFL_C_COMPILER ${AFL_C_COMPILER} CACHE STRING "A string passed to the subproject")
set(AFL_CXX_COMPILER ${AFL_CXX_COMPILER} CACHE STRING "A string passed to the subproject")


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
    enable_testing()
    file(GLOB_RECURSE TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/test/unit/src/*.cpp
        ${PROJECT_SOURCE_DIR}/test/unit/src/*.cxx)

    set(UNIT_TEST_BINARY ${PROJECT_NAME}-test)
    add_executable(${UNIT_TEST_BINARY} ${CPP_SOURCES} ${TEST_SOURCES})
    target_link_libraries(${UNIT_TEST_BINARY} PRIVATE
        ${Boost_LIBRARIES} gtest_main
    )
    target_compile_options(${UNIT_TEST_BINARY} PRIVATE -O1 -g)
    target_include_directories(${UNIT_TEST_BINARY} PRIVATE  ${Boost_INCLUDE_DIRS} code/header test/unit/header)
    target_link_libraries(${UNIT_TEST_BINARY} PRIVATE ${PROJECT_NAME}-c_lib)

    # Unit tests
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(googletest)

    include(GoogleTest)
    gtest_discover_tests(${UNIT_TEST_BINARY})
endif()

if (FUZZ_BUILD)
    file(GLOB FUZZY_CPP_TESTS
        ${PROJECT_SOURCE_DIR}/test/fuzzy/lib_fuzz/*.cpp
        ${PROJECT_SOURCE_DIR}/test/fuzzy/lib_fuzz/*.cxx
    )
    foreach(FUZZY_CPP_TEST IN LISTS FUZZY_CPP_TESTS)
        get_filename_component(FUFY_EXEC_NAME_FILE ${FUZZY_CPP_TEST} NAME_WE)
        set(FUZZY_EXEC_NAME ${PROJECT_NAME}-fuzz-${FUFY_EXEC_NAME_FILE})
        message("FUZZY" ${FUZZY_CPP_TEST} " DISTANCE "${FUZZY_EXEC_NAME})

        add_executable(${FUZZY_EXEC_NAME} ${FUZZY_CPP_TEST} ${CPP_SOURCES})
        target_compile_options(${FUZZY_EXEC_NAME} PRIVATE -O1 -g)
        target_compile_options(${FUZZY_EXEC_NAME} PRIVATE -fsanitize=fuzzer -fsanitize=address)
        target_link_libraries(${FUZZY_EXEC_NAME} PRIVATE -fsanitize=fuzzer -fsanitize=address)

        target_include_directories(${FUZZY_EXEC_NAME} PRIVATE  ${Boost_INCLUDE_DIRS} code/header)
        target_link_libraries(${FUZZY_EXEC_NAME} PRIVATE ${PROJECT_NAME}-c_lib)
    endforeach()
endif()


if (AFL_BUILD)
    add_subdirectory(test/fuzzy/afl)
endif()