#include <vector>
#include <iostream>

#include "edge_detection.h"
#include "image_filter.h"

void EdgeDetection::detection(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  cv::Mat edge1, edge2, robert_edge, prewitt_edge;

  float robert_data1[] = {
    -1, 0, 0,
    0, 1, 0,
    0, 0, 0
  };

  float robert_data2[] = {
    0, 0, -1,
    0, 1, 0,
    0, 0, 0
  };

  float prewitt_data1[] = {
    -1, 0, 1,
    -1, 0, 1,
    -1, 0, 1
  };

  float prewitt_data2[] = {
    -1, -1, -1,
    0, 0, 0,
    1, 1, 1 
  };


  homogenOp(image, edge1, 3);
  differOp(image, edge2, 3);
  robert_differential(image, robert_edge, robert_data1, robert_data2);
  prewitt_differential(image, prewitt_edge, prewitt_data1, prewitt_data2);

  cv::imshow("image", image);
  cv::imshow("edge-homogenOp", edge1);
  cv::imshow("edge-differOp", edge2);
  cv::imshow("edge-robert_edge", robert_edge);
  cv::imshow("edge-prewitt", prewitt_edge);
  cv::waitKey();
}

// 유사도 검출 
void EdgeDetection::homogenity_detection(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  cv::Mat edge;
  homogenOp(image, edge, 3);

  cv::imshow("image", image);
  cv::imshow("edge-homogenOp", edge);
  cv::waitKey();
}

// 차 연산자 검출 
void EdgeDetection::difference_detection(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  cv::Mat edge;
  differOp(image, edge, 3);

  cv::imshow("image", image);
  cv::imshow("edge-differOp", edge);
  cv::waitKey();
}

// Robert Edge 
void EdgeDetection::robert_differential(cv::Mat image, cv::Mat& dst, float data1[],
    float data2[]) {
  cv::Mat dst1, dst2;
  cv::Mat mask1(3, 3, CV_32F, data1);     // 입력 인수로 마스크 행렬 초기화
  cv::Mat mask2(3, 3, CV_32F, data2);

  ImageFilter::filter(image, dst1, mask1);         // 사용자 정의 회선 함수
  ImageFilter::filter(image, dst2, mask2);

  cv::magnitude(dst1, dst2, dst);         // 회선 결과 두 행렬의 크기
  dst.convertTo(dst, CV_8U);          // 윈도우 표시 위한 형변환

  dst1 = cv::abs(dst1);             // 회선 결과 음수 원소를 양수화 
  dst2 = cv::abs(dst2);
  dst1.convertTo(dst1, CV_8U);        // 윈도우 표시 위한
  dst2.convertTo(dst2, CV_8U);
}

// Prewitt Edge 
void EdgeDetection::prewitt_differential(cv::Mat image, cv::Mat& dst, float data1[],
    float data2[]) {
  cv::Mat dst1, mask1(3, 3, CV_32F, data1);
  cv::Mat dst2, mask2(3, 3, CV_32F, data2);

  cv::filter2D(image, dst1, CV_32F, mask1);   // OpenCV 제공 회선 함수
  cv::filter2D(image, dst2, CV_32F, mask2);
  cv::magnitude(dst1, dst2, dst);
  dst.convertTo(dst, CV_8U);

  cv::convertScaleAbs(dst1, dst1);        // 절대값 및 형변환 동시 수행
  cv::convertScaleAbs(dst2, dst2);
//  cv::imshow("prewitt - 수직 마스크", dst1);           // 윈도우 행렬
 // cv::imshow("prewitt - 수평 마스크", dst2);
}

void EdgeDetection::differOp(cv::Mat img, cv::Mat& dst, int mask_size) {
  dst = cv::Mat(img.size(), CV_8U, cv::Scalar(0));
  cv::Point h_m(mask_size / 2, mask_size / 2);

  for (int i = h_m.y; i < img.rows - h_m.y; i++) {
    for (int j = h_m.x; j < img.cols - h_m.x; j++) {
      std::vector<uchar> mask;
      for (int u = 0 ; u < mask_size; u++) {
        for (int v = 0; v < mask_size; v++){
          int y = i + u - h_m.y;
          int x = j + v - h_m.x;
          mask.push_back(img.at<uchar>(y,x));
        }
      }

      unsigned char max = 0;
      for (int k = 0; k < (int)mask.size() / 2; k++) {
        int start = mask[k];
        int end = mask[mask.size() - k - 1];
        unsigned char difference = cv::abs(start - end);
        if (difference > max) {
          max = difference;
        }
      }
      dst.at<unsigned char>(i, j) = max;
    }
  }
}

void EdgeDetection::homogenOp(cv::Mat img, cv::Mat& dst, int mask_size) {

  dst = cv::Mat(img.size(), CV_8U, cv::Scalar(0));
  cv::Point h_m(mask_size / 2, mask_size / 2);

  for (int i = h_m.y; i < img.rows - h_m.y; i++) {
    for (int j = h_m.x; j < img.cols - h_m.x; j++) {
      unsigned char max = 0;
      for (int u = 0; u < mask_size; u++) {
        for (int v = 0; v < mask_size; v++) {
          int y = i + u - h_m.y;
          int x = j + v - h_m.x;
          unsigned char difference = cv::abs(img.at<uchar>(i,j) - img.at<uchar>(y, x));
          if(difference > max) {
            max = difference;
          }
        }
      }
      dst.at<uchar>(i, j) = max;
    }
  }
}
