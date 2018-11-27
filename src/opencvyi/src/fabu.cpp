#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "camera_driver.hpp"
#include "debug_utility.hpp"
#include "video_recoder.hpp"
using namespace cv;
using namespace std;
//#define USE_VIDEO

int main(int argc, char **argv)
{  autocar::driver::camera_driver camera_info;
Mat recordframe;
autocar::vision_mul::video_recoder recoder;
int saveframe;
#ifndef USE_VIDEO
   VideoCapture cap;
    camera_info.open_camera(cap);
   // cap.open(0);
    if (!cap.isOpened()) {
		std::cerr << "ERROR! Unable to open camera\n";
		return -1;
	}
#else
     VideoCapture cap;
    cap.open("/home/cuicheng/work/src/opencvyi/video/123.MOV");
    if (!cap.isOpened()) {
		std::cerr << "ERROR! Unable to open video\n";
		return -1;
	}
#endif
    ros::init(argc,argv,"image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/image", 1);
    cv_bridge::CvImagePtr frame = boost::make_shared< cv_bridge::CvImage >();
    frame->encoding = sensor_msgs::image_encodings::BGR8;
    bool ifget1 = ros::param::get("save_result", saveframe);
    #ifdef USE_VIDEO  
    saveframe=0;
    #endif
     if (ifget1==1&&saveframe==1)
      printf("理论上应该开始录制了");
      else if (ifget1==0)
       printf("没有读取到是否录制参数信息");
     else printf("不保存内容模式");
       
    while(ros::ok())
    {  
    
       cap >> frame->image;       
       if( frame->image.empty() )
       {
        ROS_ERROR_STREAM( "Failed to capture frame!" );
        ros::shutdown();
       }
       if (saveframe) 
       {frame->image.copyTo(recordframe);
       recoder.save_frame(recordframe);}
     frame->header.stamp = ros::Time::now();
       
         pub.publish(frame->toImageMsg());
         cv::waitKey( 30 );
         ros::spinOnce();
         }
    cap.release();
}
