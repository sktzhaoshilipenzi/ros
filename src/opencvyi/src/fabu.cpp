#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//#include "camera_driver.hpp"
#include "debug_utility.hpp"
#include "video_recoder.hpp"
using namespace cv;
using namespace std;
#define USE_VIDEO
bool open_camera(cv::VideoCapture & capture_camera_forward )
{   std::string usb_cam_id;
    int exposure_time ;
    capture_camera_forward.set(CV_CAP_PROP_FPS,120);
    bool ifget2 = ros::param::get("usb_cam_id",usb_cam_id);
    bool ifget3 = ros::param::get("exposure_time",exposure_time);
    if (ifget2==1&&ifget3==1)
    {
      printf("读取摄像头参数成功");
      capture_camera_forward.open(usb_cam_id); // open camera of ptz
      if(!capture_camera_forward.isOpened())
    {
        std::cout<<"Cannot open capture_camera_forward!"<<std::endl;
        return false;
    }
    capture_camera_forward.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
    capture_camera_forward.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture_camera_forward.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    set_camera_exposure(usb_cam_id, exposure_time);

    }
    else 
    printf("读取摄像头参数失败");
 
return true;
  
}
int main(int argc, char **argv)
{  //autocar::driver::camera_driver camera_info;
   ros::init(argc,argv,"image_publisher");
Mat recordframe;
autocar::vision_mul::video_recoder recoder;
int saveframe;
#ifndef USE_VIDEO
   VideoCapture cap;
    //camera_info.open_camera(cap);
   // cap.open(0);
   open_camera(cap);
    if (!cap.isOpened()) {
		std::cerr << "ERROR! Unable to open camera\n";
		return -1;
	}
#else
     VideoCapture cap;
    cap.open("/home/cuicheng/work/src/opencvyi/video/test.avi");
    if (!cap.isOpened()) {
		std::cerr << "ERROR! Unable to open video\n";
		return -1;
	}
#endif
   
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
