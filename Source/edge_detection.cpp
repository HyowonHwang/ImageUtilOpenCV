#include <vector>
#include <iostream>

#include "edge_detection.h"
#include "image_filter.h"

int th = 50;
cv::Mat image, gray, edge;

void onTrackbar(int, void*) {
  cv::GaussianBlur(gray, edge, cv::Size(3, 3), 0.7);      // 가우시안 블러링
  cv::Canny(edge, edge, th, th * 2, 3);         // 캐니에지 수행

  cv::Mat color_edge;
  image.copyTo(color_edge, edge);           // 에지 영역만 복사
  imshow("color", color_edge);
}

void EdgeDetection::color_edge_detection(const std::string filename) {
  image = cv::imread(filename, 1);    // 컬러 영상 로드
  CV_Assert(image.data);                // 예외 처리
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);        // 명암도 변환

  cv::namedWindow("컬러 에지", 1);
  cv::createTrackbar("Canny th", "컬러 에지", &th, 100, onTrackbar);
  onTrackbar(th, 0);                  // 트랙바 함수 호출

  cv::waitKey(0);
}

void EdgeDetection::detection(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  cv::Mat edge1, edge2, robert_edge, prewitt_edge, sobel_edge, canny_edge;

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

  float sobel_data1[] = {
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1
  };

  float sobel_data2[] = {
    -1, -2, -1,
    0, 0, 0,
    1, 2, 1 
  };

  homogenOp(image, edge1, 3);
  differOp(image, edge2, 3);
  robert_differential(image, robert_edge, robert_data1, robert_data2);
  prewitt_differential(image, prewitt_edge, prewitt_data1, prewitt_data2);
  //sobel 
  prewitt_differential(image, sobel_edge, sobel_data1, sobel_data2);
  cv::Canny(image, canny_edge, 100, 150); 

  cv::imshow("image", image);
  cv::imshow("edge-homogenOp", edge1);
  cv::imshow("edge-differOp", edge2);
  cv::imshow("edge-robert_edge", robert_edge);
  cv::imshow("edge-prewitt", prewitt_edge);
  cv::imshow("edge-sobel", sobel_edge);

  //laplician
  laplacian_differential(image);

  // LOG/ DOG
  log_differential(image);
  cv::imshow("edge-canny", canny_edge);

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
}

void EdgeDetection::laplacian_differential(cv::Mat image) {
  short data1[] = {
    0, 1, 0,
    1, -4, 1,
    0, 1, 0
  };

  short data2[] = {
    -1, -1, -1,
    -1, 8, -1,
    -1, -1, -1 
  };


  cv::Mat dst1, mask1(3, 3, CV_16S, data1);
  cv::Mat dst2, mask2(3, 3, CV_16S, data2);
  cv::Mat dst3;

  cv::filter2D(image, dst1, CV_16S, mask1);   // OpenCV 제공 회선 함수
  cv::filter2D(image, dst2, CV_16S, mask2);

  cv::Laplacian(image, dst3, CV_16S, 1);

  cv::convertScaleAbs(dst1, dst1); 
  cv::convertScaleAbs(dst2, dst2);
  cv::convertScaleAbs(dst3, dst3);        // 절대값 및 형변환 동시 수행

  cv::imshow("edge-laplian4", dst1);
  cv::imshow("edge-laplian8", dst2);
  cv::imshow("edge-laplian opencv", dst3);
}

// LOG(Laplician of Guassian)
cv::Mat getLoGmask(cv::Size size, double sigma) {
  double ratio = 1 / (CV_PI * cv::pow(sigma, 4.0));
  int center = size.height / 2;
  cv::Mat dst(size, CV_64F);

  for (int i = 0; i < size.height; i++) {
    for (int j = 0; j < size.width; j++) {
      int x2 = (j - center) * (j - center);
      int y2 = (i - center) * (i - center);

      double value = (x2 + y2) / (2 * sigma * sigma);
      dst.at<double>(i, j) = -ratio * (1 - value) * exp(-value);
    }
  }

  double scale = (center * 10 / ratio);
  return dst * scale;
}

void EdgeDetection::log_differential(cv::Mat image) {
  double sigma = 1.4;
  cv::Mat LoG_mask = getLoGmask(cv::Size(9, 9), sigma);

  std::cout << LoG_mask << std::endl;
  std::cout << cv::sum(LoG_mask) << std::endl;

  cv::Mat dst1, dst2, dst3, dst4, gaus_img;
  cv::filter2D(image, dst1, -1, LoG_mask);
  cv::GaussianBlur(image, gaus_img, cv::Size(9, 9), sigma, sigma);
  cv::Laplacian(gaus_img, dst2, -1, 5);

  cv::GaussianBlur(image, dst3, cv::Size(1, 1), 0.0);
  cv::GaussianBlur(image, dst4, cv::Size(9, 9), 1.6);
  cv::Mat dst_DoG = dst3 - dst4;

  cv::normalize(dst_DoG, dst_DoG, 0, 255, CV_MINMAX);

  imshow("image", image);
  imshow("dst1 - LoG_filter2D", dst1);
  imshow("dst2 - LOG_OpenCV", dst2);
  imshow("dst_DoG - DOG_OpenCV", dst_DoG);
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
