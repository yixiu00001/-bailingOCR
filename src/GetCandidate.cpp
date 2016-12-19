#include "../include/GetCandidate.h"
#include <stdio.h>
GetCandidate::GetCandidate()
{}

GetCandidate::~GetCandidate()
{}

vector<Candidate> GetCandidate::run(Mat &Image)
{
	printf("get GetCandidate\n");

	imshow("img1", Image);
	waitKey(0);
	Mat oriBgrImage = Image;
	oriBgrImage_8UC3 = Image.clone();
	imshow("img2", oriBgrImage_8UC3);
	waitKey(0);
	cv::cvtColor(oriBgrImage_8UC3,gray_source_image,CV_BGR2GRAY);
	featureExtract();
	Filter();



	return candidateStore;
}

void GetCandidate::showMSER()
{
	Mat plot;
	plot.create(this->oriBgrImage_8UC3.size(), this->oriBgrImage_8UC3.type());
	plot = Scalar::all(255);
	/*
	printf("main=%d\n", this->strVectorStore.size());
	printf("rows=%d cols=%d\n", this->oriBgrImage_8UC3.rows, this->oriBgrImage_8UC3.cols);
	for(int i =  0; i < this->strVectorStore.size(); i++){
		printf("vecsize=%d\n", this->strVectorStore[i].size());
		for (int j = 0; j < this->strVectorStore[i].size(); j++){
			printf("size=%d\n", this->strVectorStore[i].size());
			plot.at<Vec3b>(i, j) = this->oriBgrImage_8UC3.at<Vec3b>(i, j);
		}
	}
	mshow("mser", plot);
	*/
}
void GetCandidate::featureExtract()
{
	strVectorStore.clear();
	mser = MSER(2, 20, 2000, 0.25, 0.2, 200, 1.01, 0.003, 5);
	mser(this->gray_source_image,this->strVectorStore);

	//showMSER();

}
void GetCandidate::ExtractCCfeatures()
{
	ccStore.clear();
	int Num = strVectorStore.size();
	Candidate cd;
	for(int i= 0; i < Num ;i++)
	{
		cd.pointsNum = strVectorStore[i].size();
		cd.max_i = strVectorStore[i][0].y;
		cd.min_i = strVectorStore[i][0].y;
		cd.max_j = strVectorStore[i][0].x; 
		cd.min_j = strVectorStore[i][0].x; 
		float temp = 0;
		float tempr = 0;
		float tempg = 0;
		float tempb = 0;
		for(int j = 1;j < cd.pointsNum;j++)
		{
			if(strVectorStore[i][j].y > cd.max_i)
				cd.max_i = strVectorStore[i][j].y;
			else if(strVectorStore[i][j].y < cd.min_i)
				cd.min_i = strVectorStore[i][j].y;

			if(strVectorStore[i][j].x > cd.max_j)
				cd.max_j = strVectorStore[i][j].x;
			else if(strVectorStore[i][j].x < cd.min_j)
				cd.min_j = strVectorStore[i][j].x;
			temp  +=  this->gray_source_image.at<unsigned char>(strVectorStore[i][j].y, strVectorStore[i][j].x);
			tempr +=  this->oriBgrImage_8UC3.at<cv::Vec3b>(strVectorStore[i][j].y, strVectorStore[i][j].x)[0];
			tempg +=  this->oriBgrImage_8UC3.at<cv::Vec3b>(strVectorStore[i][j].y, strVectorStore[i][j].x)[1];
			tempb +=  this->oriBgrImage_8UC3.at<cv::Vec3b>(strVectorStore[i][j].y, strVectorStore[i][j].x)[2];
		}
		cd.middle_i = (cd.max_i + cd.min_i)/2;
		cd.middle_j = (cd.max_j + cd.min_j)/2;
		cd.avergecolor = temp / cd.pointsNum;
		cd.averarer = tempr / cd.pointsNum;
		cd.averareg = tempg / cd.pointsNum;
		cd.averareb = tempb / cd.pointsNum;
		cd.index = i;
		cd.alive = true;
		cd.high = cd.max_i - cd.min_i;
		cd.width = cd.max_j - cd.min_j;
		getStrokeWidth(strVectorStore[i],cd);
	    float var = 0;
		for(int j = 0; j< cd.pointsNum;j++)
			var += (gray_source_image.at<unsigned char>(strVectorStore[i][j].y, strVectorStore[i][j].x) - cd.avergecolor)*(gray_source_image.at<unsigned char>(strVectorStore[i][j].y, strVectorStore[i][j].x) - cd.avergecolor);
		cd.colorVariance = var/cd.pointsNum;
		float sum = 0;
		for(int i = 0; i < cd.pointsNum; i++){
			sum += cd.strokeWidh;
		}
		cd.StrokeMean = sum / cd.pointsNum;
		cd.highWidthRatio = double(cd.max_i - cd.min_i)/(cd.max_j - cd.min_j);
		float area = (cd.max_i - cd.min_i)*(cd.max_j - cd.min_j);
		cd.grayRatio = float(cd.pointsNum)/(area + 1);
		ccStore.push_back(cd);
	}
}

void GetCandidate::Filter()
{
	ExtractCCfeatures();
	candidateStore.clear();
	for(unsigned int i = 0; i< ccStore.size();i++)
	{
		Candidate cd =  ccStore[i];
		
		Size sz = this->oriBgrImage_8UC3.size();
		double w = sz.width, h = sz.height;

		if((cd.high*cd.width) < (w*h/200.0) && ((cd.high*cd.width) > (w*h/20000.0))) {
			//if(sqrt(cd.strokeVariance) / cd.StrokeMean < 1.2){
			if(cd.highWidthRatio < 3 && cd.highWidthRatio > 0.33){
				candidateStore.push_back(cd);
			}
		}
		/*if((cd.strokeVariance / cd.StrokeMean) < 35){
			candidateStore.push_back(cd);
		}*/
	}
}
void GetCandidate::getStrokeWidth(vector<cv::Point2i>& component, Candidate& cad)
{
	double ccisize = component.size();
	double NumWidth = 0, avgstrokewidth = 0, variancestrokewidth = 0; 
	Mat StrokeImage = this->gray_source_image;
	for(int i = 0; i< ccisize;i++)
	{
		int iindex = component[i].y,jindex = component[i].x;
		if(StrokeImage.at<float>(iindex,jindex) == 151 || StrokeImage.at<float>(iindex,jindex) == 0) 
			StrokeImage.at<float>(iindex,jindex) = 0; 
		else
			NumWidth++;
		avgstrokewidth += StrokeImage.at<float>(iindex,jindex);
		variancestrokewidth += (StrokeImage.at<float>(iindex,jindex) * StrokeImage.at<float>(iindex,jindex));
	}
	cad.strokeWidthRatio = NumWidth/ccisize;
	cad.strokeWidh = avgstrokewidth/ccisize;
	cad.strokeVariance = (variancestrokewidth/ccisize) - (cad.strokeWidthRatio)*(cad.strokeWidthRatio);
}
