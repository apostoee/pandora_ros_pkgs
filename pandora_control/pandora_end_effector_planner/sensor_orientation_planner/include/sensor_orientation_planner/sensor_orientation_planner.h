/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014, P.A.N.D.O.R.A. Team.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author:  Evangelos Apostolidis
* Author:  Chris Zalidis
*********************************************************************/
#ifndef SENSOR_ORIENTATION_PLANNER_SENSOR_ORIENTATION_PLANNER_H
#define SENSOR_ORIENTATION_PLANNER_SENSOR_ORIENTATION_PLANNER_H

#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <actionlib/server/simple_action_server.h>
#include <pandora_end_effector_planner/MoveSensorAction.h>
#include <tf/transform_listener.h>


namespace pandora_control
{
  enum
  {
    HIGH_START = 0,
    LOW_START = 1,
    HIGH_LEFT = 2,
    LOW_LEFT = 3,
    HIGH_CENTER = 4,
    LOW_CENTER = 5,
    HIGH_RIGHT = 6,
    LOW_RIGHT = 7,
  };

  class SensorOrientationActionServer
  {
    private:
      ros::NodeHandle nodeHandle_;
      std::string actionName_;
      actionlib::SimpleActionServer<
        pandora_end_effector_planner::MoveSensorAction> actionServer_;

      ros::Publisher sensorPitchPublisher_;
      ros::Publisher sensorYawPublisher_;

      int position_;
      int command_;
      double maxPitch_;
      double maxYaw_;
      double timeStep_;
      std::string pitchCommandTopic_;
      std::string yawCommandTopic_;
      std::string sensorFrame_;

      tf::TransformListener tfListener_;

      void callback(const pandora_end_effector_planner::MoveSensorGoalConstPtr& goal);

      bool getPlannerParams();
      void centerSensor();
      void scan();
      void pointSensor(std::string pointOfInterest);

    public:
      SensorOrientationActionServer(
        std::string name,
        ros::NodeHandle nodeHandle_);

      ~SensorOrientationActionServer(void);
  };
}  // namespace pandora_control
#endif  // SENSOR_ORIENTATION_PLANNER_SENSOR_ORIENTATION_PLANNER_H
