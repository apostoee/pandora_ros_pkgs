cmake_minimum_required(VERSION 2.8.3)
project(watchdog_communications)
find_package(catkin REQUIRED COMPONENTS message_generation std_msgs)

add_message_files(
  FILES
  watchdogResetMsg.msg
  wdtTimeoutNotificationMsg.msg
  )

add_service_files(
  FILES
  watchdogSrv.srv
  )
  

generate_messages(
  DEPENDENCIES std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime 
)
