#ifndef VIDEO_RECODER_H
#define VIDEO_RECODER_H

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

namespace autocar
{
namespace vision_mul
{
    class video_recoder {
    public:
        video_recoder(std::string video_dir_="/home/cuicheng/work/src/opencvyi/video", int cols_=640, int rows_=480);
        ~video_recoder();
        void save_frame(const cv::Mat &image);

    private:
        cv::VideoWriter writer;
        int rows;
        int cols;
    };
}
}

#endif // VIDEO_RECODER_H

