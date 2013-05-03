cmake_minimum_required (VERSION 2.6)
project (randomforest)

add_subdirectory(sherwood)

include_directories(
  external/
)

#link_directories()

add_subdirectory(src)

