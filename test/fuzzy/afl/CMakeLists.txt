# subproject/CMakeLists.txt

# Fuzzy tests using AFL
set(CMAKE_C_COMPILER ${CMAKE_AFL_C_COMPILER})
set(CMAKE_CXX_COMPILER ${CMAKE_AFL_CXX_COMPILER})
message("Compiler is" ${CMAKE_CXX_COMPILER})

file(GLOB_RECURSE C_SOURCES
    ${NEXT_SILICON_ROOT_PATH}/code/src/*.c
)

file(GLOB_RECURSE CPP_SOURCES
    ${NEXT_SILICON_ROOT_PATH}/code/src/*.cpp
    ${NEXT_SILICON_ROOT_PATH}/code/src/*.cxx
)

add_executable(${PROJECT_NAME}-afl main_afl.cpp ${CPP_SOURCES})
set_target_properties(${PROJECT_NAME}-afl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_compile_options(${PROJECT_NAME}-afl PRIVATE -O1 -g)
target_compile_options(${PROJECT_NAME}-afl PRIVATE -fsanitize=address)
target_link_libraries(${PROJECT_NAME}-afl PRIVATE -fsanitize=address)

target_include_directories(${PROJECT_NAME}-afl PRIVATE  ${Boost_INCLUDE_DIRS} ${NEXT_SILICON_ROOT_PATH}/code/header)

target_link_directories(${PROJECT_NAME}-afl  PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}-afl PRIVATE ${PROJECT_NAME}-c_lib)