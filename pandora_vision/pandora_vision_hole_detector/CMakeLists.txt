cmake_minimum_required(VERSION 2.8.3)
project(pandora_vision_hole_detector)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  vision_communications
  state_manager
  state_manager_communications
  cv_bridge
  image_transport
  sensor_msgs
  dynamic_reconfigure
  pcl_ros
  message_filters
  roslint
)

generate_dynamic_reconfigure_options(
  cfg/depth_cfg.cfg
  cfg/hole_fusion_cfg.cfg
  cfg/rgb_cfg.cfg
)

catkin_package(
  DEPENDS
  CATKIN_DEPENDS
  roscpp
  vision_communications
  state_manager
  state_manager_communications
  cv_bridge
  image_transport
  sensor_msgs
  dynamic_reconfigure
  pcl_ros
  message_filters
  roslint
  INCLUDE_DIRS
  include
  LIBRARIES
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)


########################### DepthNode ############################
add_library(depth_node_lib
  src/utils/message_conversions.cpp
  src/utils/hole_filters.cpp
  src/utils/holes_conveyor.cpp
  src/utils/noise_elimination.cpp
  src/utils/edge_detection.cpp
  src/utils/blob_detection.cpp
  src/utils/bounding_box_detection.cpp
  src/utils/morphological_operators.cpp
  src/utils/visualization.cpp
  src/utils/timer.cpp
  src/utils/parameters.cpp
  )
target_link_libraries(depth_node_lib
  ${catkin_LIBRARIES}
)

add_executable(depth_node
  src/depth_node/depth.cpp
  src/depth_node/depth_node.cpp
  src/depth_node/hole_detector.cpp
)

set (DEPTH_COMPILE_FLAGS "-Wno-deprecated")

target_link_libraries(depth_node
  depth_node_lib
  ${catkin_LIBRARIES}
)

set_target_properties(depth_node
  PROPERTIES COMPILE_FLAGS
  ${DEPTH_COMPILE_FLAGS}
)

add_dependencies(depth_node
  vision_gencfg
  vision_communications_gencpp
  ${catkin_EXPORTED_TARGETS}
)


########################### RgbDepthSynchronizerNode ###########################
add_library(synchronizer_lib
  src/utils/message_conversions.cpp
  src/utils/timer.cpp
)
target_link_libraries(synchronizer_lib
  ${catkin_LIBRARIES}
)

add_executable(synchronizer_node
  src/synchronizer_node/rgb_depth_synchronizer.cpp
  src/synchronizer_node/rgb_depth_synchronizer_node.cpp
)

target_link_libraries(synchronizer_node
  synchronizer_lib
  ${catkin_LIBRARIES}
)

add_dependencies(synchronizer_node
  vision_gencfg
  vision_communications_gencpp
  ${catkin_EXPORTED_TARGETS}
)


############################## HoleFusionNode ##################################
add_library(hole_fusion_lib
  src/utils/message_conversions.cpp
  src/utils/blob_detection.cpp
  src/utils/bounding_box_detection.cpp
  src/utils/parameters.cpp
  src/utils/edge_detection.cpp
  src/utils/hole_filters.cpp
  src/utils/morphological_operators.cpp
  src/utils/noise_elimination.cpp
  src/utils/timer.cpp
  src/utils/holes_conveyor.cpp
  src/utils/visualization.cpp
)

target_link_libraries(hole_fusion_lib
  ${catkin_LIBRARIES}
)

add_executable(hole_fusion_node
  src/hole_fusion_node/hole_fusion.cpp
  src/hole_fusion_node/hole_fusion_node.cpp
  src/hole_fusion_node/hole_merger.cpp
  src/hole_fusion_node/rgb_filters.cpp
  src/hole_fusion_node/depth_filters.cpp
  src/hole_fusion_node/planes_detection.cpp
)

target_link_libraries(hole_fusion_node
  hole_fusion_lib
  ${catkin_LIBRARIES}
)

add_dependencies(hole_fusion_node
  vision_gencfg
  vision_communications_gencpp
  ${catkin_EXPORTED_TARGETS}
)


################################# RgbDummyNode #################################
add_executable(rgb_dummy_node
  src/rgb_dummy_node/rgb_dummy_node.cpp
  src/rgb_dummy_node/rgb_dummy.cpp
  src/utils/hole_filters.cpp
  src/utils/message_conversions.cpp
  src/utils/noise_elimination.cpp
  src/utils/edge_detection.cpp
  src/utils/blob_detection.cpp
  src/utils/bounding_box_detection.cpp
  src/utils/timer.cpp
  src/utils/parameters.cpp
  src/utils/morphological_operators.cpp
  src/utils/visualization.cpp
)

target_link_libraries(rgb_dummy_node ${catkin_LIBRARIES} )

add_dependencies(rgb_dummy_node
  vision_communications_gencpp
  ${catkin_EXPORTED_TARGETS}
)

