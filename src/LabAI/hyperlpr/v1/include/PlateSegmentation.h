#ifndef HYPERPR_PLATESEGMENTATION_H
#define HYPERPR_PLATESEGMENTATION_H

#include "PlateInfo.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

namespace pr1 {

class PlateSegmentation {
public:
  const int PLATE_NORMAL = 6;
  const int PLATE_NORMAL_GREEN = 7;
  const int DEFAULT_WIDTH = 20;
  PlateSegmentation(std::string phototxt, std::string caffemodel);
  PlateSegmentation() {}
  void segmentPlatePipline(PlateInfo &plateInfo, int stride,
                           std::vector<cv::Rect> &Char_rects);

  void segmentPlateBySlidingWindows(cv::Mat &plateImage, int windowsWidth,
                                    int stride, cv::Mat &respones);
  void templateMatchFinding(const cv::Mat &respones, int windowsWidth,
                            std::pair<float, std::vector<int>> &candidatePts);
  void refineRegion(cv::Mat &plateImage, const std::vector<int> &candidatePts,
                    const int padding, std::vector<cv::Rect> &rects);
  void ExtractRegions(PlateInfo &plateInfo, std::vector<cv::Rect> &rects);
  cv::Mat classifyResponse(const cv::Mat &cropped);

private:
  cv::dnn::Net net;
};

} // namespace pr1

#endif // HYPERPR_PLATESEGMENTATION_H
