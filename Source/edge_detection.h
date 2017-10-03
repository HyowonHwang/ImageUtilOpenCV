#ifndef _EDGE_DETECTION_H_
#define _EDGE_DETECTION_H_

#include <opencv2/opencv.hpp>

class EdgeDetection {
  public:
    static void detection(const std::string filename);
    static void homogenity_detection(const std::string filename);
    static void difference_detection(const std::string filename);
  private:
    static void robert_differential(cv::Mat image, cv::Mat& dst, float data1[],
        float data2[]);
    static void prewitt_differential(cv::Mat image, cv::Mat& dst, float data1[], float data2[]);

    static void homogenOp(cv::Mat img, cv::Mat& dst, int mask_size);
    static void differOp(cv::Mat img, cv::Mat& dst, int mask_size);
};

#endif //_EDGE_DETECTION_H_
