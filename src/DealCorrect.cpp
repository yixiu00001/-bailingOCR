#include "../include/DealCorrect.h"

#define GRAY_THRESH 130
#define HOUGH_VOTE 150

using namespace cv;
using namespace std;
DealCorrect::DealCorrect()
{}
DealCorrect::~DealCorrect()
{}

Mat DealCorrect::standardizeW(double target_w, cv::Mat &src)
{
		double w = src.size().width;
		double h = src.size().height;

		resize(src, src, Size(target_w, target_w/w*h), 0, 0, INTER_LINEAR);

	//cout<<"w = "<<src.size().width<<" h= "<<src.size().height<<endl;
		return src;
}
Mat DealCorrect::standardizeH(double ratio_h, Mat &src)
{
	double w = src.size().width;
	double h = src.size().height;
	resize(src, src, Size(w*ratio_h, h*ratio_h), 0, 0, INTER_LINEAR);

	return src;
}
