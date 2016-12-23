#include"LinkCandidate.h"
#include<stdio.h>

std::vector<Rect> LinkCandidate::run(std::vector<Candidate>& candidateStore)
{
	std::vector< std::vector<bool> > relation;

	vector<cv::Rect> cgts;
	//this->candidateStore = candidateStore;
	relation = std::vector< std::vector<bool> >(3000,std::vector<bool>(3000,true));
	link(candidateStore, relation);
	finalresult(candidateStore, relation, cgts);
	return cgts;
}
void LinkCandidate::finalresult(std::vector<Candidate> &candidateStore, std::vector< std::vector<bool> > &relation, vector<cv::Rect> &cgts)
{
	Iqueue* Iq;
	Iq = new Iqueue;
	int CandidateNum;
	CandidateNum = candidateStore.size();
	for(int i = 0;i< CandidateNum;i++)
	{
		bool sign = false;
		for(int j = 0;j< CandidateNum;j++)
		{
			sign = sign | relation[i][j];
		}
		candidateStore[i].alive = sign;
		candidateStore[i].chuan = 0;
	}
	
	int index = 1;
	int tempNum;
	for(int i = 0;i< CandidateNum;i++)
	{
		if(candidateStore[i].alive == false)
		{
			continue;
		}

		Iq->reset();
		Iq->push(i);

		candidateStore[i].chuan = index;
		candidateStore[i].alive = false;
		while(Iq->length)
		{
			tempNum = Iq->pop();
			for(int j = 0;j< CandidateNum;j++)
			{
				if(relation[tempNum][j] == true)
				{
					Iq->push(j);
					candidateStore[j].chuan = index;
					candidateStore[j].alive = false;
					relation[tempNum][j] = false;
					relation[j][tempNum] = false;
				}
			}

		}
		index++;
	}

	vector< vector<int> > store(index + 2);
	for(int i = 0;i< CandidateNum;i++)
	{
		store[candidateStore[i].chuan].push_back(i);
	}

	int max_y;
	int min_y;
	int max_x;
	int min_x;
	max_y = 0;
	min_y = 65535;

	max_x = 0;
	min_x = 65535;

	
	for(int i = 1;i< index;i++)
	{
		if(store[i].size() <=4)
			continue;

		vector<cv::Point> PointV;
		for(unsigned int j = 0; j< store[i].size();j++)
		{
			PointV.push_back(cv::Point(candidateStore[store[i][j]].min_j,candidateStore[store[i][j]].min_i));
			PointV.push_back(cv::Point(candidateStore[store[i][j]].max_j,candidateStore[store[i][j]].max_i));
		}
		
		cv::Rect textline = cv::boundingRect(PointV);
		cgts.push_back(textline);
		
		min_y = (textline.y>min_y)?min_y:textline.y;
		max_y = (textline.y<max_y)?max_y:textline.y;

		min_x = (textline.x>min_x)?min_x:textline.x;
		max_x = (textline.x<max_x)?max_x:textline.x;

	}
}

void LinkCandidate::findPairRegion(int i, int j, std::vector<Candidate> &candidateStore, std::vector< std::vector<bool> > &relation)
{
	relation[i][j] = false;
	relation[j][i] = false;
	Candidate cdI = candidateStore[i];
	Candidate cdJ = candidateStore[j];
	//printf("findPairRegion 1 i=%d j=%d\n", i,j);

	if(cdI.middle_j == cdJ.middle_j && cdI.middle_i == cdJ.middle_i) 
	{	
		//printf("findPairRegion 2\n");
		return;
	}
	if(cdI.high/cdJ.high > 2.0 || cdI.high/cdJ.high < 0.5) 
	{
		//printf("findPairRegion 3\n");
		return;
	}
	if(cdI.width/cdJ.width > 4 || cdI.width/cdJ.width < 0.25) 
	{
		//printf("findPairRegion 4\n");
		return;
	}
	if(cdI.strokeWidh/cdJ.strokeWidh > 4 || cdI.strokeWidh/cdJ.strokeWidh < 0.2) 
	{
		//printf("findPairRegion 5\n");
		return;
	}
	//if(abs(candidateStore[i].avergecolor - candidateStore[j].avergecolor) > 20 ) return;
	//if(abs(candidateStore[i].averareb - candidateStore[j].averareb) > 20 ) return;
	//if(abs(candidateStore[i].averarer - candidateStore[j].averarer) > 20 ) return;
	//if(abs(candidateStore[i].averareg - candidateStore[j].averareg) > 20 ) return;
	double deltai = abs(cdI.middle_i - cdJ.middle_i);
	double deltaj = abs(cdI.middle_j - cdJ.middle_j);
	//printf("findPairRegion 6\n");
	double Theroidi = min(cdI.width,cdJ.width);
	double Theroidj = min(cdI.high,cdJ.high);
	if(deltai > 0.4*Theroidi || deltaj > 4*Theroidj) 
		return ; 

	relation[i][j] = true;
	relation[j][i] = true;
}

void LinkCandidate::link(std::vector<Candidate> &candidateStore, std::vector< std::vector<bool> > &relation) 
{
	int CandidateNum;
	CandidateNum = candidateStore.size();
	for(int i = 0;i< CandidateNum - 1;i++)
	{
		for(int j = i + 1;j< CandidateNum;j++)
		{
			findPairRegion(i,j, candidateStore, relation);
		}
	}
}

