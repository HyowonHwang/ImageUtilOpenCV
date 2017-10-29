#ifndef _OCR_OPERATION_H
#define _OCR_OPERATION_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class OcrOperation {
  public:
    OcrOperation();
    OcrOperation(std::string path);
    ~OcrOperation();
    void detect();
  private:
    cv::Mat preprocessing(cv::Mat image);
    void find_letters(cv::Mat img, cv::Mat& org_image);

    std::string path_;
};

#endif //_OCR_OPERATION_H
