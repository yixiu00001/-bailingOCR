#ifndef __TEXT_RECOGNITION__
#define __TEXT_RECOGNITOIN__
#include "tesseract/baseapi.h"
#include "head.h"
#include <string>
using namespace tesseract;
using namespace cv;
using namespace std;

class TextRecognition{
public:
	void Init(int flag, char*, PageSegMode, char*);
	//flag=0,chi_sim,  姓名         无黑白名单
	//flag=1,chi_sim,  性别+民族，  有白名单
	//flag=2,eng，     出生         有白名单，0~9数字（or chi_sim+eng "出""生""年""月""日" 0~9）
	//flag=3,chi_sim+eng, 地址、身份号码   有黑名单
	char* run(Mat &);
	
private:
//	TessBaseAPI tess;
};

#endif
