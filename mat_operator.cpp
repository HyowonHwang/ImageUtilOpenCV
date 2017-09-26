#include <iostream>
#include <opencv2/opencv.hpp>

void test_mat_operator() {
		float data[] = {
				1, 0, 2,
				-3, 4, 6, 
				-1, -2, 3
		};

		cv::Mat m1(3,3, CV_32FC1, data);
		cv::Mat m2(cv::Matx13f(6, 30, 8));
		cv::Mat m2_t = m2.t();

		cv::Mat m1_inv = m1.inv(cv::DECOMP_LU);
		cv::Mat x = m1_inv * m2_t; 

		std::cout << "[m1] =" << std::endl << m1 << std::endl;
		std::cout << "[m1_inv] =" << std::endl << m1_inv << std::endl;
		std::cout << "[m2_t ] =" << std::endl << m2_t << std::endl;

		std::cout << "[solution] =" << std::endl << x << std::endl;
}
