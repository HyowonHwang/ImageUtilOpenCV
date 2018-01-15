#include <iostream>
#include <string>
#include <list>
#include <opencv2/opencv.hpp>
#include "image_merge.h"


void test_image_merge(const std::string img1, const std::string img2) {
  cv::Mat image1 = cv::imread(img1, cv::IMREAD_GRAYSCALE);
  cv::Mat image2 = cv::imread(img2, cv::IMREAD_GRAYSCALE);

  double alpha = 0.6, beta = 0.7;
  cv::Mat add_img1 = image1 + image2; 
  cv::Mat add_img2 = image1 * 0.5 + image2 * 0.5;
  cv::Mat add_img3 = image1 * alpha + image2 * (1 - alpha);

  cv::Mat add_img4;

  cv::addWeighted(image1, alpha, image2, beta, 0, add_img4);

  cv::imshow("image1", image1); 
  cv::imshow("image2", image2); 
  cv::imshow("add_imag1", add_img1); 
  cv::imshow("add_imag2", add_img2); 
  cv::imshow("add_imag3", add_img3); 
  cv::imshow("add_imag4", add_img4); 

  cv::waitKey(0);
}

void test_image_concat(const std::list<std::string> file_list) {
  for(auto file_path : file_list) {
    std::cout << file_path << std::endl;
  }
}
