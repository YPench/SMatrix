#pragma once
#include "afxmt.h"

//#include "CGCom.h"

#include <string>
#include <ostream>
#include <iterator>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>

using namespace std;
#define  MAX_CLAUSE_LENGTH 128
#define  Double_MAX_CLAUSE_LENGTH 256
#define	 MAX_PRODUCT_SEMAPHORE 128
#define	 RESPONCE_WAIT_TIME 100
#define  MAX_SENTENCE		65535
#define  EVERY_FILE_NUMBER 10000
#define LEXICON_DATA_FOLDER		".\\..\\Data\\"

#define POSITIVE "Positive"
#define NEGETIVE "Negetive"

enum SEGMENT_TOKEN_TYPE{CHINESE_TOKEN, NUMERAL_TOKEN, ENGLISH_TOKEN, PUNCTUATION_TOKEN, AMBIGUITY_TOKEN, SPECIAL_TOKEN, MERGERED_TOKEN, PHRASE_TOKEN, 
MOAS_TOKEN, OAS_TOKEN, CAS_TOKEN, UNDEFINED_TOKEN};

typedef struct _AmbiguityInfo
{
	SEGMENT_TOKEN_TYPE OAS_CAS_TYPE;
	unsigned int START;
	unsigned int END;
	unsigned int A_START;
	unsigned int A_END;
}AmbiguintInfo, *pAmbiguintInfo;

typedef struct _CLAUSEPOS
{
	vector<string> Clause;
	vector<string> POS;
}CLAUSEPOS, Sentencepos, *pCLAUSEPOS;


namespace NLPOP{
	void ChineseToken(const char* SentBuf, deque<string>& dTokenCS);
	size_t  Get_Chinese_Sentence_Length_Counter(const char* contentchar);
	string CString2string(CString str);
	CString string2CString(string str);
	void charseq_to_vector(const char* pcharseq, vector<string>& pvector);
	void merging_numbers(string& stringref, set<string>& Number_Set);
	string merging_numbers(const char* pcharseq, set<string>& Number_Set);

	void merging_English(string& stringref, set<string>& English_Set);
	string merging_English(const char* pcharseq, set<string>& English_Set);


	void Output_Processing_Message(bool Total_Flag, size_t TotalCnt, size_t CurCnt);
	void LoadWordsStringSet(string pathstr, set<string>& strset);
	void Get_Specified_Files(string pmFolderPath, vector<string>& FilesRoot_v, string DotAddSuffix);
	void LoadWordData_AS_Map_string_MorphstrSet(string pmWDpath, map<string, set<string>>& pmWordMor_ms);

	bool Exist_of_This_File(string filename);
	BOOL FolderExist(CString strPath);
	BOOL FileExist(CString strFileName);
	BOOL CreateFolder(CString strPath);
	void DeleteDir(CString strDelDir);
	void Delete_Files_In_Dir(CString strDelDir);

	bool Find_Substr_In_Chinese_Sent(const char* TargetBuf, const char* CandidateBuf);
	void Save_Set_String_With_Comma_Divide(string pmSaveName, set<string>& pmSaveSet);

	string Get_Substr_by_Chinese_Character_Cnt(const char* charstr, int position, int Cnt);
}

namespace SMOP{

	void AmbiguityInfo_v_to_Ambiguities_mm(vector<AmbiguintInfo*>& AmbiguityInfo_v, map<int, map<int, AmbiguintInfo*>>& Ambiguities_mm);

	void Generating_Segmentation_Path(vector<string>& RefSPath_v, vector<vector<int>*>& SPath_vv, vector<vector<string>*>& SegmentingPathStr_vv);
	void Segmentation_Path_to_Segmentation_String(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark,list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& tmarkite, vector<int>& pmSeg_v, vector<string>& pmTokenCS);
	void Segmentation_Path_to_Segmentation_String(vector<int>& pmSeg_v, vector<string>& pmTokenCS, vector<string>& pmSegs_v);

	void Save_CLAUSEPOS_deque_Without_Icwb_POS(string pmSavePath, vector<CLAUSEPOS>& pmPasedPos);
	void Save_txt_map_string_long_with_Freq(map<string, unsigned long>& pmmap, string pmSavePath, string pmmemostr);
	void Save_Info_multimap_long_string(multimap<unsigned long, string>& pmMultimap, string pmSaveName, string pmmemostr);
	void Save_Sorted_dat_multimap_string_long(multimap<unsigned long, string>& pmMultimap, string pmSavePath, string pmmemostr);

}

namespace AppCall{

	void Maxen_Responce_Message_with_Save(const char* poutstr);
	int Secretary_Message_Box(string msgstr, UINT nType);
	int Secretary_Message_Box(const char* cmsg);

	void Maxen_Responce_Message(const char* poutstr);
	void Response_Diplay(std::string sout, DWORD MessageType);
	void Consensus_Responce_Message(string msgstr, UINT nType);

	void Secretary_Hide();
	void Secretary_Show();
};
template<typename TCLASS> bool Read_Line_InFile_To_Vector_Deque_List(string ResultFolder, TCLASS& pm_T_Class, unsigned int ThreadID, char Gchar, bool IgnoreEnterCharFlag)
{
	char getlineBuf[MAX_SENTENCE];
	ifstream in(ResultFolder.c_str());
	if(in.bad())
		return false;
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF){
		in.close();
		return false;
	}
	while(true){
		if(in.peek() == EOF){	
			break;
		}
		in.getline(getlineBuf, MAX_SENTENCE, Gchar);
		pm_T_Class.push_back(getlineBuf);
		if(IgnoreEnterCharFlag){
			in.getline(getlineBuf, MAX_SENTENCE, '\n');
		}
	} 
	in.close();
	return true;
}
template<typename TCLASS> bool Write_Vector_Deque_List_To_File(string pmSaveFileName, TCLASS& pm_T_Class, unsigned int ThreadID, char Gchar, string pmmemo)
{
	ofstream out(pmSaveFileName.c_str());
	if(out.bad())
		return 0;
	out.clear();
	out.seekp(0, ios::beg);
	//--------------------------save result file;
	//out << 0xEF << 0xBB << 0xBF;
	for(TCLASS::iterator dite = pm_T_Class.begin(); dite != pm_T_Class.end();  dite++){
		out << *dite << Gchar;
	}
	out.close();
	return true;
}



