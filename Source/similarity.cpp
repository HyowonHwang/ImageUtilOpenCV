#include <iostream>
#include <string>
#include "similarity.h"
#include "histogram.h"

cv::Mat load_hist(std::string filename, cv::Vec3i bins, cv::Vec3f ranges) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
  cv::Mat hsv, hist;

  cv::cvtColor(image, hsv, CV_BGR2HSV);
  Histogram::calc_histo2D(hsv, hist, bins, ranges, 2);
 
  cv::imshow("original" + filename, image);
  return hist;
}

double Similarity::calc_similarity(const std::string target, const std::string compare) {

  cv::Vec3i bins(30, 40, 0);
  cv::Vec3f ranges(180, 256, 0);

  cv::Mat targetImage = load_hist(target, bins, ranges);
  cv::Mat compareImage = load_hist(compare, bins, ranges);

  cv::Mat draw_target = Histogram::draw_histo2D(targetImage);
  cv::Mat draw_compare = Histogram::draw_histo2D(compareImage);

  double similarity = cv::compareHist(targetImage, compareImage, CV_COMP_CORREL);

  std::cout << "result " << similarity << std::endl;

  cv::imshow("hist-" + target, draw_target);
  cv::imshow("hist-" + compare, draw_compare);
  cv::waitKey();
}


