#include <iostream>
#include "vector_test.h"
#include "scalar_test.h"
#include "mat_operator.h"
#include "read_image.h"
#include "read_video.h"
#include "histogram.h"
#include "image_merge.h"
#include "deblurring.h"
#include "image_filter.h"
#include "edge_detection.h"

int main(int argc, char** argv) {
  std::cout << "----------vector----------" << std::endl;
  test_vector();
  std::cout << "----------scalar----------" << std::endl;
  test_scalar();
  std::cout << "----------mat_operator----------" << std::endl;
  test_mat_operator();

  if(argv[1] != nullptr) {
    std::string option(argv[1]);
    std::cout << "option " << option << std::endl;

    if(argv[2] != nullptr) {
      std::string filename(argv[2]);
      if(option == "image") {
        std::cout << "----------image_read----------" << std::endl;
        test_read_image(filename);
      } else if ( option == "video") {
        std::cout << "----------video_read----------" << std::endl;
        test_read_video(filename);
      } else if ( option == "histogram") {
        std::cout << "----------histogram----------" << std::endl;
        Histogram::histogram(filename);
      } else if ( option == "histogram_strecth") {
        std::cout << "----------histogram----------" << std::endl;
        Histogram::stretch_histogram(filename);
      } else if ( option == "image_merge" && argv[3] != nullptr) {
        std::cout << "----------image_merge----------" << std::endl;
        std::string img2_file_name(argv[3]);
        test_image_merge(filename,img2_file_name);
      } else if ( option == "blurr") {
        std::cout << "----------blurr----------" << std::endl;
        ImageFilter::blurr(filename);
      } else if ( option == "sharpen") {
        std::cout << "----------blurr----------" << std::endl;
        ImageFilter::sharpening(filename);
      } else if ( option == "edge_detection") {
        std::cout << "----------edge detection----------" << std::endl;
        EdgeDetection::detection(filename);
      } else if ( option == "color_edge_detection") {
        std::cout << "----------edge detection----------" << std::endl;
        EdgeDetection::color_edge_detection(filename);
      } else if ( option == "deburr") {
        std::cout << "----------deburr----------" << std::endl;
        deblurring(filename);
      }
    }
  }
  return 0;
}
