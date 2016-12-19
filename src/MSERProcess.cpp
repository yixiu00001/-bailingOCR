/*=============================================================================
#
# Author: yixiu - yixiu@inspur.com
#
# Technique Research Centre
#
# Last modified: 2016-11-01 10:27
#
# Filename: mser.cpp
#
# Description:提供图像识别的接口，接口内容包括：
#（1）图像预处理；
#（2）图像调用MSER检测
#（3）图像小区域链接组合
#（4）组合图像拼接，二值化，调用tesseract识别
#
=============================================================================*/
#include "../include/MSERProcess.h"
#include <fstream>

/** A constructor with an optional argument specifying path of the model file.
 *  If called with no argument, the model file is assumed to be stored in the
 *  the working directory as "../model".
 *
 *  @param model_path Path of the model file, either absolute or relative to
 *  the working directory.
 */
MSERProcess::MSERProcess(char*modelPath)
{
	printf("1 MSERProcess path=%s\n", modelPath);

	if(modelPath==NULL)
	{
		strcpy(this->path, "../model/");
	}else	
	{
		strcpy(this->path, modelPath);
	}
	strcat(this->path, "/");
	printf("2 MSERProcess path=%s\n", this->path);

	//dealCorrect = new DealCorrect();
	//getCandidate = new GetCandidate();
	linkCandidate = new LinkCandidate();

}

MSERProcess::~MSERProcess()
{

	//delete dealCorrect;
	//delete getCandidate;
	delete linkCandidate;
}

char* MSERProcess::doGetText(Mat src)
{
	printf("get here1\n");
	//step1, preprocessing, resize and correct
	Mat corrMat;
	corrMat = dealCorrect.standardizeW(1200.0, src);
	showWindowImg("standardizeW", corrMat);
	waitKey(0);

	printf("get here2\n");
	//step2, get candidate, MSER获取候选区域
	candidateStore = getCandidate.run(corrMat);

	printf("get here3=%d\n", candidateStore.size());
	return "test";
	//step3, link candidates， 版面分割
	cgts = linkCandidate->run(candidateStore);

	printf("get here4\n");
	//step4, recognized识别
	Mat plot = doLink2Target(corrMat);

	printf("get here5\n");
	TextRecognition tess;
	tess.Init(3,"chi_sim+eng", PSM_SINGLE_BLOCK, this->path); //PSM_AUTO  PSM_SINGLE_BLOCK
	char* outStr = tess.run(plot);
	char * outStrDelSpace = dealSpace(outStr);
	return outStrDelSpace;

}
Mat MSERProcess::doLink2Target(Mat src)
{
	Mat plot, image_roi;
	plot.create(src.size(), CV_8UC1);
	plot = Scalar::all(255);
	Mat tmpImg = src.clone();
	Mat tmpImg2 = src.clone();
	for(unsigned int i=0;i<cgts.size();i++)
	{
		Rect r = cgts[i];

		if(r.width >18)
		{

			rectangle(tmpImg2, r.tl(), r.br(), CV_RGB(255,0,0), 1,8,0);
			image_roi = tmpImg.rowRange(r.tl().y, r.br().y);
			image_roi = image_roi.colRange(r.tl().x, r.br().x);

			image_roi = FilterAdaptive(image_roi);

			for(int i = r.tl().y; i < r.br().y; i++){
				for (int j = r.tl().x; j < r.br().x; j++){
					plot.at<uchar>(i, j) = image_roi.at<uchar>(i - r.tl().y, j - r.tl().x);
				}
			}

		}
	}

	showWindowImg("link ", tmpImg2);
	showWindowImg("Text final TMP", plot);
	cv::imwrite("final4.jpg", plot);
	waitKey(0);
	return plot;
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
void MSERProcess::showWindowImg(string name,Mat &img)
{
	namedWindow(name,0);
	imshow(name,img);
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
	 
