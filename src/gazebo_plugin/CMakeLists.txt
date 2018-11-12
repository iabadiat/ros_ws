cmake_minimum_required(VERSION 2.8.3)
project(gazebo_plugin)

# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS 
  roscpp 
  gazebo_ros
  baxter_core_msgs 
)

find_package(std_msgs REQUIRED)

add_compile_options(-std=c++11)
# Depend on system install of Gazebo
find_package(gazebo REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS} ${roscpp_INCLUDE_DIRS} ${std_msgs_INCLUDE_DIRS})

add_library(${PROJECT_NAME} src/step_world_plugin.cpp)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES} ${roscpp_LIBRARIES})

catkin_package(
  DEPENDS 
    roscpp 
    gazebo_ros 
    baxter_core_msgs
)


