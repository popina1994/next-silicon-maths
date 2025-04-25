cmake_minimum_required(VERSION 3.22)
project(Next-Silicon-Maths LANGUAGES CXX C)

# Set C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system thread iostreams program_options)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

enable_testing()

file(GLOB_RECURSE CPP_SOURCES
    ${PROJECT_SOURCE_DIR}/code/src/*.cpp
    ${PROJECT_SOURCE_DIR}/code/src/*.cxx
)

file(GLOB_RECURSE C_SOURCES
    ${PROJECT_SOURCE_DIR}/code/src/*.c
)

file(GLOB_RECURSE TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/src/*.cpp
    ${PROJECT_SOURCE_DIR}/test/src/*.cxx
)

add_library(${PROJECT_NAME}-c_lib STATIC ${C_SOURCES})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/code/main.cpp ${CPP_SOURCES})
add_executable(${PROJECT_NAME}-test ${CPP_SOURCES} ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME}-c_lib PRIVATE  code/header)
target_include_directories(${PROJECT_NAME} PRIVATE  ${Boost_INCLUDE_DIRS} code/header)
target_include_directories(${PROJECT_NAME}-test PRIVATE  ${Boost_INCLUDE_DIRS} code/header test/header)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-c_lib)
target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME}-c_lib)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Boost_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME}-test PRIVATE
    ${Boost_LIBRARIES} gtest_main
)

# Unit tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-test)

# Fuzzy tests
set(CMAKE_C_COMPILER ${CMAKE_FUZZY_C_COMPILER})
set(CMAKE_CXX_COMPILER ${CMAKE_FUZZY_CXX_COMPILER})
message("Compiler is" ${CMAKE_CXX_COMPILER})

add_executable(${PROJECT_NAME}-afl ${PROJECT_SOURCE_DIR}/test/main_fuzzy.cpp ${CPP_SOURCES})
target_compile_options(${PROJECT_NAME}-afl PRIVATE -O1 -g)
target_compile_options(${PROJECT_NAME}-afl PRIVATE -fsanitize=address)
target_link_libraries(${PROJECT_NAME}-afl PRIVATE -fsanitize=address)


target_include_directories(${PROJECT_NAME}-afl PRIVATE  ${Boost_INCLUDE_DIRS} code/header)

target_link_libraries(${PROJECT_NAME}-afl PRIVATE ${PROJECT_NAME}-c_lib)

set(AFL_OUTPUT_DIR ${CMAKE_BINARY_DIR}/out)

# Ensure that the output directory is created
file(MAKE_DIRECTORY ${AFL_OUTPUT_DIR})