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
	//flag=0,chi_sim,  ����         �޺ڰ�����
	//flag=1,chi_sim,  �Ա�+���壬  �а�����
	//flag=2,eng��     ����         �а�������0~9���֣�or chi_sim+eng "��""��""��""��""��" 0~9��
	//flag=3,chi_sim+eng, ��ַ����ݺ���   �к�����
	char* run(Mat &);
	
private:
//	TessBaseAPI tess;
};

#endif
