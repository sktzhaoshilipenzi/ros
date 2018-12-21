#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "camera_driver.hpp"
#include "armor_detect.hpp"
#include "angle_slover.hpp"
#include "armor_recorder.hpp"
#include "debug_utility.hpp"
#include "armor_info.hpp"
#include <chrono>
#include <vector>
#include "opencvyi/position.h"
      
class ImageConvertor  
    {   public: 
        sensor_msgs::ImagePtr msgs;
        opencvyi::position message;  
        ros::NodeHandle nh_;  
        //image_transport::ImageTransport it_;
        
       
        image_transport::Subscriber image_sub_;
   // cv::Mat  A = cv::Mat::eye(3,3,CV_64FC1);
   // cv::Mat   B = cv::Mat::zeros(3,1,CV_64FC1);
      // cv::Mat A=Mat::eye(3,3, CV_32FC1);
      // cv::Mat B=Mat::eye(3,1, CV_32FC1);
        autocar::driver::camera_driver camera_info; 
       // AngleSolver angle_slover(1,1);
        //AngleSolver * angle_slover;
        Point2f image_center ;
        AngleSolverFactory angle_slover_factory;
        autocar::vision_mul::ArmorDetector armor_detector;
        autocar::slover::Armor_recorder armor_recorder;
        std::vector<autocar::vision_mul::armor_info> multi_armors;
        autocar::vision_mul::armor_pos armor_pos_; 
        ros::Publisher pub;
        
        ImageConvertor(//AngleSolver angle_slover1,
 autocar::driver::camera_driver camera_info1, Point2f image_center1 ,
 AngleSolverFactory angle_slover_factory1,autocar::vision_mul::ArmorDetector armor_detector1
 ,autocar::slover::Armor_recorder armor_recorder1,
 std::vector<autocar::vision_mul::armor_info> multi_armors1,autocar::vision_mul::armor_pos armor_pos_1
 ){  
            
            //image_transport::ImageTransport it_(nh_);
            //it_=it;
            /*发布主题out*/  
           // angle_slover=&angle_slover1;
            camera_info=camera_info1;
            image_center=image_center1;
            angle_slover_factory=angle_slover_factory1;
            //angle_slover_factory.setSolver(angle_slover);
           // angle_slover_factory.setSolver(angle_slover);
            //angle_slover_factory=angle_slover_factory1;
           armor_detector=armor_detector1;
            armor_recorder=armor_recorder1;
            multi_armors=multi_armors1;
            armor_pos_=armor_pos_1;
            pub = nh_.advertise<opencvyi::position>("pos_info", 1); 
                    /*订阅主题camera/image*/ 
            image_transport::ImageTransport it_(nh_);         
            image_sub_ = it_.subscribe("camera/image", 1, &ImageConvertor::ImageCb, this); 
            //cv::namedWindow(OUT_WINDOW, CV_WINDOW_AUTOSIZE);  
            //cv::namedWindow(IN_WINDOW, CV_WINDOW_AUTOSIZE);  
        }  
      
        ~ImageConvertor()  
        {  
            //cv::destroyWindow(IN_WINDOW);  
            //cv::destroyWindow(OUT_WINDOW);  
        }  
         void ImageCb(const sensor_msgs::ImageConstPtr& msg)  
        {AngleSolver angle_slover1(camera_info.camera_matrix, camera_info.dist_coeffs, 21.6, 5.4, camera_info.z_scale);
        angle_slover1.setRelationPoseCameraPTZ(camera_info.r_camera_ptz, camera_info.t_camera_ptz, camera_info.barrel_ptz_offset_y);
        angle_slover_factory.setSolver(&angle_slover1);
        float x,y,z;
         int flag;
  //angle_slover_factory.setSolver(&angle_slover);
    armor_pos_.reset_pos();
  multi_armors.clear();
  cv::Mat  yingyingying;
  cv_bridge::toCvShare(msg, "bgr8")->image.copyTo(yingyingying);
  if (armor_detector.detect(yingyingying, multi_armors)
            && multi_armors.size())
        {
            armor_pos_ = armor_recorder.SlectFinalArmor(multi_armors,*angle_slover_factory.slover, angle_slover_factory,yingyingying);
           // serial_mul::publish2car(armor_pos_, current_yaw, current_pitch);  // 发布消息
        }
        else{
            armor_pos_.reset_pos();
            //serial_mul::publish2car(armor_pos_, current_yaw, current_pitch); 
        }
  if (armor_pos_.Flag)
        {
            armor_recorder.miss_detection_cnt = 0;
            armor_recorder.setLastResult(armor_pos_, record_time);
        }
        else {
            armor_recorder.miss_detection_cnt++;
            if (armor_recorder.miss_detection_cnt > 3) armor_recorder.clear();
        }

 
        
              // printf("%f  %f %f\n",hh.angle_x,hh.angle_y,hh.angle_z);
              // printf("%f %f %f \n",armor_pos_.angle_x,armor_pos_.angle_y,armor_pos_.angle_z); 
             x=armor_pos_.angle_x;
             y=armor_pos_.angle_y;
             z=armor_pos_.angle_z;
             flag=armor_pos_.Flag;           
  //for(const auto& hh : armor_recorder.history_pos)
      //  {
              // printf("%f  %f %f\n",hh.angle_x,hh.angle_y,hh.angle_z);
        //       printf("%f %f %f \n",hh.angle_x,hh.angle_y,hh.angle_z); }
  try
  {
    cv::imshow("before", cv_bridge::toCvShare(msg, "bgr8")->image);
    
    cv::waitKey(3);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
  
  
    try {char str[30];
            sprintf(str, "%.2f, %.2f, %.2fcm", armor_pos_.angle_x, armor_pos_.angle_y, armor_pos_.angle_z);
            putText(yingyingying, str, Point(5, 20), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(0,205,0), 1);
                
				imshow("result", yingyingying);
                cv::waitKey(3); // 很有必要
			}
			catch (cv::Exception e) {
				std::cout << "show image error" << std::endl;
			}

message.x=x;
  message.y=y;
  message.z=z;
  message.Flag=flag;
  //ROS_INFO("Talker: GPS: x = %f, y = %f ,z = %f,flag = %d",  message.x ,message.y,message.z,message.Flag);
  //printf("Talker: GPS: x = %f, y = %f ,z = %f,flag = %d",  message.x ,message.y,message.z,message.Flag);
  pub.publish(message);
    
  
            }
      };
 
int main(int argc, char **argv)
{  autocar::driver::camera_driver camera_info; 
       // AngleSolver angle_slover(1,1);
      //  AngleSolver * angle_slover;
       // Point2f image_center ;
        AngleSolverFactory angle_slover_factory;
        autocar::vision_mul::ArmorDetector armor_detector;
        autocar::slover::Armor_recorder armor_recorder;
        std::vector<autocar::vision_mul::armor_info> multi_armors;
        autocar::vision_mul::armor_pos armor_pos_; 
    //angle_slover.setRelationPoseCameraPTZ(camera_info.r_camera_ptz, camera_info.t_camera_ptz, camera_info.barrel_ptz_offset_y);
  //angle_slover_factory.setTargetSize(21.6, 5.4, AngleSolverFactory::TARGET_ARMOR);
  //angle_slover_factory.setTargetSize(12.4, 5.4, AngleSolverFactory::TARGET_SAMLL_ARMOR);
 
 
 // autocar::driver::camera_driver camera_info;
 // autocar::vision_mul::armor_pos armor_pos_;
 // autocar::slover::Armor_recorder armor_recorder;
  camera_info.set_camera_pose();
  
   // angle_slover=&angle_slover1;
  Point2f image_center = Point2f(camera_info.camera_matrix.at<double>(0,2), camera_info.camera_matrix.at<double>(1,2));
  
  //AngleSolverFactory angle_slover_factory;
  angle_slover_factory.setTargetSize(21.6, 5.4, AngleSolverFactory::TARGET_ARMOR);
  angle_slover_factory.setTargetSize(12.4, 5.4, AngleSolverFactory::TARGET_SAMLL_ARMOR);
 // angle_slover_factory.setSolver(&angle_slover);
  //autocar::vision_mul::ArmorDetector armor_detector;
  //autocar::param_mul::armor_param  par;
  //armor_detector.setPara(par);
  //std::vector<autocar::vision_mul::armor_info> multi_armors;
  ros::init(argc, argv, "image_listener3");
   ImageConvertor ic(camera_info,image_center,angle_slover_factory,armor_detector,armor_recorder,multi_armors,armor_pos_);
 // ImageConvertor ic();
  //image_transport::ImageTransport it(nh);
  //image_transport::Subscriber sub = it.subscribe("camera/image", 1, boost::bind(&imageCallback,_1,angle_slover,camera_info,image_center,angle_slover_factory,armor_detector,armor_recorder,multi_armors,armor_pos_)); 

  //message.x=x;
  //message.y=y;
  //message.z=z;
  //message.Flag=flag;
  //ros::Publisher pub = nh.advertise<opencvyi::position>("pos_info", 1);
  //ROS_INFO("Talker: GPS: x = %f, y = %f ,z = %f,flag = %d",  message.x ,message.y,message.z,message.Flag);
  //printf("Talker: GPS: x = %f, y = %f ,z = %f,flag = %d",  message.x ,message.y,message.z,message.Flag);
  //pub.publish(message);
  ros::spin();
  //cv::destroyWindow("before");
}
