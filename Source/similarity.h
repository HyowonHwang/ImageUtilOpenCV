#ifndef _SIMMARITY_H_
#define _SIMMARITY_H_

#include <opencv2/opencv.hpp>

class Similarity {
  public:
    static double calc_similarity(const std::string target, const std::string compare);
};

#endif //_SIMMARITY_H_
