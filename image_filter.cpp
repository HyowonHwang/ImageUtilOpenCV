#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "image_filter.h"


void ImageFilter::filter(cv::Mat img, cv::Mat& dst, cv::Mat mask) {
  dst = cv::Mat(img.size(), CV_32F, cv::Scalar(0));
  cv::Point h_m = mask.size() / 2;

  for(int i = h_m.y; i < img.rows - h_m.y; i++) {
    for( int j = h_m.x; j <img.cols - h_m.x; j++) {
      float sum = 0;
      for( int u = 0; u < mask.rows; u++){
        for( int v = 0; v < mask.cols; v++ ) {
          int y = i + u - h_m.y;
          int x = j + v - h_m.x;
          sum += mask.at<float>(u, v) * img.at<unsigned char>(y, x);
        }
      }
      dst.at<float>(i,j) = sum;
    }
  }
}

void ImageFilter::blurr(std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  // 3X3
  /* 
  float data[] = {
    1 / 9.f,  1 / 9.f, 1 / 9.f,
    1 / 9.f,  1 / 9.f, 1 / 9.f,
    1 / 9.f,  1 / 9.f, 1 / 9.f
  };
  */

  float data[] = {
    1 / 25.f,  1 / 25.f, 1 / 25.f, 1 / 25.f, 1 / 25.f,
    1 / 25.f,  1 / 25.f, 1 / 25.f, 1 / 25.f, 1 / 25.f,
    1 / 25.f,  1 / 25.f, 1 / 25.f, 1 / 25.f, 1 / 25.f,
    1 / 25.f,  1 / 25.f, 1 / 25.f, 1 / 25.f, 1 / 25.f,
    1 / 25.f,  1 / 25.f, 1 / 25.f, 1 / 25.f, 1 / 25.f,
  };

  cv::Mat mask(5, 5, CV_32F, data);
  cv::Mat blur;

  ImageFilter::filter(image, blur, mask);
  blur.convertTo(blur, CV_8U);

  cv::imshow("image", image);
  cv::imshow("blur", blur);

  cv::waitKey();
}

void ImageFilter::sharpening(std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  // 3X3
  float data1[] = {
    0, -1, 0,
    -1, 5,-1,
    0, -1, 0
  };

  float data2[] = {
    -1, -1, -1,
    -1, 9, -1,
    -1, -1, -1
  };

  cv::Mat mask1(3, 3, CV_32F, data1);
  cv::Mat sharpen1; 

  cv::Mat mask2(3, 3, CV_32F, data2);
  cv::Mat sharpen2; 

  ImageFilter::filter(image, sharpen1, mask1);
  ImageFilter::filter(image, sharpen2, mask2);

  sharpen1.convertTo(sharpen1, CV_8U);
  sharpen2.convertTo(sharpen2, CV_8U);

  cv::imshow("image", image);
  cv::imshow("sharpen1", sharpen1);
  cv::imshow("sharpen2", sharpen2);

  cv::waitKey();
}
