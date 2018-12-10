/*******************************************************************************************************************
Copyright 2017 Dajiang Innovations Technology Co., Ltd (DJI)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files(the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and 
to permit persons to whom the Software is furnished to do so, subject to the following conditions : 

The above copyright notice and this permission notice shall be included in all copies or substantial portions of
the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*******************************************************************************************************************/

#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/video/tracking.hpp"
/**
 * @brief The RectPnPSolver class
 * solve the pnp problem of rectangle target.
 */
class RectPnPSolver {
public:
    RectPnPSolver(const cv::Mat & camera_matrix, const cv::Mat & dist_coeff, double target_width = 0, double target_height = 0){
        camera_matrix.copyTo(cam_matrix);
        dist_coeff.copyTo(distortion_coeff);

        width_target = target_width;
        height_target = target_height;
    }

    void setTargetSize(double width, double height){
        width_target = width;
        height_target = height;
    }

    void setCameraParam(const cv::Mat & camera_matrix, const cv::Mat & dist_coeff){
        camera_matrix.copyTo(cam_matrix);
        dist_coeff.copyTo(distortion_coeff);
    }

    /**
     * @brief solvePnP
     * @param points2d image points set with following order : left_up, right_up, left_down, right_down
     * @param rot rotation between camera and target center
     * @param trans tanslation between camera and target center
     */
    void solvePnP4Points(const std::vector<cv::Point2f> & points2d, cv::Mat & rot, cv::Mat & trans);

public:
    cv::Mat cam_matrix;
    cv::Mat distortion_coeff;
    double width_target;
    double height_target;
};

class AngleSolver : public RectPnPSolver {
public:
    AngleSolver(const cv::Mat & camera_matrix, const cv::Mat & dist_coeff,
                double target_width = 0, double target_height = 0, double z_scale = 1.0,
                double min_dist = 50.0, double max_dist = 800.0)
        : RectPnPSolver(camera_matrix, dist_coeff, target_width, target_height){
        min_distance = min_dist;
        max_distance = max_dist;

        rot_camera2ptz = cv::Mat::eye(3,3,CV_64FC1);
        trans_camera2ptz = cv::Mat::zeros(3,1,CV_64FC1);
        offset_y_barrel_ptz = 0;
        scale_z = z_scale;
    }

    void setScaleZ(double scale) {scale_z = scale;}
    void setRelationPoseCameraPTZ(const cv::Mat & rot_camera_ptz, const cv::Mat & trans_camera_ptz, double y_offset_barrel_ptz);

    void getTarget2dPoinstion(const cv::RotatedRect & rect, std::vector<cv::Point2f> & target2d, const cv::Point2f & offset);
    void getTarget2dPoinstion_rect(const cv::Rect & rect, std::vector<cv::Point2f> & target2d, const cv::Point2f & offset);
    /**
     * @brief getAngle
     * @param rect input target rectangle

     * @param angle_x output angle on x axis
     * @param angle_y output angle on y axis
     * @param offset input rectangle offset, default (0,0)
     * @return
     */
    bool getAngle(const cv::RotatedRect & rect, double & angle_x, double & angle_y, double bullet_speed, const cv::Point2f & offset = cv::Point2f());
    bool getAngle_rect(const cv::Rect & rect, double & angle_x, double & angle_y, double bullet_speed, const cv::Point2f & offset = cv::Point2f());

    void tranformationCamera2PTZ(const cv::Mat & pos, cv::Mat & transed_pos);

    /**
     * @brief adjustPTZ2Barrel Calculate the angle the barrel should rotate to reach the point in PTZ coordinate
     * @param pos_in_ptz point in PTZ coordinate
     * @param angle_x angle of x axis the PTZ should rotate
     * @param angle_y angle of y axis the PTZ should rotate
     */
    void adjustPTZ2Barrel(const cv::Mat & pos_in_ptz, double & angle_x, double & angle_y, double bullet_speed = 0.0);

    double _distance; // target distance
public:
    cv::Mat position_in_camera;
    cv::Mat position_in_ptz;
private:
    cv::Mat trans_camera2ptz;
    cv::Mat rot_camera2ptz;

    // offset between barrel and ptz on y axis (cm)
    double offset_y_barrel_ptz;

    // scope of detection distance (cm)
    double min_distance;
    double max_distance;
    
    double scale_z;
};


class AngleSolverFactory {
public:
    AngleSolverFactory(AngleSolver * angle_solver = NULL): slover(angle_solver){
    }

    typedef enum {TARGET_RUNE, TARGET_ARMOR, TARGET_SAMLL_ARMOR } TargetType;

    void setSolver(AngleSolver * angle_slover){
        slover = angle_slover;
    }
    AngleSolver & getSolver(){
        return *slover;
    }
    void setTargetSize(double width, double height, TargetType type);

    void adjustRect2FixedRatio(cv::RotatedRect & rect, double wh_ratio){
        rect.size.height = rect.size.width / wh_ratio;
    }
    void adjustRect2FixedRatio_rect(cv::Rect & rect, double wh_ratio){
        rect.height = rect.width / wh_ratio;
    }

    bool getAngle(const cv::RotatedRect & rect, TargetType type, double & angle_x, double & angle_y, double bullet_speed, const cv::Point2f & offset = cv::Point2f());
    bool getAngle_rect(const cv::Rect & rect, TargetType type, double & angle_x, double & angle_y, double bullet_speed, const cv::Point2f & offset = cv::Point2f());
<<<<<<< HEAD
    AngleSolver * slover;
=======
>>>>>>> def115a31aa4ffdd4f2f5f7e9bd63a0e42d21992
private:
    double armor_width;
    double armor_height;
    double small_armor_width;
    double small_armor_height;
    double rune_width;
    double rune_height;
<<<<<<< HEAD
    
=======
    AngleSolver * slover;
>>>>>>> def115a31aa4ffdd4f2f5f7e9bd63a0e42d21992
};
class km {
    public:
    cv::Vec3f predict(cv::KalmanFilter &kf,cv::Vec3f last,cv::Vec3f now);
    void init (cv::KalmanFilter &kf);
    private:
    cv::Mat measurement;
    cv::Mat prediction;
    int k;  // 表示进行了几次卡尔曼 ，大于一次开始校准
    int cnt;  //丢失目标帧数，大于10初始化卡尔曼
};
