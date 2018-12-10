#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
namespace enc = sensor_msgs::image_encodings;  
      
    /*准备再次发布的图像显示到本窗口*/  
    //static const char OUT_WINDOW[] = "Image Out";  
    /*读取订阅的图像并显示到本窗口*/  
    //static std::string IN_WINDOW = "Image In";  
      
    class ImageConvertor  
    {   sensor_msgs::ImagePtr msgs;  
        ros::NodeHandle nh_;  
        image_transport::ImageTransport it_;  
        image_transport::Subscriber image_sub_;  
        image_transport::Publisher image_pub_;  
        
        public:  
        ImageConvertor():it_(nh_){  
            /*发布主题out*/  
            image_pub_ = it_.advertise("after_process", 1); 
                    /*订阅主题camera/image*/  
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
        {  static  long cnt=0;
            cv_bridge::CvImagePtr cv_ptr;  
           
           
            try  
            {  
                 /*转化成CVImage*/  
                         cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);  
                         cv::imshow("origin",cv_ptr->image);
                         cv::waitKey(3);
            }  
      
            catch (cv_bridge::Exception& e)  
            {  
                ROS_ERROR("cv_bridge exception is %s", e.what());  
                return;  
            }  
      
          //  if(cv_ptr->image.rows > 10 && cv_ptr->image.cols > 10)  
             // cv::circle(cv_ptr->image, cv::Point(10,10), 10, CV_RGB(255,0,0));  
    cv::Mat color_light,gray_img_;
	std::vector<cv::Mat> bgr_channel;
	cv::split(cv_ptr->image, bgr_channel);
	int enemy_color,light_threshold_val,blue_color_diff,red_color_diff;
    bool ifget4 = ros::param::get("enemy_color",enemy_color);//0是红色
    bool ifget5 = ros::param::get("light_threshold_val",light_threshold_val);
    bool ifget6 = ros::param::get("blue_color_diff",blue_color_diff);
    bool ifget7 = ros::param::get("red_color_diff",red_color_diff);
  if (cnt==0) 
     { if (ifget4==1&&ifget5==1&ifget6==1&ifget7==1)
       printf ("敌方模式读取成功");
    else printf ("敌方模式读取失败");cnt++;}
	if (enemy_color == 0)
		cv::subtract(bgr_channel[2], bgr_channel[1], color_light);
	else
		cv::subtract(bgr_channel[0], bgr_channel[1], color_light);

  	cv::Mat binary_brightness_img; // 亮度二值化
  	cv::Mat binary_color_img;      // 颜色二值化
  	cv::Mat binary_light_img;      // &
  	
  	cv::cvtColor(cv_ptr->image, gray_img_, cv::ColorConversionCodes::COLOR_BGR2GRAY);
  	// TODO(noah.guo): param
	// cv::adaptiveThreshold(gray_img_, binary_brightness_img, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,7,5);
    // 环境亮度是显著因素之一
  	cv::threshold(gray_img_, binary_brightness_img, light_threshold_val, 255, CV_THRESH_BINARY);  //200
  	//TODO(noah.guo): param
  	float thresh;
 	if (enemy_color == 1) // 这里对快速移动依然有影响
   	 	thresh = blue_color_diff;
  	else
    	thresh = red_color_diff;  //50
	//  cv::adaptiveThreshold(color_light, binary_color_img, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 5);
  	cv::threshold(color_light, binary_color_img, thresh, 255, CV_THRESH_BINARY);
    // 这里的形态学需要考虑一下,尤其是装甲板快速移动时
  	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  	cv::dilate(binary_color_img, binary_color_img, element, cv::Point(-1, -1), 1);
    //cv::morphologyEx(binary_color_img,binary_color_img, MORPH_OPEN, element);
  	binary_light_img = binary_color_img & binary_brightness_img;
      msgs = cv_bridge::CvImage(std_msgs::Header(), "mono8",binary_light_img ).toImageMsg();  
            //cv::imshow(IN_WINDOW, cv_ptr->image);  
         //   imshow("jieguo",binary_light_img);
          //  cv::Mat img_out;  
    //cv_ptr->image=binary_light_img;
                    /*convert RGB to GRAY*/  
       //             cv::cvtColor(cv_ptr->image,img_out ,CV_RGB2GRAY); 
         //  imshow("jieguo",cv_ptr->image);
            //cv::imshow(OUT_WINDOW, img_out);  
            //cv::imshow("node_b talker to node_c2",img_out);
            cv::waitKey(3);  
                    /*转化成ROS图像msg发布到主题out*/  
            image_pub_.publish(msgs);  

        }  
      
      
    };  
      
int main(int argc, char** argv)  
    {  
        ros::init(argc, argv, "imagelistener");  
        ImageConvertor ic;  
        ros::spin();  
          
        return 0;  
    }
 
