/*********************************************************************
*
* Software License Agreement (BSD License)
*
* Copyright (c) 2014, P.A.N.D.O.R.A. Team.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following
* disclaimer in the documentation and/or other materials provided
* with the distribution.
* * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* Author: Victor Daropoulos
*********************************************************************/

#include "pandora_vision_landoltc/landoltc3d_detection.h"


namespace pandora_vision
{

/**
@brief Default Constructor
@param void
@return void
**/

LandoltC3dDetection::LandoltC3dDetection(const std::string& ns): _nh(ns), landoltc3dNowON(true) 
{
  getGeneralParams();
  
  //!< Initiliaze and preprocess reference image
  _landoltc3dDetector.initializeReferenceImage(patternPath);
  
  if(PredatorOn)
  {
    _landoltc3dPredator = _nh.subscribe(predator_topic_name, 1,
    &LandoltC3dDetection::predatorCallback, this);
  }
  else
  {
    _inputImageSubscriber = _nh.subscribe(imageTopic, 1,
    &LandoltC3dDetection::imageCallback, this);
  }
      
  ROS_INFO("[landoltc3d_node] : Created LandoltC3d Detection instance");

}

/**
@brief Default destructor
@return void
**/

LandoltC3dDetection::~LandoltC3dDetection(void)
{
  ROS_INFO("[landoltc3d_node] : Destroying LandoltC3d Detection instance");
}

/**
@brief Get parameters referring to view and frame characteristics
@return void
**/
void LandoltC3dDetection::getGeneralParams()
{
  packagePath = ros::package::getPath("pandora_vision_landoltc");
  
  //! Declare publisher and advertise topic
  //! where algorithm results are posted if it works alone
  if (_nh.getParam("published_topic_names/landoltc_alert", param))
  {
    _landoltc3dPublisher = 
      _nh.advertise<vision_communications::LandoltcAlertsVectorMsg>(param, 10);
  }
  else
  {
    ROS_FATAL("Landoltc alert topic name not found");
    ROS_BREAK();
  }
  
  //! Declare subscriber
  //! where algorithm results are posted if it works with predator
  if (_nh.getParam("subscribed_topic_names/predator_topic_name", predator_topic_name))
  {
    ROS_DEBUG("Loaded topic name to use with predator");
  }
  else
  {
    ROS_FATAL("Landoltc image topic name not found");
    ROS_BREAK();
  }
  
  
  //!< Get the path to the pattern used for detection
  if (_nh.hasParam("patternPath"))
  {
    _nh.getParam("patternPath", patternPath);
    ROS_DEBUG_STREAM("patternPath: " << patternPath);
  }
  else
  {
    ROS_DEBUG("[landoltc3d_node] : Parameter patternPath not found. Using Default");
    std:: string temp = "/bold.jpg";
    patternPath.assign(packagePath);
    patternPath.append(temp);
  }
  
  //!< Get the PredatorOn value
  if(_nh.getParam("operation_state", PredatorOn))
  {
    ROS_DEBUG("[landoltc3d_node] : Loading predator_on parameter");
  }
  else
  {
    ROS_DEBUG("[landoltc3d_node] : Parameter PredatorOn not found. Using Default");
    PredatorOn = false;
  }

  //!< Get the camera to be used by landoltc3d node;
  if (_nh.hasParam("camera_name"))
  {
    _nh.getParam("camera_name", cameraName);
    ROS_DEBUG_STREAM("camera_name : " << cameraName);
  }
  else
  {
    ROS_DEBUG("[landoltc3d_node] : Parameter frameHeight not found. Using Default");
    cameraName = "camera";
  }

  //!< Get the Height parameter if available;
  if (_nh.hasParam("/" + cameraName + "/image_height"))
  {
    _nh.getParam("/" + cameraName + "/image_height", frameHeight);
    ROS_DEBUG_STREAM("height : " << frameHeight);
  }
  else
  {
    ROS_DEBUG("[landoltc3d_node] : Parameter frameHeight not found. Using Default");
    frameHeight = DEFAULT_HEIGHT;
  }

  //!< Get the Width parameter if available;
  if (_nh.hasParam("/" + cameraName + "/image_width"))
  {
    _nh.getParam("/" + cameraName + "/image_width", frameWidth);
    ROS_DEBUG_STREAM("width : " << frameWidth);
  }
  else
  {
    ROS_DEBUG("[landoltc3d_node] : Parameter frameWidth not found. Using Default");
    frameWidth = DEFAULT_WIDTH;
  }

  //!< Get the listener's topic;
  if (_nh.hasParam("/" + cameraName + "/topic_name"))
  {
    _nh.getParam("/" + cameraName + "/topic_name", imageTopic);
    ROS_DEBUG_STREAM("imageTopic : " << imageTopic);
  }
  else
  {
    ROS_DEBUG("[landoltc3d_node] : Parameter imageTopic not found. Using Default");
    imageTopic = "/camera_head/image_raw";
  }

  //!< Get the images's frame_id;
  if (_nh.hasParam("/" + cameraName + "/camera_frame_id"))
  {
    _nh.getParam("/" + cameraName + "/camera_frame_id", cameraFrameId);
    ROS_DEBUG_STREAM("camera_frame_id : " << cameraFrameId);
  }
  else
  {
    ROS_DEBUG("[landoltc3d_node] : Parameter camera_frame_id not found. Using Default");
    cameraFrameId = "/camera";
  }
}

/**
@brief Callback for the RGB Image
@param msg [const sensor_msgs::ImageConstPtr& msg] The RGB Image
@return void
**/

void LandoltC3dDetection::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{

  cv_bridge::CvImagePtr in_msg;
  in_msg = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  landoltCFrame = in_msg -> image.clone();
  if ( landoltCFrame.empty() )
  {
    ROS_ERROR("[landoltc3d_node] : No more Frames");
    return;
  }
  //ROS_INFO("Getting Frame From Camera");

  landoltc3dCallback();

}

void LandoltC3dDetection::predatorCallback(const vision_communications::LandoltcPredatorMsg& msg)
{
 
  cv_bridge::CvImagePtr in_msg;
  in_msg = cv_bridge::toCvCopy(msg.img, sensor_msgs::image_encodings::BGR8);
  landoltCFrame = in_msg -> image.clone();
  cv::Rect bounding_box = cv::Rect(msg.x, msg.y, msg.width, msg.height);
  float posterior = msg.posterior;
  //ROS_INFO("Getting Frame From Predator");
  landoltc3dCallback();
}


/**
 @brief main function called for publishing messages in
  data fusion. In this function an object of class landoltc3dDetector
  is created to identify landoltCs in current frame.
 @param void
 @return void
 **/

void LandoltC3dDetection::landoltc3dCallback()
{
  if(!landoltc3dNowON)
  {
    return;
  }
    
  _landoltc3dDetector.begin(&landoltCFrame);
  
  //!< Create message of Landoltc Detector
  vision_communications::LandoltcAlertsVectorMsg Landoltc3dVectorMsg;
  vision_communications::LandoltcAlertMsg Landoltc3dcodeMsg;
  Landoltc3dVectorMsg.header.frame_id = cameraFrameId;
  Landoltc3dVectorMsg.header.stamp = ros::Time::now();
}

/**
  @brief Node's state manager
  @param newState [int] The robot's new state
  @return void
*/
void LandoltC3dDetection::startTransition(int newState)
{
  curState = newState;

  //!< check if datamatrix algorithm should be running now
  landoltc3dNowON =
    (curState ==
     state_manager_communications::robotModeMsg::MODE_EXPLORATION)
    || (curState ==
        state_manager_communications::robotModeMsg::MODE_IDENTIFICATION)
    || (curState ==
        state_manager_communications::robotModeMsg::MODE_ARM_APPROACH)
    || (curState ==
        state_manager_communications::robotModeMsg::MODE_TELEOPERATED_LOCOMOTION)
    || (curState ==
        state_manager_communications::robotModeMsg::MODE_DF_HOLD);

  //!< shutdown if the robot is switched off
  if (curState ==
      state_manager_communications::robotModeMsg::MODE_TERMINATING)
  {
    ros::shutdown();
    return;
  }

  prevState = curState;

}

/**
 @brief After completion of state transition
 @return void
 */
void LandoltC3dDetection::completeTransition()
{
  ROS_INFO("[Landoltc3d_node] : Transition Complete");
}

} // namespace pandora_vision
