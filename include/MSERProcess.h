/*=============================================================================
#
# Author: yixiu - yixiu@inspur.com
#
# Technique Research Centre
#
# Last modified: 2016-11-01 9:30
#
# Filename: mser.h
#
# Description:提供图像识别的接口
#
=============================================================================*/
#ifndef _MSER_
#define _MSER_
#include "head.h"
#include "DealCorrect.h"
#include "GetCandidate.h"
#include "LinkCandidate.h"
#include "TextRecognition.h"
#include<climits>
using namespace cv;
using namespace std;
class MSERProcess
{
	public:
		MSERProcess( char* path);
		MSERProcess();
		~MSERProcess();

		//char *process(char*, int);
		char* doGetText(Mat src);
	private:
		Mat FilterAdaptive(Mat &src);
		void showWindowImg(string, Mat &);
		Mat doLink2Target(Mat src);
		char* dealSpace(char* source);
		//void showMserRes(Mat &src, vector<Candidate> candidateStore);
		//char* dealSpace(char* source);
	private:
		DealCorrect dealCorrect;
		GetCandidate  getCandidate;
		LinkCandidate *linkCandidate;
	private:
		char path[INT_MAX];
		vector<Candidate>candidateStore;
		vector<Rect> cgts; 
};		
#endif
