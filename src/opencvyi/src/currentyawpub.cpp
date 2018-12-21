#include <ros/ros.h>
#include <opencvyi/position.h>
#include <std_msgs/Float32.h>
#include "LinuxSerial.hpp"
#include "common_serial.hpp"
#include "camera_driver.hpp"
#include "armor_detect.hpp"
#include "angle_slover.hpp"
#include "armor_recorder.hpp"
#include "debug_utility.hpp"
#include "armor_info.hpp"
void posCallback(const opencvyi::position::ConstPtr &msg)
{ autocar::serial_mul::listen2car();
  short current_yaw= autocar::serial_mul::get_yaw();
  short current_pitch = autocar::serial_mul::get_pitch();
    autocar::vision_mul::armor_pos armor_pos_;
             armor_pos_.angle_x=msg->x;
             armor_pos_.angle_y=msg->y;
             armor_pos_.angle_z=msg->z;
             armor_pos_.Flag=msg->Flag ; 
    autocar::serial_mul::publish2car(armor_pos_, current_yaw, current_pitch);
    //计算离原点(0,0)的距离
   // std_msgs::Float32 distance;
   // distance.data = sqrt(pow(msg->x,2)+pow(msg->y,2));
    //float distance = sqrt(pow(msg->x,2)+pow(msg->y,2));
  //  ROS_INFO("listener: pos: x = %f, y = %f ,z = %f,flag = %d",  msg->x ,msg->y,msg->z,msg->Flag);
  ///srv  ROS_INFO("Listener: Distance to origin = %f, state: %s",distance.data,msg->state.c_str());
 // printf ("ss\n");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("pos_info", 1, posCallback);
  //ros::spin()用于调用所有可触发的回调函数。将进入循环，不会返回，类似于在循环里反复调用ros::spinOnce()。
  ros::spin(); 
  return 0;
}
