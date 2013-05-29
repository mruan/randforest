cmake_minimum_required (VERSION 2.6)
project (randomforest)

#add_subdirectory(sherwood)

# This program can only be compiled with g++ v4.6 and above
# to remove any dependencies on boost libraries
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# set where to put the binary
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
  include/sherwood_c0x
  include/shotton
)

#link_directories()

add_subdirectory(trial)