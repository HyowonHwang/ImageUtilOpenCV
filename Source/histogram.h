#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include <opencv2/opencv.hpp>

class Histogram {
  public:
    static void histogram(const std::string filename);
    static void stretch_histogram(const std::string filename);
    static void calc_histo(const cv::Mat& image, cv::Mat& hist, int
        bins, int range_max = 256); 
    static void calc_histo2D(const cv::Mat& image, cv::Mat& hist, cv::Vec3i 
        bins, cv::Vec3f ranges_, int dims_ = 0); 
   static void draw_histo(cv::Mat hist, cv::Mat &hist_img, cv::Size size =
       cv::Size(256,200));
   static cv::Mat draw_histo2D(cv::Mat hist);
};

#endif //_HISTOGRAM_H_
