#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "histogram.h"

int search_valueIdx(cv::Mat hist, int bias);

void calc_histo(const cv::Mat& image, cv::Mat& hist, int bins, int range_max =
    256) {
  int histo_size[] = { bins};
  float range[] = { 0, (float)range_max};
  int channels[] = { 0 };
  const float* ranges[] = { range };

  cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histo_size, ranges);
}

void draw_histo(cv::Mat hist, cv::Mat &hist_img, cv::Size size = cv::Size(256,200)) {
  hist_img = cv::Mat(size, CV_8U, cv::Scalar(255));
  float bin = (float)hist_img.cols / hist.rows;
  cv::normalize(hist, hist, 0, hist_img.rows, cv::NORM_MINMAX);

  for( int i = 0; i < hist.rows; i++ ) {
    float start_x = i * bin;
    float end_x = (i+1) * bin;
    cv::Point2f pt1(start_x, 0);
    cv::Point2f pt2(end_x, hist.at<float>(i));

    if(pt2.y > 0){
      cv::rectangle(hist_img, pt1, pt2, cv::Scalar(0), -1);
    }
  }
  cv::flip(hist_img, hist_img, 0);
}

void test_histogram(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  cv::Mat hist;
  cv::Mat hist_img;

  calc_histo(image, hist, 256);
  draw_histo(hist, hist_img);

  cv::imshow("image", image);
  cv::imshow("histogram", hist_img);
  cv::waitKey();
}

void test_stretch_histogram(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE); 
  cv::Mat hist, hist_dst, hist_img, hist_dst_img;
  int histsize = 64;
  int ranges = 256;
  calc_histo(image, hist,histsize, ranges);

  float bin_width = (float)ranges / histsize;
  int low_value = (int)(search_valueIdx(hist, 0) * bin_width);
  int high_value = (int)(search_valueIdx(hist, hist.rows-1) * bin_width);

  std::cout << "high_value" << high_value << std::endl;
  std::cout << "low_value" << low_value << std::endl;

  int d_value = high_value - low_value;
  cv::Mat dst = (image - low_value) * (255.0 / d_value);

  calc_histo(dst, hist_dst, histsize, ranges);
  draw_histo(hist, hist_img);
  draw_histo(hist_dst, hist_dst_img); 

  cv::imshow("image", image);
  cv::imshow("hist_imag", hist_img);
  cv::imshow("dst", dst);
  cv::imshow("hist_dst_img", hist_dst_img);
  cv::waitKey();

}

int search_valueIdx(cv::Mat hist, int bias=0) {
  for(int i = 0; i < hist.rows; i++) {
    int idx = cv::abs(bias -i);
    if(hist.at<float>(idx) > 0) {
      return idx;
    }
  }
  return -1;
}
