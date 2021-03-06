find_package(catkin REQUIRED COMPONENTS
  rostest
  roslib 
  map_server
  )

find_package(PkgConfig)
pkg_check_modules(NEW_YAMLCPP yaml-cpp>=0.5)
if(NEW_YAMLCPP_FOUND)
  add_definitions(-DHAVE_NEW_YAMLCPP)
endif(NEW_YAMLCPP_FOUND)

link_directories(${catkin_LIBRARY_DIRS})
include_directories(map_loader/include ${catkin_INCLUDE_DIRS})

########################  map loader  ###############################

add_library(map_loader map_loader/src/map_loader.cpp)
target_link_libraries(map_loader 
  image_loader
  yaml-cpp
  ${catkin_LIBRARIES}
  )

########################  Tests  ####################################

########################  Unit Tests  ###############################

########################  Utils  ####################################

catkin_add_gtest(utils_test unit/utils_test.cpp)
target_link_libraries(utils_test 
  ${catkin_LIBRARIES} 
  utils 
  gtest_main
  )

#########################  ObjectsTest  #############################

catkin_add_gtest(objects_test unit/objects_test.cpp)
target_link_libraries(objects_test 
  ${catkin_LIBRARIES}
  utils
  objects
  gtest_main
  ) 

########################  ObjectListTest  ###########################

catkin_add_gtest(object_list_test unit/object_list_test.cpp)
target_link_libraries(object_list_test 
  ${catkin_LIBRARIES} 
  objects 
  utils
  gtest_main
  )

########################  ObjectFactoryTest  ########################

catkin_add_gtest(object_factory_test unit/object_factory_test.cpp)
target_link_libraries(object_factory_test 
  ${catkin_LIBRARIES} 
  object_factory
  map_loader
  gtest_main
  ) 

########################  PoseFinderTest  ############################

catkin_add_gtest(pose_finder_test unit/pose_finder_test.cpp)
target_link_libraries(pose_finder_test 
  ${catkin_LIBRARIES}
  pose_finder 
  map_loader 
  gtest_main
  )

#########################  VictimTest  ##############################

catkin_add_gtest(victim_test unit/victim_test.cpp)
target_link_libraries(victim_test
  ${catkin_LIBRARIES}
  objects
  victim 
  gtest_main
  ) 
   
#########################  VictimListTest  ##########################

catkin_add_gtest(victim_list_test unit/victim_list_test.cpp)
target_link_libraries(victim_list_test 
  ${catkin_LIBRARIES}
  objects
  victim_list
  gtest_main
  ) 

########################  VictimClustererTest  ######################

catkin_add_gtest(victim_clusterer_test unit/victim_clusterer_test.cpp)
target_link_libraries(victim_clusterer_test ${catkin_LIBRARIES}
  victim_clusterer 
  objects
  victim
  gtest_main
  ) 

#########################  Functional Tests  ########################
 
add_rostest(functional/alert_handler_static_test.launch)
add_rostest(functional/alert_handler_test.launch)

