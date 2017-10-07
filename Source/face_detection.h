#ifndef _FACE_DETECTION_H
#define _FACE_DETECTION_H

#include <string>
#include <opencv2/opencv.hpp>

class FaceDetection {
  public:
    FaceDetection(std::string base_path, std::string image_path);
    ~FaceDetection();
     
    bool load_cascade(cv::CascadeClassifier& cascade, const std::string
        filename);

    cv::Mat preprocessing(cv::Mat image);
    cv::Point2d calc_center(cv::Rect obj);
    void detect();

  private:
    bool valid_base_path();
    std::string base_path_;
    std::string image_path_;
};

#endif //_FACE_DETECTION_H
