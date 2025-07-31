//
// Created by Jack Yu on 20/09/2017.
//

#ifndef HYPERPR_PLATEDETECTION_H
#define HYPERPR_PLATEDETECTION_H

#include "PlateInfo.h"
#include <opencv2/opencv.hpp>
#include <vector>
namespace pr1 {
class PlateDetection {
public:
  PlateDetection(std::string filename_cascade);
  PlateDetection();
  void LoadModel(std::string filename_cascade);
  void plateDetectionRough(cv::Mat InputImage,
                           std::vector<pr1::PlateInfo> &plateInfos,
                           int min_w = 36, int max_w = 800);
  //        std::vector<pr1::PlateInfo> plateDetectionRough(cv::Mat
  //        InputImage,int min_w= 60,int max_h = 400);

  //        std::vector<pr1::PlateInfo>
  //        plateDetectionRoughByMultiScaleEdge(cv::Mat InputImage);

private:
  cv::CascadeClassifier cascade;
};

} // namespace pr1

#endif // HYPERPR_PLATEDETECTION_H
