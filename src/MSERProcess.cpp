#include "../include/MSERProcess.h"
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
	vector<Rect> cgts;
	cgts = linkCandidate.run(candidateStore);

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
					}
				}
			}
	}

	//showWindowImg("link ", tmpImg2);
	//imshow("link", tmpImg2);
	//imshow("plot", plot);
	//waitKey(0);
	char* outStr = textRecognition.run(plot);
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
