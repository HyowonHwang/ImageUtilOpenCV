#include <iostream>
#include <opencv2/opencv.hpp>
#include "read_image.h"

void print_matInfo(std::string name, cv::Mat img) {
  std::string mat_type;

  if(img.depth() == CV_8U) {
    mat_type = "CV_8U";
  } else if(img.depth() == CV_8S) {
    mat_type = "CV_8S";
  } else if(img.depth() == CV_16U) {
    mat_type = "CV_16U";
  } else if(img.depth() == CV_16S) {
    mat_type = "CV_16S";
  } else if(img.depth() == CV_32S) {
    mat_type = "CV_32S";
  } else if(img.depth() == CV_64F) {
    mat_type = "CV_64F";
  }

  std::cout << name;
  std::cout << ":depth : " <<  mat_type << " channels: " << img.channels() << std::endl;
}

void test_read_image(const std::string filename) {

  cv::Mat gray2gray = imread(filename, cv::IMREAD_GRAYSCALE);
  cv::Mat gray2color = imread(filename, cv::IMREAD_COLOR);
  cv::Mat gray2unchanged = imread(filename, cv::IMREAD_UNCHANGED);

  cv::Rect roi(100, 100, 1,1);

  std::cout << "unchagned (100,100) pixel value : " << gray2unchanged(roi) << std::endl;

  print_matInfo("gray2gray", gray2gray);
  print_matInfo("gray2color", gray2color);
  print_matInfo("gray2unchanged", gray2unchanged);
  cv::imshow("gray2gray", gray2gray);
  cv::imshow("gray2color", gray2color);
  cv::imshow("gray2unchanged", gray2color);
  cv::waitKey(0);
}
