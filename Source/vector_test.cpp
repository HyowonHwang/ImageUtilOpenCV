#include "vector_test.h"
#include <opencv2/opencv.hpp>

void test_vector() {
  cv::Vec <int,2> v1(5,12);
  cv::Vec <double, 3> v2(40, 130.7, 125.6);
  cv::Vec2b v3(10, 10);
  cv::Vec6f v4(40.f, 230.5f, 526.f);
  cv::Vec3i v5(200, 230, 250);

  cv::Vec3d v6 = v2 + (cv::Vec3d)v5;
  cv::Vec2b v7 = (cv::Vec2b)v1 + v3;

  std::cout<<"[v3] =" << v3 << std::endl;
  std::cout<<"[v7] =" << v7 << std::endl;
  std::cout<<"[v3 * v7] =" << v3.mul(v7) << std::endl;
}
