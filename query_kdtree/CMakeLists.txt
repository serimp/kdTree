cmake_minimum_required (VERSION 2.6)
project (query_kdtree)

# set (KdTree_VERSION_MAJOR 1)
# set (KdTree_VERSION_MINOR 0)


# configure_file (
# "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
# "${PROJECT_BINARY_DIR}/TutorialConfig.h"
# )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h

include_directories(../include)
add_executable(query_kdtree ${CMAKE_SOURCE_DIR}/query_kdtree/query_kdtree.cpp)

