#include <iostream>
#include <string>
#include "histogram.h"

int search_valueIdx(cv::Mat hist, int bias);

void Histogram::calc_histo(const cv::Mat& image, cv::Mat& hist, int bins, int range_max) {
  int histo_size[] = { bins};
  float range[] = { 0, (float)range_max};
  int channels[] = { 0 };
  const float* ranges[] = { range };

  cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histo_size, ranges);
}

void Histogram::calc_histo2D(const cv::Mat& image, cv::Mat& hist, cv::Vec3i
    bins, cv::Vec3f ranges_, int dims_) {

  int dims = ( dims_ <= 0) ? image.channels() : dims_;
  int channels[] = { 0, 1, 2};
  int histo_size[] = { bins[0], bins[1], bins[2]};

  float range1[] = { 0, ranges_[0]};
  float range2[] = { 0, ranges_[1]};
  float range3[] = { 0, ranges_[2]};
  const float* ranges[] = { range1, range2, range3 };

  cv::calcHist(&image, 1, channels, cv::Mat(), hist, dims, histo_size, ranges);
  cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX);
}

void Histogram::draw_histo(cv::Mat hist, cv::Mat &hist_img, cv::Size size) {
  hist_img = cv::Mat(size, CV_8U, cv::Scalar(255));
  float bin = (float)hist_img.cols / hist.rows;
  cv::normalize(hist, hist, 0, hist_img.rows, cv::NORM_MINMAX);

  for( int i = 0; i < hist.rows; i++ ) {
    float start_x = i * bin;
    float end_x = (i+1) * bin;
    cv::Point2f pt1(start_x, 0);
    cv::Point2f pt2(end_x, hist.at<float>(i));

    if(pt2.y > 0){
      cv::rectangle(hist_img, pt1, pt2, cv::Scalar(0), -1);
    }
  }
  cv::flip(hist_img, hist_img, 0);
}

cv::Mat Histogram::draw_histo2D(cv::Mat hist) {
  if(hist.dims != 2) {
    std::cerr << "not 2dimens data" << std::endl;
  }

  float ratio_value = 512.f;
  float ratio_hue = 180.f / hist.rows;
  float ratio_sat = 256.f / hist.cols;

  cv::Mat graph(hist.size(), CV_32FC3);
  for (int i = 0;  i < hist.rows; i++) {
    for (int j = 0; j < hist.cols; j++) {
      float value = hist.at<float>(i, j) * ratio_value;
      float hue = i * ratio_hue;
      float sat = j * ratio_sat;
      graph.at<cv::Vec3f>(i, j) = cv::Vec3f(hue, sat, value);
    }
  }

  graph.convertTo(graph, CV_8U);
  cv::cvtColor(graph, graph, CV_HSV2BGR);
  cv::resize(graph, graph, cv::Size(0,0), 10, 10, cv::INTER_NEAREST);

  return graph;
}


void Histogram::histogram(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  cv::Mat hist;
  cv::Mat hist_img;

  calc_histo(image, hist, 256);
  draw_histo(hist, hist_img);

  cv::imshow("image", image);
  cv::imshow("histogram", hist_img);
  cv::waitKey();
}

void Histogram::stretch_histogram(const std::string filename) {
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE); 
  cv::Mat hist, hist_dst, hist_img, hist_dst_img;
  int histsize = 64;
  int ranges = 256;
  calc_histo(image, hist,histsize, ranges);

  float bin_width = (float)ranges / histsize;
  int low_value = (int)(search_valueIdx(hist, 0) * bin_width);
  int high_value = (int)(search_valueIdx(hist, hist.rows-1) * bin_width);

  std::cout << "high_value" << high_value << std::endl;
  std::cout << "low_value" << low_value << std::endl;

  int d_value = high_value - low_value;
  cv::Mat dst = (image - low_value) * (255.0 / d_value);

  calc_histo(dst, hist_dst, histsize, ranges);
  draw_histo(hist, hist_img);
  draw_histo(hist_dst, hist_dst_img); 

  cv::imshow("image", image);
  cv::imshow("hist_imag", hist_img);
  cv::imshow("dst", dst);
  cv::imshow("hist_dst_img", hist_dst_img);
  cv::waitKey();

}

int search_valueIdx(cv::Mat hist, int bias=0) {
  for(int i = 0; i < hist.rows; i++) {
    int idx = cv::abs(bias -i);
    if(hist.at<float>(idx) > 0) {
      return idx;
    }
  }
  return -1;
}
