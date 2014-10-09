#pragma once
#include "afxmt.h"
#include "SMCom.h"

class Clexical;
class CIcwb
{

public:

//	map<string, COUPLERBAG> UniCouple_m;
	Clexical& m_Clexical;
public:

	CIcwb(Clexical& m_Clexical);
	~CIcwb();
	string Executer_Port(string pmService, vector<string> sParam_v, vector<LPVOID> pParam_v);

	void Extract_Icwb_Training_Ambiguity_Segment_Standard();

	//----Computer_The_Correct_Rate_According_to_Icwb_In_Two_Files
	void Computer_The_Correct_Rate_According_to_Icwb_In_Two_Files();
	void Statistic_Segment_Result_With_CIcwb_In_File(string pmRetPath, string pmCicwbGold);
	void Save_Statistic_Segment_Result_Compare_With_CIcwb(string pmSaveFileName, deque<CLAUSEPOS>& ErrorSubSentIcwb_d, deque<CLAUSEPOS>& ErrorSubSentStat_d, string SegInfo);
	void Save_Error_Segment_Result_In_CLAUSEPOS_Deque(string pmSaveFileName, deque<CLAUSEPOS>& ErrSegRet_d);
	void Modify_WordsSet_Use_Icwb_Set(deque<CLAUSEPOS>& ErrorSubSentIcwb_d, deque<CLAUSEPOS>& ErrorSubSentStat_d);

	//----Extract_Unequal_Segment_Result_In_Two_Files_And_Save-----------------------------------------
	void Extract_Unequal_Segment_Result_In_Two_Files_And_Save();
	void Extract_The_Error_Result_In_Deque(deque<string>& Static_d, deque<string>& CICWGold_d, deque<string>& Error_d, deque<string>& Correct_d);

	//-----sticking to Icwb training Corpus
	void Extract_CIcwb_Training_To_Set(string pmPath, set<string>& pmWordsSet);
	void Extract_CIcwb_Training_TXT_to_Blank_TXT(string pmPath);


	void Extract_CIcwb_Training_Words_To_Set();
	//--------------------------------------------------------------------------------------------------------------
	//void Compare_The_Result_Whith_Icwb_In_Deque(string pmPath, string pmIcwbPath);
	void Statistic_Segment_Result_With_CIcwb_In_Deque(deque<string>& Static_d, deque<string>& CICWGold_d);
	void Subtract_Mark_Start_Ambiguity(unsigned long& RefTotalCnt, unsigned long& SegTotalCnt, deque<CLAUSEPOS>& ErrorSubSentIcwb_d, deque<CLAUSEPOS>& ErrorSubSentStat_d);

	void Check_Overlopping_and_Combination_Ambiguity_Error_Rate(const char* ResultFolder, deque<CLAUSEPOS> ErrorSubSentIcwb_d,	deque<CLAUSEPOS> ErrorSubSentStat_d);

};
