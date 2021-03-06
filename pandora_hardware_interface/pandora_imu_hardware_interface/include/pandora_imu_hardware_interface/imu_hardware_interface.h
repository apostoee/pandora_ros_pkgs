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
*********************************************************************/
#ifndef PANDORA_IMU_HARDWARE_INTERFACE_IMU_HARDWARE_INTERFACE_H
#define PANDORA_IMU_HARDWARE_INTERFACE_IMU_HARDWARE_INTERFACE_H

#include "ros/ros.h"
#include "tf/tf.h"
#include <boost/math/constants/constants.hpp>
#include <hardware_interface/imu_sensor_interface.h>
#include <hardware_interface/robot_hw.h>
#include <controller_manager/controller_manager.h>
#include <pandora_imu_hardware_interface/imu_serial_interface.h>

namespace pandora_hardware_interface
{
namespace imu
{
  class ImuHardwareInterface : public hardware_interface::RobotHW
  {
    private:
      ros::NodeHandle nodeHandle_;

      ImuSerialInterface imuSerialInterface;
      hardware_interface::ImuSensorInterface imuSensorInterface_;
      hardware_interface::ImuSensorHandle::Data imuData_;
      double imuOrientation[4];

    public:
      explicit ImuHardwareInterface(
        ros::NodeHandle nodeHandle);
      ~ImuHardwareInterface();
      void read();
  };
}  // namespace imu
}  // namespace pandora_hardware_interface
#endif  // PANDORA_IMU_HARDWARE_INTERFACE_IMU_HARDWARE_INTERFACE_H
