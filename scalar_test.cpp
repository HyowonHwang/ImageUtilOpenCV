#include <iostream>
#include <opencv2/opencv.hpp>


void test_scalar() {
  cv::Scalar_ <uchar> red(0, 0, 255);
  cv::Scalar_ <int> blue(255, 0, 0);
  cv::Scalar_ <double> color1(500);
  cv::Scalar_ <float> color2(100.f, 200.f, 125.9f);


  cv::Vec3d green(0, 0, 300.5);

  std::cout << "blue = " << blue[0] << ", " << blue[1]; 
  std::cout << ", " << blue[2] <<", " <<  blue[3] << std::endl; 
  std::cout << "red = " << red << std::endl; 
}
