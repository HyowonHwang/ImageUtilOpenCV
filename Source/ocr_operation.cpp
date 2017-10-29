#include <iostream>
#include <baseapi.h>
#include <sys/time.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
  find_letters(gray, image);

  cv::imshow("org", image);
  cv::imshow("letter", gray);
  cv::waitKey();
}

cv::Mat OcrOperation::preprocessing(cv::Mat image) {
  cv::Mat gray, th_img;
  //cv::Mat kernel(5, 15, CV_8UC1, cv::Scalar(1));
  cv::Mat kernel(5, 25, CV_8UC1, cv::Scalar(1));
  cv::cvtColor(image, gray, CV_BGR2GRAY); 
  //cv::GaussianBlur(gray, gray, cv::Size(7, 7), 2, 2);
//  cv::blur(gray, gray, cv::Size(5,5));
//  cv::Canny(gray, gray, 100, 150); 
//  cv::Sobel(gray, gray, CV_8U, 1, 0, 3);

//  cv::threshold(gray, th_img, 120, 255, cv::THRESH_BINARY);
  cv::adaptiveThreshold(gray, th_img, 255, cv::ADAPTIVE_THRESH_MEAN_C,
     cv::THRESH_BINARY_INV, 3,12);
 // cv::morphologyEx(th_img, th_img, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 1);
// cv::morphologyEx(th_img, th_img, cv::MORPH_CLOSE, kernel);
  cv::morphologyEx(th_img, th_img, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), 2);
  return th_img;
}

void OcrOperation::find_letters(cv::Mat img, cv::Mat& org_img) {
  std::vector<std::vector<cv::Point>> contours;
  cv::Mat hieracy;
  cv::Mat org_img_dup = org_img.clone();

  cv::findContours(img.clone(), contours, hieracy,  cv::RETR_EXTERNAL,
      cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

  for(int idx = 0; idx < contours.size(); idx++) {
    cv::Rect rect = cv::boundingRect(contours[idx]);

    if (rect.height > 10 && rect.width > 40 && !(rect.width >= 512 - 5
          && rect.height >= 512 - 5)){
      std::cout << "find_letter" << std::endl;
      cv::rectangle(org_img, rect, cv::Scalar(255,0,0),2);
      find_string(org_img_dup(rect));
//      std::string filepath = "/tmp/letter"+ std::to_string(idx) + ".jpg";
//      cv::imwrite(filepath, org_img_dup(rect));
    }
  }
}


void OcrOperation::find_string(cv::Mat image) {
  // initilize tesseract OCR engine
  tesseract::TessBaseAPI *myOCR =
    new tesseract::TessBaseAPI();

  if (myOCR->Init(NULL, "kor")) {
    return;
  }


  // recognize text
  myOCR->SetImage( image.data, image.size().width, image.size().height,
      image.channels(), image.step1());
  myOCR->Recognize(0);
  const char *text1 = myOCR->GetUTF8Text();

  // remove "newline"
  std::string t1(text1);

  // print found text
  std::cout<<"found text1"<<std::endl;
  std::cout<<t1<< std::endl;

  delete [] text1;

  // destroy tesseract OCR engine
  myOCR->Clear();
  myOCR->End();
}

