cmake_minimum_required(VERSION 2.8.3)
project(sbpl_utils)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(ROS_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(catkin REQUIRED COMPONENTS sbpl)
# find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS graph)

include_directories(${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS})

catkin_package(
    DEPENDS
      # OpenCV 
    CATKIN_DEPENDS sbpl
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}
)

# common commands for building c++ executables and libraries
add_library(${PROJECT_NAME} 
            src/hash_manager/hash_manager.cpp
            src/environments/boost_graph_environment.cpp
            src/visualization/grid_visualizer.cpp)
          target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES}
            ${catkin_LIBRARIES})

catkin_add_gtest(hash_manager_test tests/hash_manager_test.cpp examples/hashable_states.cpp)
target_link_libraries(hash_manager_test ${PROJECT_NAME} libopencv)

# catkin_add_gtest(boost_environment_test tests/boost_environment_test.cpp)
# target_link_libraries(boost_environment_test ${PROJECT_NAME})

add_executable(boost_environment_test tests/boost_environment_test.cpp)
target_link_libraries(boost_environment_test ${PROJECT_NAME})
