#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "histogram.h"

void put_string(cv::Mat &frame, std::string text, cv::Point pt, int value) {
		text += std::to_string(value);

		cv::Point shade = pt + cv::Point(2, 2);
		int font = cv::FONT_HERSHEY_SIMPLEX;
		cv::putText(frame, text, shade, font, 0.7, cv::Scalar(0,0,0), 2);
		cv::putText(frame, text, pt, font, 0.7, cv::Scalar(120, 200,90), 2);
}

void test_read_video(const std::string filename) {
		cv::VideoCapture capture;
		capture.open(filename);

		double frame_rate = capture.get(CV_CAP_PROP_FPS);
		std::cout << "frame_rate : " << frame_rate << std::endl;
		int delay = 1000 / frame_rate;
		int frame_count = 0;
		cv::Mat frame;

		while( capture.read(frame) ) {
				if(cv::waitKey(delay) >= 0) {
						break;
				}
				put_string(frame, "frame_count ", cv::Point(20,50), frame_count++);
				cv:imshow("file read ", frame);
		}
}
