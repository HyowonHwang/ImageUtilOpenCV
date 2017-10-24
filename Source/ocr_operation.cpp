#include <iostream>
#include "ocr_operation.h"

OcrOperation::OcrOperation() {
}

OcrOperation::OcrOperation(std::string path) : path_(path) {
}

OcrOperation::~OcrOperation() {
}

void OcrOperation::detect() {
  cv::Mat image = imread(path_, cv::IMREAD_COLOR);
  CV_Assert(image.data);
  cv::Mat gray = preprocessing(image);
  find_letters(gray);

  cv::imshow("org", image);
  cv::imshow("letter", gray);
  cv::imwrite("/tmp/grag.jpg", gray);
  cv::waitKey();
}

cv::Mat OcrOperation::preprocessing(cv::Mat image) {
  cv::Mat gray, th_img;
  cv::cvtColor(image, gray, CV_BGR2GRAY); 
//  cv::GaussianBlur(gray, gray, cv::Size(7, 7), 2, 2);

//  cv::threshold(gray, th_img, 127, 255, cv::THRESH_BINARY);
  cv::adaptiveThreshold(gray, th_img, 255, cv::ADAPTIVE_THRESH_MEAN_C,
      cv::THRESH_BINARY_INV, 3,12);
//  cv::morphologyEx(th_img, th_img, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 1);
  return th_img;
}

void OcrOperation::find_letters(cv::Mat& img) {
  std::vector<std::vector<cv::Point>> contours;
  cv::Mat hieracy;

  cv::findContours(img.clone(), contours, hieracy,  cv::RETR_EXTERNAL,
      cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

  for(int idx = 0; idx < contours.size(); idx++) {
    cv::Rect rect = cv::boundingRect(contours[idx]);

    if (rect.height > 10 && rect.width > 40 && !(rect.width >= 512 - 5
          && rect.height >= 512 - 5)){
      std::cout << "find_letter" << std::endl;
      cv::rectangle(img, rect, cv::Scalar(255,0,0),2);
    }
  }
}
