#include "camera_driver.hpp"
#include "debug_utility.hpp"
namespace autocar
{
namespace driver
{

bool camera_driver::open_camera(cv::VideoCapture & capture_camera_forward ) // 以后在拓展其他的吧
{
    std::string param_file=camera_mat_file;
   // camera_mat_file = param_file;

    cv::FileStorage fs(param_file, cv::FileStorage::READ);
    if(!fs.isOpened())
        std::cout << "Cannot open [" << param_file <<", please check if the file is exist." << std::endl;
    else 
        std::cout<<"加载摄像头参数成功"<<std::endl;
    cv::FileNode root = fs.root();
    
    cv::FileNode driver_mul = root["driver_mul"];

    driver_mul["cam_id"] >> usb_cam_id;
  
    driver_mul["exposure_time"] >> exposure_time;

    capture_camera_forward.set(CV_CAP_PROP_FPS,120);
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

    return true;
}

void camera_driver::set_camera_pose()
{
    cv::FileStorage fs(camera_mat_file, cv::FileStorage::READ);
    if(!fs.isOpened())
        std::cout << "Cannot open [" << camera_mat_file << "], please check if the file is exist." << std::endl;
    cv::FileNode root = fs.root();
    
    // driver_mul
    
}

} // namespace driver
} // namespace autocar