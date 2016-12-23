#ifndef _person_
#define _person_

#include "head.h"
#include "DealCorrect.h"
#include "GetCandidate.h" 
#include "LinkCandidate.h"
#include "TextRecognition.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
class MSERProcess
{
	public:
	MSERProcess(char*path);
	~MSERProcess();
	
	char* doGetTxt(cv::Mat img );

	private:
	DealCorrect dealCorrect;
	GetCandidate getCandidate;
	LinkCandidate linkCandidate;
	TextRecognition textRecognition;


	cv::Mat FilterAdaptive(Mat &src);
	char* dealSpace(char* source);

	private:
	char path[65535];
	//vector<Rect> cgts;

};

#endif
