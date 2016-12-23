#include "../include/head.h"
#include "../include/MSERProcess.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>
using namespace std;

using namespace cv;
vector<string> dir(string path);
int main(int argc, char* argv[])
{
	printf("get here\n");
	if(argc<2)
	{
		printf("./bailing img1\n");
		return -1;
	}
	char*path="/root/1xiu/bailingOCR2.0/model/";
	MSERProcess mp(path);
	MSERProcess *p = new MSERProcess(path);

	char* filePath = argv[1];
	vector<string> fileList;
	fileList = dir(filePath);
	for(unsigned i=0;i<fileList.size();i++)
	{
		const char* name = fileList[i].c_str();
		printf("name=%s\n",name);
		Mat img = imread(name, 1);
		if(img.empty())
		{
			printf("Img is empty!\n");
			return -1;
		}
	   char*res1 = mp.doGetTxt( img);
	   printf("res=%s\n",res1);

	   char* res2 = p->doGetTxt( img);
	   printf("res=%s\n",res2);
	}

	   delete p;
}
vector<string> dir(string cate_dir)
{
	vector<string>  files;
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir=opendir(cate_dir.c_str())) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr=readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
			continue;
		else if(ptr->d_type == 8)    ///file
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			files.push_back(cate_dir+"/"+string(ptr->d_name));
		else if(ptr->d_type == 10)    ///link file
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			continue;
		else if(ptr->d_type == 4)    ///dir
		{
			files.push_back(cate_dir+"/"+string(ptr->d_name));
			/*
			   memset(base,'\0',sizeof(base));
			   strcpy(base,basePath);
			   strcat(base,"/");
			   strcat(base,ptr->d_nSame);
			   readFileList(base);
			 */
		}
	}
return files;
}
