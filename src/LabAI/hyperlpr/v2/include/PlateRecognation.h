#ifndef _PLATERECOGNATION_H_
#define _PLATERECOGNATION_H_
#include <opencv2/dnn.hpp>
#include "PlateInfo.h"
namespace pr2 {
	class PlateRecognation {
	public:
		PlateRecognation(std::string Rec_prototxt, std::string Rec_cafffemodel);
		void segmentation_free_recognation(cv::Mat src, pr2::PlateInfo &plateinfo);
	private:
		cv::dnn::Net RecNet;
	};
}//namespace pr2
#endif // !_PLATERECOGNATION_H_

