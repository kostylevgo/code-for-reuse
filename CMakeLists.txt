cmake_minimum_required(VERSION 3.24)
project(code-for-reuse)

set(CMAKE_CXX_STANDARD 23)

include_directories(lib)

add_compile_definitions(LOCAL)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -fsanitize=address,undefined")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
endif()

file(TOUCH ./in.txt ./out.txt)

add_executable(main main.cpp)

file(GLOB_RECURSE sources examples/*.cpp)

foreach(src ${sources})
    file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR}/examples ${src})

    string(REGEX REPLACE "\\.cpp$" "" rel_path ${rel_path})

    string(MAKE_C_IDENTIFIER ${rel_path} name)
    string(REPLACE "__" "_" name ${name})

    add_executable("examples_${name}" ${src})
endforeach()