################################ RgbNode ######################################
add_library(rgb_node_lib
  src/utils/hole_filters.cpp
  src/utils/holes_conveyor.cpp
  src/utils/noise_elimination.cpp
  src/utils/edge_detection.cpp
  src/utils/blob_detection.cpp
  src/utils/bounding_box_detection.cpp
  src/utils/message_conversions.cpp
  src/utils/morphological_operators.cpp
  src/utils/visualization.cpp
  src/utils/parameters.cpp
  src/utils/timer.cpp
)

target_link_libraries(rgb_node_lib
  ${catkin_LIBRARIES} )

add_executable(rgb_node
  src/rgb_node/hole_node.cpp
  src/rgb_node/hole_detection.cpp
  src/rgb_node/hole_detector.cpp
  src/rgb_node/texture_filter.cpp
)

target_link_libraries(rgb_node
  rgb_node_lib
  ${catkin_LIBRARIES}
)

add_dependencies(rgb_node
  vision_gencfg
  vision_communications_gencpp
  ${catkin_EXPORTED_TARGETS}
)

#~ To check lint : catkin_make roslint_pandora_vision_hole_detector
set(ROSLINT_CPP_OPTS
  "--filter=-whitespace/end_of_line, -build/include_order, -whitespace/blank_line, -whitespace/parens, -whitespace/comments, -build/include_what_you_use, -whitespace/braces, -whitespace/newline, -readability/multiline_string, -runtime/printf, -readability/streams")

FILE(GLOB_RECURSE ${PROJECT_NAME}_LINT_SRCS
  RELATIVE ${PROJECT_SOURCE_DIR}

  include/rgb_node/hole_detection.h
  include/rgb_node/hole_detector.h
  include/rgb_node/edge_detection.h
  include/rgb_node/texture_filter.h
  include/rgb_node/blob_extraction.h
  include/rgb_node/rgb_constants.h
  src/rgb_node/hole_detection.cpp
  src/rgb_node/hole_detector.cpp
  src/rgb_node/edge_detection.cpp
  src/rgb_node/texture_filter.cpp
  src/rgb_node/blob_extraction.cpp
  src/rgb_node/rgb_constants.cpp
  )

################################# ROSLINT ######################################
# Execute via lint : catkin_make roslint_{PACKAGE_NAME}

set(ROSLINT_CPP_OPTS
  "--filter=-whitespace/end_of_line, -build/include_order, -whitespace/blank_line, -whitespace/parens, -whitespace/comments, -build/include_what_you_use, -whitespace/braces, -whitespace/newline, -readability/multiline_string, -runtime/printf, -readability/streams")

FILE(GLOB_RECURSE ${PROJECT_NAME}_LINT_SRCS
  RELATIVE ${PROJECT_SOURCE_DIR}

  ## depth_node ##

  include/depth_node/depth.h
  include/depth_node/hole_detector.h

  src/depth_node/depth.cpp
  src/depth_node/depth_node.cpp
  src/depth_node/hole_detector.cpp


  ## utils ##

  include/utils/blob_detection.h
  include/utils/bounding_box_detection.h
  include/utils/defines.h
  include/utils/parameters.h
  include/utils/edge_detection.h
  include/utils/hole_filters.h
  include/utils/holes_conveyor.h
  include/utils/morphological_operators.h
  include/utils/noise_elimination.h
  #include/utils/timer.h
  include/utils/visualization.h
  include/utils/message_conversions.h

  src/utils/blob_detection.cpp
  src/utils/bounding_box_detection.cpp
  src/utils/defines.h
  src/utils/parameters.cpp
  src/utils/edge_detection.cpp
  src/utils/hole_filters.cpp
  src/utils/holes_conveyor.cpp
  src/utils/morphological_operators.cpp
  src/utils/noise_elimination.cpp
  #src/utils/timer.cpp
  src/utils/visualization.cpp
  src/utils/message_conversions.cpp


  ## hole_fusion_node ##

  include/hole_fusion_node/depth_filters.h
  include/hole_fusion_node/hole_merger.h
  include/hole_fusion_node/hole_fusion.h
  include/hole_fusion_node/planes_detection.h
  include/hole_fusion_node/rgb_filters.h

  src/hole_fusion_node/depth_filters.cpp
  src/hole_fusion_node/hole_merger.cpp
  src/hole_fusion_node/hole_fusion.cpp
  src/hole_fusion_node/hole_fusion_node.cpp
  src/hole_fusion_node/planes_detection.cpp
  src/hole_fusion_node/rgb_filters.cpp


  ## synchronizer_node ##

  include/synchronizer_node/rgb_depth_synchronizer.h

  src/synchronizer_node/rgb_depth_synchronizer.cpp
  src/synchronizer_node/rgb_depth_synchronizer_node.cpp
  )

roslint_cpp(${${PROJECT_NAME}_LINT_SRCS})