/*=============================================================================
#
# Author: yixiu - yixiu@inspur.com
#
# Technique Research Centre
#
# Last modified: 2016-11-01 10:27
#
# Filename: MSERProcess.cpp
#
# Description:提供图像识别的接口，接口内容包括：
#（1）图像预处理；
#（2）图像调用MSER检测
#（3）图像小区域链接组合
#（4）组合图像拼接，二值化，调用tesseract识别
#
=============================================================================*/
#include "../include/MSERProcess.h"
#include<stdio.h>
//为了兼容java使用jni方法调用，将函数中用到的变量放在了全局位置
//如果放在类头文件的成员变量位置，java调用会出core,预估java和c++的内存分配方式不一样
	vector<Candidate> candidateStore;
	vector<Candidate>ccStore;
	vector< vector<cv::Point2i> > strVectorStore; 
MSERProcess::MSERProcess(char*path)
{
	if(path==NULL)
		strcpy(this->path,"../model/");

	strcpy(this->path,path);

	textRecognition.Init(3,"chi_sim+eng", PSM_SINGLE_BLOCK, this->path); //PSM_AUTO  PSM_SINGLE_BLOCK

}
MSERProcess::~MSERProcess()
{
}
//证照识别的主函数，输入原始图片，输出识别的txt文本
char* MSERProcess::doGetTxt( cv::Mat src)
{
	//printf("step1 dealCorrect\n");

	//step1, preprocessing,do resize and correct
	src = dealCorrect.standardizeW(1200.0, src);
	//step2, get candidate, MSER获取候选区域
	Mat oriBgrImage_8UC3;
	Mat gray_source_image; 

	//printf("step2 getCandidate\n");
	getCandidate.run(src,oriBgrImage_8UC3, gray_source_image, strVectorStore, ccStore,candidateStore);

	//step3, link candidates， 版面分割
	//printf("step3 linkCandidate\n");

	int max_y;
	int min_y;
	int max_x;
	int min_x;
	vector<Rect> cgts;
	cgts = linkCandidate.run(candidateStore, max_y, min_y, max_x, min_x);
	//printf("max_y=%d min_y = %d max_x =%d min_x=%d\n", max_y, min_y, max_x, min_x);

	//step4, recognized识别
	//printf("get target domain\n");
	Mat plot, image_roi;
	Mat tmpImg = src.clone();
	Mat tmpImg2 = src.clone();

	plot.create(src.size(), CV_8UC1);
	plot = Scalar::all(255);

	Mat plotTmp;
	plotTmp.create(src.size(), CV_8UC1);
	plotTmp = Scalar::all(255);


	//get the main domain, top/bottom/left/right
	int top_y = (max_y - min_y)*0+min_y;
	int bottom_y = max_y - (max_y -min_y)*0.05;
	int left_x = min_x;
	int right_x = max_x ;

	//int top_y = (max_y - min_y)*0.25+min_y;
	//int bottom_y = max_y - (max_y -min_y)*0.25;
	//int left_x = (max_x - min_x)*0.12 + min_x;
	//int right_x = max_x - (max_x - min_x)*0.12;

	for(unsigned int i = 0; i < cgts.size(); i++)
	{
		Rect r = cgts[i];

		//if(r.y<top_y ||r.y>bottom_y)
		//	continue;

		if(r.width > 18){
			rectangle( tmpImg2, r.tl(), r.br(), CV_RGB(255, 0, 0), 1, 8, 0 );
			//cout<<"Height = "<<r.br().y-r.tl().y<<" Width = "<<r.br().x-r.tl().x<<endl;
			image_roi = tmpImg.rowRange(r.tl().y, r.br().y);
			image_roi = image_roi.colRange(r.tl().x, r.br().x);

			image_roi = FilterAdaptive(image_roi);

			for(int i = r.tl().y; i < r.br().y; i++){
				for (int j = r.tl().x; j < r.br().x; j++){
					plot.at<uchar>(i, j) = image_roi.at<uchar>(i - r.tl().y, j - r.tl().x);
					//copy the selected domain to a new image
					if(r.y>=top_y && r.y<=bottom_y && r.x>=left_x && r.x <= right_x)
					{
						plotTmp.at<uchar>(i, j) = image_roi.at<uchar>(i - r.tl().y, j - r.tl().x);

					}
					}
				}
			}
	}

	//showWindowImg("link ", tmpImg2);
	//imshow("link", plot);
	//imwrite("plot2.jpg", tmpImg2);
	//imwrite("plot.jpg", plotTmp);
	//waitKey(0);
	//compute the text of the input image base tessract
	char* outStr = textRecognition.run(plot);
	//deal the space
	char* outStrDst = dealSpace(outStr);
	return outStrDst;
	//return "target test";
}

Mat MSERProcess::FilterAdaptive(Mat &src)
{

	double w = src.size().width;
	double h = src.size().height;

	cvtColor(src, src, COLOR_BGR2GRAY);
	Mat binarization;
	Mat temp;

	int blockSize = (int)src.size().height;
	if(blockSize % 2 == 0)
	{
		blockSize--;
	}

	//cout<<"blockSize = "<<blockSize<<" C = "<<h/2.0<<endl;
	adaptiveThreshold(src, binarization, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, blockSize, h*0.4);
	//medianBlur(binarization, binarization, 1);

	return binarization;
}

char* MSERProcess::dealSpace(char* source)
{
	char *p = source;
	int j=0;
	while(*p ==' ')
	{
		p++;
	}
	while(*p!='\0')
	{
		if(*p=='\n' && *(p-1)=='\n')
		{
			p++;
		}
		if(*p!=' ')
		{
			source[j++] = *p;
			p++;
		}
		
		while(*p==' ')
		{
			p++;
		}

	}

	source[j]='\0';

	return source;
}
