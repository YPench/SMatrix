#pragma once
#include "afxmt.h"
#include "SMCom.h"

#include "CMaxmatch.h"

class CAmbitor
{

public:
	map<string, unsigned long> AmbiStringInfo_m;
	map<string, unsigned long> AmbiChainStringInfo_m;
	CMaxmatch m_A_CMaxmatch;

public:

	CAmbitor();
	~CAmbitor();
	void Segmentation_Ambiguity_Detecting_Port(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v);
	void Segmentation_Ambiguity_Labelling_Port(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v);

	//======Detection========================
	bool Compromise_Combinational_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v);
	bool Combinational_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v);
	bool Overlapping_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v);
	bool Maximum_Overlapping_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v);
	void Omni_Words_or_MNAG_Segmentation_Ambiguity_Detection(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], bool Omni_MNAG_Flag, unsigned int mLength, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v);
	bool BiMM_Segmentation_Ambiguity_Detection(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], vector<string>& pmToken_v, unsigned int mLength, vector<AmbiguintInfo*>& AmbiguityInfo_v);

	//=======Token Mark=======================
	bool Combinational_Ambiguity_Detector_Use_Token_Mark(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark, list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& markite, CLAUSEPOS& pmCLAUSEPOS, bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength);
	bool Overlapping_Ambiguity_Detector_Use_Token_Mark(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark, list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& markite, CLAUSEPOS& pmCLAUSEPOS, bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength);
	bool Ambiguity_Detector_Use_Token_Mark(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark, list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& markite, CLAUSEPOS& pmCLAUSEPOS, bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH]);

	//=======Support=======================
	bool Is_BiMM_Ambiguity(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], int mlength, int START, int END);
	bool Is_Compromise_Combinational_Ambiguity(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], int mlength, int START, int END);
	bool Is_Overlapping_Ambiguity(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], int mlength, int START, int END);

	void Collecting_Ambiguity_in_two_Segmentation_Path(vector<int>& FMM_S_v, vector<int>& BMM_S_v, vector<AmbiguintInfo*>& AmbiguityInfo_v, SEGMENT_TOKEN_TYPE OAS_CAS_TYPE, vector<string>& TokenCS_v);
	void Collecting_Ambiguity_String_Info(vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v);
	void Save_Ambiguity_Statistics_Info(string savepath);

};
