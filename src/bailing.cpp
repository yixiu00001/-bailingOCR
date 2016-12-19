#include "../include/head.h"
#include "../include/MSERProcess.h"
#include<stdio.h>
#include<stdlib.h>
#define TEST 1
int main(int argc, char* argv[])
{
	if(TEST==1)
	{

		Mat img = imread(argv[1], 1);
		if(img.empty())
		{
			cout<<"File read failed!"<<endl;
			return -1;
		}
		char*path = "/root/1xiu/bailingOCR/model/";
		MSERProcess *mser = new MSERProcess(path);

		char * res = mser->doGetText(img);
		cout<<"res="<<res<<endl;
	}
	else if(TEST==2)
	{
	/*	
		Mat img = imread(argv[1], 1);
		if(img.empty())
		{
			cout<<"File read failed!"<<endl;
			return -1;
		}
		int size = img.elemSize()*img.total();
		vector<uchar> buff;//buffer for coding
		vector<int> param = vector<int>(2);
		param[0] = CV_IMWRITE_JPEG_QUALITY;
		param[1] = 95;//default(95) 0-100

		imencode(".jpg", img, buff, param);
		uchar *pImg = &(buff[0]);
		char *ppImg = (char*)pImg;

		char* modelPath="/root/1xiu/bailingOCR/model/";

		MSERProcess *pr = new MSERProcess(modelPath);
		char * res = pr->process(ppImg, size);
		cout<<"res="<<res<<endl;
		*/

	}
	return 0;

}
