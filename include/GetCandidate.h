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
	void run(Mat &Image,Mat &oriBgrImage_8UC3,Mat &gray_source_image,vector< vector<cv::Point2i> > &strVectorStore,vector<Candidate> &ccStore, vector<Candidate>&candidateStore);
	
private:
	void featureExtract(Mat &oriBgrImage_8UC3,Mat &gray_source_image,vector< vector<cv::Point2i> > &strVectorStore);

	void ExtractCCfeatures(Mat &oriBgrImage_8UC3,Mat &gray_source_image,vector< vector<cv::Point2i> > &strVectorStore,vector<Candidate>&ccStore);

	void Filter(Mat &oriBgrImage_8UC3,Mat &gray_source_image,vector< vector<cv::Point2i> > &strVectorStore,vector<Candidate> &ccStore,vector<Candidate> &candidateStore);

	void getStrokeWidth(vector<cv::Point2i>& component, Candidate& cad, Mat &oriBgrImage_8UC3,Mat &gray_source_image);
private:
MSER mser;
	//Mat oriBgrImage_8UC3;
	//Mat gray_source_image;
	
	//vector<Candidate>ccStore;
	//vector<Candidate>candidateStore;
	//vector< vector<cv::Point2i> > strVectorStore; 
};

#endif
