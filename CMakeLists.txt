cmake_minimum_required(VERSION 2.8.3)
project(sbpl_utils)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(ROS_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++0x")

find_package(catkin REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)

catkin_package(
    DEPENDS
    CATKIN_DEPENDS
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}
)

# common commands for building c++ executables and libraries
add_library(${PROJECT_NAME} src/hash_manager/hash_manager.cpp)

catkin_add_gtest(hash_manager_test tests/hash_manager_test.cpp examples/hashable_states.cpp)
target_link_libraries(hash_manager_test ${PROJECT_NAME})
