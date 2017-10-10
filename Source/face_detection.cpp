#include <iostream>
#include "face_detection.h"

const std::string face_cascade_path("haarcascade_frontalface_alt2.xml");
const std::string lpface_cascade_path("lbpcascade_frontalface.xml");
const std::string eyes_cascade_path("haarcascade_eye.xml");

FaceDetection::FaceDetection(std::string base_path, std::string image_path) :
  base_path_(base_path),
  image_path_(image_path) {
}

FaceDetection::~FaceDetection() {}
     
bool FaceDetection::load_cascade(cv::CascadeClassifier& cascade, const std::string
    filename) {
  if(valid_base_path()) {
    std::string full_name = base_path_ + filename;
    CV_Assert(cascade.load(full_name));
  }
  return false;
}

cv::Mat FaceDetection::preprocessing(cv::Mat image) {
  cv::Mat gray;
  cv::cvtColor(image, gray, CV_BGR2GRAY); 
  cv::equalizeHist(gray, gray);
  return gray;
}


cv::Point2d FaceDetection::calc_center(cv::Rect obj) {
  cv::Point2d c = (cv::Point2d)obj.size() / 2.0;
  cv::Point2d center = (cv::Point2d)obj.tl() + c;
  return center;
}


void FaceDetection::detect() {
  if(!valid_base_path() || image_path_.empty()) {
    return;
  }

  cv::CascadeClassifier face_cascade, eyes_cascade;
  //load_cascade(face_cascade, face_cascade_path);
  load_cascade(face_cascade, lpface_cascade_path);
  load_cascade(eyes_cascade, eyes_cascade_path);

  cv::Mat image = imread(image_path_, cv::IMREAD_COLOR);
  CV_Assert(image.data);
  cv::Mat gray = preprocessing(image);

  std::vector<cv::Rect> faces, eyes;
  std::vector<cv::Point2d> eyes_center;

  face_cascade.detectMultiScale(gray, faces, 1.1, 2,
      cv::CASCADE_DO_CANNY_PRUNING, cv::Size(100,100)); 
  /*
  face_cascade.detectMultiScale(gray, faces, 1.1, 2,
      cv::CASCADE_SCALE_IMAGE, cv::Size(100,100)); 

      */

  if(faces.size() > 0) {
    std::cout << "face detected" << std::endl;
    eyes_cascade.detectMultiScale(gray(faces[0]), eyes, 1.15, 7, 0,
        cv::Size(25, 20));
    if(eyes.size() == 2) {
      cv::Point2d center;
      for(int i = 0; i < eyes.size(); i++) {
        center = calc_center(eyes[i] + faces[0].tl());
        cv::circle(image, center, 5, cv::Scalar(0, 255, 0),2);
        eyes_center.push_back(center);
      }
      std::cout << "rotate : " << angle_face(eyes_center) << std::endl;
    }

    cv::rectangle(image, faces[0], cv::Scalar(255, 0, 0), 2);
    cv::imshow("image", image);
    output_face(faces, 250);
    cv::waitKey();
  }
}

void FaceDetection::output_face(std::vector<cv::Rect> faces, int
    size) {
  cv::Mat img = cv::imread(image_path_);
  for(auto rect : faces) {
    cv::Mat cropped_img = img(rect);
    cv::resize(cropped_img, cropped_img, cv::Size(size, size), 0, 0, cv::INTER_NEAREST);
    cv::imshow("cropped" , cropped_img);
  }
}

double FaceDetection::angle_face(std::vector<cv::Point2d>  pt) {
  cv::Point2f delta = (pt[0].x > pt[1].x) ? pt[0] - pt[1] : pt[1] - pt[0];
  return cv::fastAtan2(delta.y, delta.x);
}

bool FaceDetection::valid_base_path() {
  return !base_path_.empty();
}
