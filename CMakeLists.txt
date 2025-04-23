cmake_minimum_required(VERSION 3.22)
project(Next-Silicon-Maths LANGUAGES CXX C)

# Set C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

find_package(BLAS REQUIRED)
find_package(MKL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB_RECURSE CPP_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.cxx
)

file(GLOB_RECURSE C_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.c
)
if(DEFINED MEMORY_USAGE)
    message("")
else()
    set(MEMORY_USAGE "1")
endif()

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${C_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${MKL_INCLUDE_DIRS} header)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system thread iostreams program_options)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${MKL_LIBRARIES} ${BLAS_LIBRARIES}
    ${Boost_LIBRARIES}
)