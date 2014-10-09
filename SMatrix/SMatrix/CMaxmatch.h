#pragma once
#include "afxmt.h"
#include "SMCom.h"



class CMaxmatch
{

public:
	CMaxmatch();
	~CMaxmatch();

	void Maximum_Match_Segmentation_Port(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<string>& TokenCS_v, vector<int>& Seg_v);
	void Maximum_Match_Segmentation(const char* Sentbuf, vector<string>& segment_v, set<string>& pmWordSet, DWORD SegmentType);

	void Forword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v);
	void Backword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v);
	
	void Generate_Segmentation_Matrix(const char* SentBuf, set<string>& locCSWords_Set, vector<string>& TokenCS_v, int Max_Word_Length, bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int& mLength);
	
	//------
	void Omni_segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, int asStart, int asEnd, vector<vector<int>*>& Rtndeque_vv);
	void Recersive_Breadth_First_Omni_segmentation(int position, int endposit, multimap<int, int>& pmMultiMap, vector<int>* pS_v, vector<vector<int>*>& Rtndeque_vv);
	void Recersive_Omni_segmentation(int position, multimap<int, int>& pmMultiMap, vector<int> S, deque<vector<int>>& Rtndeque);
	//------
	void Maximum_no_cover_Ambiguity_Graph(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<string>& TokenCS_v, vector<vector<int>*>& Rtndeque_vv);
	void Recersive_Maximum_no_cover_Ambiguity_Graph(int START, int back_point,  int mLength, multimap<int, int>& pmMultiMap, vector<int>* pS_v, vector<vector<int>*>& Rtndeque_vv);

};
