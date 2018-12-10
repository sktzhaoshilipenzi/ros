/**
 * Robomaster Vision program of Autocar
 * Copyright (c) 2018, Xidian University Robotics Vision group.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
 * documentation files(the "Software"), to deal in the Software without restriction.
 */

#ifndef CAMERA_DRIVER_H
#define CAMERA_DRIVER_H

#include <opencv2/opencv.hpp>

namespace autocar
{
namespace driver
{
/**
 * @brief: 摄像头配置类型
 */
class camera_driver
{
public:
    /**
     * @brief: 读入配置文件
     */
    camera_driver()
    {
        camera_mat_file = "/home/cuicheng/work/src/opencvyi/config/480P_120_RH.xml";
<<<<<<< HEAD
            cv::FileStorage fs(camera_mat_file, cv::FileStorage::READ);
    if(!fs.isOpened())
        std::cout << "Cannot open [" << camera_mat_file << "], please check if the file is exist." << std::endl;
    cv::FileNode root = fs.root();
    
    // driver_mul
    cv::FileNode driver_mul = root["driver_mul"];
    driver_mul["Camera_Matrix"] >> camera_matrix;
    driver_mul["Distortion_Coefficients"] >> dist_coeffs;
    driver_mul["ptz_camera_x"] >> ptz_camera_x;
    driver_mul["ptz_camera_y"] >> ptz_camera_y;
    driver_mul["ptz_camera_z"] >> ptz_camera_z;
    driver_mul["z_scale"] >> z_scale;
    
    driver_mul["barrel_ptz_offset_y"] >> barrel_ptz_offset_y;
    std::cout << "Camera_Matrix Size: " << camera_matrix.size() << std::endl;
    std::cout << "Distortion_Coefficients Size: " << dist_coeffs.size() << std::endl;

    // double barrel_ptz_offset_y = 0;
    const double overlap_dist = 100000.0;
    double theta = -atan((ptz_camera_y + barrel_ptz_offset_y)/overlap_dist);
    double r_data[] = {1,0,0,0,cos(theta),-sin(theta), 0, sin(theta), cos(theta)};
    double t_data[] = {0, ptz_camera_y, ptz_camera_z}; // ptz org position in camera coodinate system
    
    cv::Mat t(3,1, CV_64FC1, t_data);
    cv::Mat r(3,3, CV_64FC1, r_data); // Mat::eye(3, 3, CV_64FC1);
    t.copyTo(t_camera_ptz);
    r.copyTo(r_camera_ptz);
=======
>>>>>>> def115a31aa4ffdd4f2f5f7e9bd63a0e42d21992
    }
    /**
     * @brief Read vision paramaters, such as camera matrix, Distortion matrix and so on.
    */
    bool open_camera(cv::VideoCapture& ); 
    /**
     * @brief 矫正摄像头与云台的打击点
    */
    void set_camera_pose();
public:
    // 相机参数
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;
    
    std::string camera_mat_file;
    int exposure_time;
    std::string usb_cam_id;
    // 相机安装
    double ptz_camera_x;
    double ptz_camera_y;
    double ptz_camera_z;

    cv::Mat t_camera_ptz;
    cv::Mat r_camera_ptz;
    double barrel_ptz_offset_y;
    double z_scale;
};

} // namespace driver
} // namespace autocar

#endif // VISION_PARAM_H