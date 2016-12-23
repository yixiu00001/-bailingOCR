#include "TextRecognition.h"
#define MAX_SIZE 65536
#include<time.h>
TessBaseAPI tess;
void TextRecognition::Init(int flag, char *lang, PageSegMode mode, char*path)
{
/*
	char current_absolute_path[MAX_SIZE];
	//获取当前目录绝对路径
	if (NULL == realpath("./", current_absolute_path))
	{
		printf("***Error***\n");
		exit(-1);
	}
	//printf("current absolute path:%s\n", current_absolute_path);
	strcat(current_absolute_path, "/");
	strcpy(current_absolute_path1, current_absolute_path);
	//tess.Init(current_absolute_path, lang, OEM_DEFAULT);
	*/

	tess.Init(NULL, lang, OEM_DEFAULT);
	tess.SetPageSegMode(mode); 
	char current_absolute_path1[MAX_SIZE];
	strcpy(current_absolute_path1, path);
	strcat(current_absolute_path1, "resource/config.txt");
	tess.ReadConfigFile(current_absolute_path1);
	//tess.SetVariable("language_model_penalty_non_dict_word", "0.15");
	//tess.SetVariable("language_model_penalty_non_freq_dict_word", "0.1");
	/*安猛注释
	  tess.SetVariable("chop_enable", "T");
	  tess.SetVariable("use_new_state_cost", "F");
	  tess.SetVariable("segment_segcost_rating", "F");
	  tess.SetVariable("enable_new_segsearch", "0");
	  tess.SetVariable("language_model_ngram_on", "0");
	  tess.SetVariable("textord_force_make_prop_words","F");
	  tess.SetVariable("edges_max_children_per_outline","40");
	//added by me
	tess.SetVariable("language_model_ngram_space_delimited_language", "0"); 
	//tess.SetVariable("textord_heavy_nr", "1");
	tess.SetVariable("load_fixed_length_dawgs", "1");
	tess.SetVariable("tosp_force_wordbreak_on_punct", "0"); 
	tess.SetVariable("textord_no_rejects", "1"); 

	 */

	/*tess.SetVariable("segsearch_max_futile_classifications", "10"); 
	  tess.SetVariable("load_fixed_length_dawgs","1"); 
	  tess.SetVariable("stopper_ambiguity_threshold_gain","8"); 
	  tess.SetVariable("stopper_ambiguity_threshold_offset","10.5"); */
	switch (flag)
	{
		case 0:
			break;
		case 1:  //性别名族 chi_sim
			tess.SetVariable("tessedit_char_blacklist","性别女男民族汉");//壮满回苗维吾尔土家彝蒙古藏布依侗瑶朝鲜白哈尼黎傣畲傈僳仡佬东乡高山拉祜水佤纳西羌仫锡伯柯克孜达斡景颇毛南撒朗塔吉克阿昌普米鄂温怒京基诺德昂保安俄罗斯裕固乌孜别门巴鄂伦春独龙塔赫哲珞");
			break;
		case 2:  //生日  eng
			tess.SetVariable("tessedit_char_whitelist","0123456789X");
			break;
		case 3:  //住址 chi_sim+eng
			tess.SetVariable("tessedit_char_blacklist", "【】。，、；：？！·‘’“”々～＂＇｀〃‖〔〕〈〉《》「」『』．〖〗（）［］｛｝§№☆★○●◎◇◆□℃‰�°¤〓↓←↑→※▲△■＃＆＠＼︿＿￣―♂♀/+-=*?''""|[]{}()^&%$#@!QWERTYUIOPLKJHGFDSAZCVBNMqwertyuioplkjhgfdsazcvbnm");
			break;
		case 4: //身份证号码  chi_sim+eng
			//tess.SetVariable("textord_heavy_nr", "1"); 
			tess.SetVariable("tessedit_char_blacklist", "/+-=*?''""|[]{}()^&%$#@!QWERTYUIOPLKJHGFDSAZCVBNMqwertyuioplkjhgfdsazcvbnm");
		default:
			break;
	}
	//tess.SetAccuracyVSpeed(tesseract::AVS_FASTEST); //??????

}

char* TextRecognition::run(Mat &src)
{
	clock_t start, finish;
	double duration;
	tess.SetImage((uchar*)src.data, src.cols, src.rows, 1, src.cols);
	cout<<"begin tess get"<<endl;
	start = clock();
	char* out = tess.GetUTF8Text();	
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout<<"end tess get:"<<duration<<endl;
	//cout<<endl<<out<<endl;

	return out;
}

