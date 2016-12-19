#include "head.h"

#ifndef _DEAL_CORRECT_
#define _DEAL_CORRECT_
class DealCorrect{
public:

	DealCorrect();
	~DealCorrect();
	cv::Mat standardizeW(double, cv::Mat &);
	cv::Mat standardizeH(double, cv::Mat &);
	//cv::Mat run(cv::Mat &);
};

#endif
