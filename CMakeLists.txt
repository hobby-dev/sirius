cmake_minimum_required(VERSION 3.5)
project(saber VERSION 0.1.0 LANGUAGES CXX)

# set build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_COMPILER "clang++")
# set clang options
set(CMAKE_CXX_FLAGS         "-fstack-protector-all -Wall -Wpedantic -Wextra -Werror" )
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -ggdb" )
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -ggdb" )
set(CMAKE_CXX_FLAGS_RELEASE "-Os" )

# add sources
add_subdirectory(src bin)

