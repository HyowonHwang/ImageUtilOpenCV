#ifndef _IMAGE_FILTER_H_
#define _IMAGE_FILTER_H_

#include <opencv2/opencv.hpp>

class ImageFilter {
  public:
    static void blurr(std::string filename);
    static void sharpening(std::string filename);
  private:
    static void filter(cv::Mat img, cv::Mat& dst, cv::Mat mask);
};

#endif //_IMAGE_FILTER_H_
