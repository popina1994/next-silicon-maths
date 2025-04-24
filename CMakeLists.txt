cmake_minimum_required(VERSION 3.22)
project(Next-Silicon-Maths LANGUAGES CXX C)

# Set C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

find_package(BLAS REQUIRED)
find_package(MKL REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system thread iostreams program_options)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

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

if(DEFINED MEMORY_USAGE)
    message("")
else()
    set(MEMORY_USAGE "1")
endif()

add_library(${PROJECT_NAME}-c_lib STATIC ${C_SOURCES})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/code/main.cpp ${CPP_SOURCES})
add_executable(${PROJECT_NAME}-test ${CPP_SOURCES} ${C_SOURCES} ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME}-c_lib PRIVATE  code/header)
target_include_directories(${PROJECT_NAME} PRIVATE ${MKL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} code/header)
target_include_directories(${PROJECT_NAME}-test PRIVATE ${MKL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} code/header test/header)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-c_lib)
target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME}-c_lib)

target_link_libraries(${PROJECT_NAME} PRIVATE ${MKL_LIBRARIES} ${BLAS_LIBRARIES}
    ${Boost_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME}-test PRIVATE ${MKL_LIBRARIES} ${BLAS_LIBRARIES}
    ${Boost_LIBRARIES} gtest_main
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-test)