#pragma once
#include "afxmt.h"
#include "SMCom.h"
//#include "Cerebra.h"
#include "CMaxmatch.h"
#include "CCutor.h"
#include "CSegment.h"
#include "CAmbitor.h"

#include "Clexical.h"
#include "..\\Include\\maxen.h"



class CSegment
{

public:
	unsigned int Max_Word_Length;
	unsigned long un_amboguity_cnt;
	unsigned long totalsent_cnt;

	Clexical& m_Clexical;
	CMaxmatch m_CMaxmatch;
	CCutor m_CCutor;
	CAmbitor m_CAmbitor;

	vector<int>* m_p_SegmentingPath;
	vector<string>* m_p_SPathStr_v;
	const char* m_p_Sent;
	size_t m_SentPosit;
	size_t m_SentLength;
	size_t m_i;
	vector<vector<string>*> m_SPathStr_vv;

	size_t CheckPoint;

	MaxentModel m_maxent;

public:

	CSegment(Clexical& m_Clexical);
	~CSegment();

	void Collecting_MOAS_Ambiugity_Segmenting_String_for_Training(unsigned int mLength, vector<int>& Seg_v, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v);

	bool Sentence_Ambiguity_Detecting_and_Segmentation(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark);
	void Sentence_Segmenting_Port(const char* SentBuf, CLAUSEPOS& SentPos);
	void Output_Ambiguity_Info(const char* cFoldPath);
	void Output_SPathStr_for_CRF(const char* cFoldPath);
	void Read_CRF_Segmenting_Path_Resusts(const char* outputpath, vector<vector<string>*>& pm_SPathStr_vv);

	void Revised_Ambiguity_Info(vector<AmbiguintInfo*>& AmbiguityInfo_v, SEGMENT_TOKEN_TYPE TYPE);

};
