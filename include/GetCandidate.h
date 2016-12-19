#ifndef _GET_CANDIDATE_
#define _GET_CANDIDATE_
#include "head.h"
#include "Candidate.h"
#include <math.h>

using namespace std;
using namespace cv;
class GetCandidate
{
public:
	GetCandidate();
	~GetCandidate();
	vector<Candidate> run(Mat &Image);
	
private:
	void featureExtract();
	void ExtractCCfeatures();
	void Filter();
	void getStrokeWidth(vector<cv::Point2i>& component, Candidate& cad);
	void showMSER();
private:
	MSER mser;
	Mat oriBgrImage_8UC3;
	Mat gray_source_image;
	
	vector<Candidate>ccStore;
	vector<Candidate>candidateStore;
	vector< vector<cv::Point2i> > strVectorStore; 
};

#endif
