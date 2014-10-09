#include "StdAfx.h"
#include "CAmbitor.h"
#include "..\\SMatrix.h"

extern bool BiMM_Detecting_Flag;
extern bool SM_MOAS_Detecting_Flag;
extern bool SM_OAS_Detecting_Flag;
extern bool SM_Compromise_CAS_Detecting_Flag;
extern bool SM_CAS_Detecting_Flag;
extern bool Omni_or_MNAG_Detecting_Flag;

CAmbitor::CAmbitor() 
{
	
}
CAmbitor::~CAmbitor()
{

}

void CAmbitor::Segmentation_Ambiguity_Detecting_Port(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	if(!(BiMM_Detecting_Flag || SM_MOAS_Detecting_Flag || SM_OAS_Detecting_Flag || SM_Compromise_CAS_Detecting_Flag || SM_CAS_Detecting_Flag || Omni_or_MNAG_Detecting_Flag)){
		return;
	}
	
	if(BiMM_Detecting_Flag){// BiMM Detecting
		BiMM_Segmentation_Ambiguity_Detection(SegMatrix, TokenCS_v, mLength, AmbiguityInfo_v);
	}
	if(SM_MOAS_Detecting_Flag){// SM MOAS Detecting
		Maximum_Overlapping_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);
	}
	if(SM_OAS_Detecting_Flag){// SM OAS Detecting
		Overlapping_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);
	}
	if(SM_Compromise_CAS_Detecting_Flag){// SM Compromise CAS Detecting
		Compromise_Combinational_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);
	}
	if(SM_CAS_Detecting_Flag){// SM CAS Detecting
		Combinational_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);
	}
	if(Omni_or_MNAG_Detecting_Flag){//Omni-words or MNAG amgiutiy detection
		Omni_Words_or_MNAG_Segmentation_Ambiguity_Detection(SegMatrix, false, mLength, TokenCS_v, AmbiguityInfo_v);
	}

	Collecting_Ambiguity_String_Info(TokenCS_v, AmbiguityInfo_v);

}

void CAmbitor::Segmentation_Ambiguity_Labelling_Port(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	m_A_CMaxmatch.Forword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, Seg_v);
	
	Maximum_Overlapping_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);

	Overlapping_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);

	Compromise_Combinational_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);

	if(false){// SM CAS Detecting
		Combinational_Ambiguity_Detection(SegMatrix, mLength, AmbiguityInfo_v);
	}

}



//==============================================================================

void CAmbitor::Omni_Words_or_MNAG_Segmentation_Ambiguity_Detection(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH],  bool Omni_MNAG_Flag, unsigned int mLength, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	vector<vector<int>*> Seg_vv;
	if(Omni_MNAG_Flag){
		m_A_CMaxmatch.Omni_segmentation(SegMatrix, mLength, 0, mLength, Seg_vv);
	}
	else{
		m_A_CMaxmatch.Maximum_no_cover_Ambiguity_Graph(SegMatrix, mLength, TokenCS_v, Seg_vv);
	}

	vector<AmbiguintInfo*> loc_AmbiguityInfo_v;
	for(size_t i = 0; i < Seg_vv.size(); i++){
		for(size_t j = i+1; j < Seg_vv.size(); j++){
			//Collecting_Ambiguity_in_two_Segmentation_Path(*Seg_vv[i], *Seg_vv[j], loc_AmbiguityInfo_v, false, TokenCS_v);
			//Collecting_Ambiguity_in_two_Segmentation_Path(*Seg_vv[i], *Seg_vv[j], loc_AmbiguityInfo_v, true, TokenCS_v);
		}
		delete Seg_vv[i];
	}
	
	map<size_t, set<size_t>> Filter_ms;
	for(size_t i = 0; i < loc_AmbiguityInfo_v.size(); i++){
		AmbiguintInfo& rlocInfo = *loc_AmbiguityInfo_v[i];
		if(Filter_ms.find(rlocInfo.START) == Filter_ms.end()){
			Filter_ms[rlocInfo.START];
		}
		//if(!Is_Compromise_Combinational_Ambiguity(SegMatrix, mLength, rlocInfo.START, rlocInfo.END)){
		//	delete loc_AmbiguityInfo_v[i];
		//}else
		//if(Is_Overlapping_Ambiguity(SegMatrix, mLength, rlocInfo.START, rlocInfo.END)){
		//	delete loc_AmbiguityInfo_v[i];
		//}else
		if(Filter_ms[rlocInfo.START].find(rlocInfo.END) == Filter_ms[rlocInfo.START].end()){
			Filter_ms[rlocInfo.START].insert(rlocInfo.END);
			AmbiguityInfo_v.push_back(loc_AmbiguityInfo_v[i]);
		}
		else{
			delete loc_AmbiguityInfo_v[i];
		}
	}
}


bool CAmbitor::BiMM_Segmentation_Ambiguity_Detection(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], vector<string>& pmToken_v, unsigned int mLength, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	vector<int> FMM_S_v;
	vector<int> BMM_S_v;
	m_A_CMaxmatch.Forword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, FMM_S_v);
	m_A_CMaxmatch.Backword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, BMM_S_v);

	Collecting_Ambiguity_in_two_Segmentation_Path(FMM_S_v, BMM_S_v, AmbiguityInfo_v, CAS_TOKEN, pmToken_v);
	return AmbiguityInfo_v.empty()?false:true;
}

bool  CAmbitor::Compromise_Combinational_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{

	for(int row = 0; row < (int)mlength; row++){
		for(int col = mlength-1; (col >= row) && (row < (int)mlength); col--){
			if(pmSegMatrix[row][col]){
				for(int incol = col-1; incol >= row; incol--){
					if(pmSegMatrix[row][incol] && pmSegMatrix[incol+1][col]){
						//if((col-row) < 2){
						//	continue;
						//}
						AmbiguintInfo* pd_AmbiguintInfo = new AmbiguintInfo;
						pd_AmbiguintInfo->START = row;
						pd_AmbiguintInfo->END = col;
						pd_AmbiguintInfo->A_START = 0;
						pd_AmbiguintInfo->A_END = 0;
						pd_AmbiguintInfo->OAS_CAS_TYPE = CAS_TOKEN;
						AmbiguityInfo_v.push_back(pd_AmbiguintInfo);
						break;
					}
				}
				row = col;
				break;
			}
		}
	}
	return AmbiguityInfo_v.empty()?false:true;
}

bool  CAmbitor::Combinational_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	bool Combinational_Flag = false;
	for(int row = 0; row < (int)mlength; row++)
	{
		for(int col = mlength-1; (col >= row) && (row < (int)mlength); col--)
		{
			if(pmSegMatrix[row][col])
			{
				if(col > row)
				{
					for(int inrow = row; inrow < col; inrow++)
					{
						for(int incol = col; incol > inrow; incol--)
						{
							if(pmSegMatrix[inrow][incol])
							{
								if(!((row == inrow) && (col == incol)))
								{
									AmbiguintInfo* pd_AmbiguintInfo = new AmbiguintInfo;
									pd_AmbiguintInfo->START = row;
									pd_AmbiguintInfo->END = col;
									pd_AmbiguintInfo->A_START = inrow;
									pd_AmbiguintInfo->A_END = incol;

									AmbiguityInfo_v.push_back(pd_AmbiguintInfo);

									if(!Combinational_Flag)
									{
										Combinational_Flag = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return Combinational_Flag;
}



bool  CAmbitor::Overlapping_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	bool Overlapping_Flag = false;
	for(int row = 0; row < (int)mlength; row++){
		for(int col = mlength-1; (col >= row) && (row < (int)mlength); col--){
			if(pmSegMatrix[row][col]){
				for(int inrow = row-1; inrow >= 0; inrow--){
					for(int incol = col - 1; incol >= row; incol--){
						if(pmSegMatrix[inrow][incol]){
							AmbiguintInfo* pD_AmbiguintInfo = new AmbiguintInfo;
							pD_AmbiguintInfo->START = inrow;
							pD_AmbiguintInfo->END = col;
							pD_AmbiguintInfo->A_START = row;
							pD_AmbiguintInfo->A_END = incol;
							pD_AmbiguintInfo->OAS_CAS_TYPE = OAS_TOKEN;
							AmbiguityInfo_v.push_back(pD_AmbiguintInfo);
							if(!Overlapping_Flag){
								Overlapping_Flag = true;
							}
						}
					}
				}
			}
		}
	}
	return Overlapping_Flag;
}

bool  CAmbitor::Maximum_Overlapping_Ambiguity_Detection(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	vector<AmbiguintInfo*> loc_AmbiguityInfo_v;
	Overlapping_Ambiguity_Detection(pmSegMatrix, mlength, loc_AmbiguityInfo_v);

	if(loc_AmbiguityInfo_v.empty()){
		return false;
	}
	else if(1 == loc_AmbiguityInfo_v.size()){
		AmbiguityInfo_v.push_back(*loc_AmbiguityInfo_v.begin());
		return true;
	}

	vector<AmbiguintInfo*>::iterator vite = loc_AmbiguityInfo_v.begin();
	AmbiguintInfo* p_T_Info = *vite;
	vite++;
	
	for(; vite != loc_AmbiguityInfo_v.end(); vite++){
		AmbiguintInfo* p_S_Info = *vite;
		if(p_T_Info->END < p_S_Info->START){
			p_T_Info->OAS_CAS_TYPE = MOAS_TOKEN;
			AmbiguityInfo_v.push_back(p_T_Info);
			p_T_Info = p_S_Info;
		}
		else{
			p_T_Info->A_END = p_S_Info->A_END;
			p_T_Info->END = p_S_Info->END;
			delete p_S_Info;
		}
	}
	//check_point++;
	//ostringstream ostrsteam;
	//ostrsteam << check_point;
	//CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	//app->pdlg->m_REditResponse.SetWindowTextW(NLPOP::string2CString(ostrsteam.str()));
	//if(19404 == check_point){
	//	bool break_flag = true;
	//}
	//TRACE(NLPOP::string2CString(ostrsteam.str()));
	//TRACE("\n");
	p_T_Info->OAS_CAS_TYPE = MOAS_TOKEN;
	AmbiguityInfo_v.push_back(p_T_Info);
	return true;
}


//======================Token Mark================================
bool  CAmbitor::Overlapping_Ambiguity_Detector_Use_Token_Mark(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark, list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& markite, CLAUSEPOS& pmCLAUSEPOS, bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength)
{
	tokenite = TokenList.erase(tokenite);
	markite = TokenMark.erase(markite);

	vector<string>::iterator vite = pmCLAUSEPOS.Clause.begin();

	bool Overlapping_Flag;
	bool return_value_flag;

	return_value_flag = false;

	for(int row = 0; row < (int)mlength; row++)
	{
		for(int col = mlength-1; (col >= row) && (row < (int)mlength); col--)
		{
			if(pmSegMatrix[row][col])
			{
				Overlapping_Flag = false;
				if(row > 0)
				{
					for(int inrow = row-1; inrow >= 0; inrow--)
					{
						for(int incol = col; incol >= row; incol--)
						{
							if(pmSegMatrix[inrow][incol])
							{
								Overlapping_Flag = true;
								break;
							}
						}
						if(Overlapping_Flag)
						{
							break;
						}
					}
				}
				if(!Overlapping_Flag)
				{
					TokenMark.insert(markite, CHINESE_TOKEN);
				}
				else
				{
					return_value_flag = true;
					markite--;
					*markite = AMBIGUITY_TOKEN;
					markite++;
					TokenMark.insert(markite, AMBIGUITY_TOKEN);
				}
				TokenList.insert(tokenite, (*vite));
				vite++;
				row = col;
				break;
			}
			else if(row == col)
			{
				TokenMark.insert(markite, CHINESE_TOKEN);
				TokenList.insert(tokenite, (*vite));
				vite++;
				break;
			}
		}
	}
	return return_value_flag;
}


bool CAmbitor::Ambiguity_Detector_Use_Token_Mark(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark, list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& markite, CLAUSEPOS& pmCLAUSEPOS, bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH])
{
	tokenite = TokenList.erase(tokenite);
	markite = TokenMark.erase(markite);

	bool DetectedAmbiguityFlag = false;
	bool RowDetectorFlag;
	bool ColumnDetectorFlag;
	bool BackAmbiguityNeedCopeFlag = false;
	string ssubtoken;
	string scurrentstr;
	char ambChar[3];
	ambChar[2] = 0;

	int passedtokencnt = 0;
	int lastRowAmbiguityPosit;
	for(vector<string>::iterator vite = pmCLAUSEPOS.Clause.begin(); vite != pmCLAUSEPOS.Clause.end(); vite++)
	{
		scurrentstr = *vite;
		int Insertedposit = 0;
		int wordtokencnt = 0;
		int i = 0;
		lastRowAmbiguityPosit = 0;
		for(; i < (int)scurrentstr.length(); wordtokencnt++, passedtokencnt++)
		{
			RowDetectorFlag = false;
			ColumnDetectorFlag = false;
			for(int c = MAX_CLAUSE_LENGTH; c > passedtokencnt ; c--)
			{
				RowDetectorFlag = (RowDetectorFlag || pmSegMatrix[passedtokencnt][c]);
				if(pmSegMatrix[passedtokencnt][c])
				{
					lastRowAmbiguityPosit = c;
				}
			}
			for(int r = passedtokencnt - 1; !(r < 0); r--)
			{
				ColumnDetectorFlag = (ColumnDetectorFlag || pmSegMatrix[r][passedtokencnt]);
			}
			if(RowDetectorFlag && ColumnDetectorFlag)
			{
				BackAmbiguityNeedCopeFlag = false;
				if(!DetectedAmbiguityFlag)
				{
					DetectedAmbiguityFlag = true;
				}
				//--insert subtoken before ambiguity
				if(Insertedposit < i)
				{
					ssubtoken = scurrentstr.substr(Insertedposit, i - Insertedposit);
					TokenList.insert(tokenite, ssubtoken);
					TokenMark.insert(markite, AMBIGUITY_TOKEN);
					Insertedposit = i;
				}
				ambChar[0] = scurrentstr.c_str()[i++];
				ambChar[1] = 0;
				if(ambChar[0] < 0)
				{
					ambChar[1] = scurrentstr.c_str()[i++];
				}
				Insertedposit = i;
				TokenList.insert(tokenite, ambChar);
				TokenMark.insert(markite, AMBIGUITY_TOKEN);
				continue;
			}
			if(scurrentstr.c_str()[i++] < 0)
			{
				i++;
			}
		}
		if(Insertedposit < i)
		{
			ssubtoken = scurrentstr.substr(Insertedposit, i - Insertedposit);
			TokenList.insert(tokenite, ssubtoken);
			if((Insertedposit == 0) && !BackAmbiguityNeedCopeFlag)
				TokenMark.insert(markite, CHINESE_TOKEN);
			else
				TokenMark.insert(markite, AMBIGUITY_TOKEN);
		}
		if(lastRowAmbiguityPosit < passedtokencnt)
		{
			BackAmbiguityNeedCopeFlag = false;
		}
		else
		{
			BackAmbiguityNeedCopeFlag = true;
		}
	}
	return DetectedAmbiguityFlag;

}
bool CAmbitor::Combinational_Ambiguity_Detector_Use_Token_Mark(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark, list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& markite, CLAUSEPOS& pmCLAUSEPOS, bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mlength)
{
	tokenite = TokenList.erase(tokenite);
	markite = TokenMark.erase(markite);

	vector<string>::iterator vite = pmCLAUSEPOS.Clause.begin();

	bool Combinational_Flag;
	bool return_value_flag;

	return_value_flag = false;

	for(int row = 0; row < (int)mlength; row++)
	{
		for(int col = mlength-1; (col >= row) && (row < (int)mlength); col--)
		{
			if(pmSegMatrix[row][col])
			{
				Combinational_Flag = false;

				if(col > row+1)
				{
					for(int inrow = row+1; inrow < col; inrow++)
					{
						for(int incol = col; incol > inrow; incol--)
						{
							if(pmSegMatrix[inrow][incol])
							{
								Combinational_Flag = true;
								break;
							}
						}
						if(Combinational_Flag)
						{
							break;
						}
					}
				}
				if(!Combinational_Flag)
				{
					TokenMark.insert(markite, CHINESE_TOKEN);
				}
				else
				{
					return_value_flag = true;
					TokenMark.insert(markite, AMBIGUITY_TOKEN);
				}
				TokenList.insert(tokenite, (*vite));
				vite++;
				row = col;
				break;
			}
		}
	}
	return return_value_flag;
}
//====================================================
bool CAmbitor::Is_BiMM_Ambiguity(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], int mlength, int START, int END)
{
	if(START >= mlength || END >= mlength){
		return false;
	}
	int row = START;
	int col = END;

	for(int row = START; row < mlength; ){
		for(int col = END; col >= row; col--){

		}
	}

	return false;
}

bool  CAmbitor::Is_Compromise_Combinational_Ambiguity(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], int mlength, int START, int END)
{
	if(END >= mlength || START >= mlength){
		return false;
	}
	int row = START;
	int col = END;

	if(pmSegMatrix[row][col]){
		for(int incol = col-1; incol >= row; incol--){
			if(pmSegMatrix[row][incol] && pmSegMatrix[incol+1][col]){
				//if((col-row) < 2){
				//	continue;
				//}
				return true;
			}
		}
	}
	return false;
}
bool CAmbitor::Is_Overlapping_Ambiguity(bool pmSegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], int mlength, int START, int END)
{
	if(END >= mlength || START >= mlength){
		return false;
	}
	for(int k = START+1; k < END; k++){
		if(pmSegMatrix[START][k] && pmSegMatrix[k][END]){
			return true;
		}
	}
	return false;
}

//=================================================================
void CAmbitor::Collecting_Ambiguity_in_two_Segmentation_Path(vector<int>& FMM_S_v, vector<int>& BMM_S_v, vector<AmbiguintInfo*>& AmbiguityInfo_v, SEGMENT_TOKEN_TYPE OAS_CAS_TYPE, vector<string>& TokenCS_v)
{
	int forwsameposit = -1;
	int A_START = 0;
	int FMMEnd = 0;
	int BMMEnd = 0;
	bool Ambiguity_Found_Flag = false;
	unsigned int f = 0, b = 0;

	vector<string> TestFMM_s, TestBMM_s;
	SMOP::Segmentation_Path_to_Segmentation_String(FMM_S_v, TokenCS_v, TestFMM_s);
	SMOP::Segmentation_Path_to_Segmentation_String(BMM_S_v, TokenCS_v, TestBMM_s);
	
	if(OAS_CAS_TYPE != CAS_TOKEN){
		bool FMM_Check_Flag = false;
		bool BMM_Check_Flag = false;
		for(; ((f < FMM_S_v.size()) && ( b < BMM_S_v.size())); ){
			if(FMM_S_v[f] == BMM_S_v[b]){
				if(FMM_Check_Flag != BMM_Check_Flag){//CAS Detected;
					Ambiguity_Found_Flag = false;
				}
				if(Ambiguity_Found_Flag){
					AmbiguintInfo* pt_AmbiguintInfo = new AmbiguintInfo;
					pt_AmbiguintInfo->START = forwsameposit+1;
					pt_AmbiguintInfo->A_START = A_START+1;
					if(FMMEnd < BMMEnd){
						pt_AmbiguintInfo->END = BMMEnd+1;
						pt_AmbiguintInfo->A_END = FMMEnd+1;
					}
					else{
						pt_AmbiguintInfo->END = FMMEnd+1;
						pt_AmbiguintInfo->A_END = BMMEnd+1;
					}
					pt_AmbiguintInfo->OAS_CAS_TYPE = OAS_CAS_TYPE;
					AmbiguityInfo_v.push_back(pt_AmbiguintInfo);
					Ambiguity_Found_Flag = false;
				}
				forwsameposit = FMM_S_v[f];
				f++;
				b++;
				FMM_Check_Flag = false;
				BMM_Check_Flag = false;
			}
			else{
				if(!Ambiguity_Found_Flag){
					Ambiguity_Found_Flag = true;
					if(FMM_S_v[f] < BMM_S_v[b]){
						A_START = FMM_S_v[f];
					}
					else{	
						A_START = BMM_S_v[b];
					}
				}
				if(FMM_S_v[f] < BMM_S_v[b]){
					FMMEnd = FMM_S_v[f];
					FMM_Check_Flag = true;
					f++;
				}
				else{	
					BMMEnd = BMM_S_v[b];
					BMM_Check_Flag = true;
					b++;
				}
			}
		}
	}
	else{
		bool F_less_B = false;
		bool CAS_Detected_Flag = false;
		for(; ((f < FMM_S_v.size()) && ( b < BMM_S_v.size())); ){
			if(FMM_S_v[f] == BMM_S_v[b]){
				if(Ambiguity_Found_Flag && CAS_Detected_Flag){
					AmbiguintInfo* pt_AmbiguintInfo = new AmbiguintInfo;
					pt_AmbiguintInfo->START = forwsameposit+1;
					pt_AmbiguintInfo->END = FMM_S_v[f];
					pt_AmbiguintInfo->A_START = 0;
					pt_AmbiguintInfo->A_END = 0;
					if(pt_AmbiguintInfo->START == pt_AmbiguintInfo->END){
						bool break_point = true;
					}
					pt_AmbiguintInfo->OAS_CAS_TYPE = OAS_CAS_TYPE;
					AmbiguityInfo_v.push_back(pt_AmbiguintInfo);
					Ambiguity_Found_Flag = false;
				}
				forwsameposit = FMM_S_v[f];
				f++;
				b++;
			}
			else{
				if(!Ambiguity_Found_Flag){
					Ambiguity_Found_Flag = true;
					CAS_Detected_Flag = true;
					if(FMM_S_v[f] < BMM_S_v[b]){
						F_less_B = true;
					}
					else{	
						F_less_B = false;
					}
				}
				else{
					if((FMM_S_v[f] < BMM_S_v[b]) && !F_less_B){
						CAS_Detected_Flag = false;
					}
					if((FMM_S_v[f] > BMM_S_v[b]) && F_less_B){
						CAS_Detected_Flag = false;
					}
				}
				if(FMM_S_v[f] < BMM_S_v[b]){
					f++;
				}
				else{
					b++;
				}
			}
		}
	}
}

void CAmbitor::Save_Ambiguity_Statistics_Info(string savepath)
{
	if(NLPOP::FolderExist(NLPOP::string2CString(savepath))){
		NLPOP::DeleteDir(NLPOP::string2CString(savepath));
	}
	_mkdir(savepath.c_str());

	//map<string, unsigned long>& AmbiStringInfo_m = pmCAmbitor.AmbiStringInfo_m;
	//map<string, unsigned long>& AmbiChainStringInfo_m = pmCAmbitor.AmbiChainStringInfo_m;
	//------------Save .txt file for init FrequentPatternSet(sub string.)	
	SMOP::Save_txt_map_string_long_with_Freq(AmbiStringInfo_m, savepath, "");
	//----Save Files Info-------------------------------------------------------------------
	multimap<unsigned long, string> SortedAmbiStringInfo_mm;
	for(map<string, unsigned long>::iterator mite = AmbiStringInfo_m.begin();  mite != AmbiStringInfo_m.end(); ){
		SortedAmbiStringInfo_mm.insert(make_pair((*mite).second, (*mite).first));
		mite = AmbiStringInfo_m.erase(mite);
	}
	SMOP::Save_Info_multimap_long_string(SortedAmbiStringInfo_mm, savepath + "Info.dat", "频率,数量,比率");
	//----Save .dat file----------------------------------------------------------
	SMOP::Save_Sorted_dat_multimap_string_long(SortedAmbiStringInfo_mm, savepath, "");
//=============================================================================
	if(AmbiChainStringInfo_m.empty()){
		return;
	}
	savepath += "Ambiguity_string_Length\\";

	_mkdir(savepath.c_str());
	//------------Save .txt file for init FrequentPatternSet(sub string.)	;
	SMOP::Save_txt_map_string_long_with_Freq(AmbiChainStringInfo_m, savepath, "");
	//----Save Files Info-------------------------------------------------------------------
	SortedAmbiStringInfo_mm.clear();

	for(map<string, unsigned long>::iterator mite = AmbiChainStringInfo_m.begin();  mite != AmbiChainStringInfo_m.end(); ){
		SortedAmbiStringInfo_mm.insert(make_pair((*mite).second, (*mite).first));
		mite = AmbiChainStringInfo_m.erase(mite);
	}
	SMOP::Save_Info_multimap_long_string(SortedAmbiStringInfo_mm, savepath + "Info.dat", "频率,数量,比率");
	//----Save .dat file----------------------------------------------------------
	SMOP::Save_Sorted_dat_multimap_string_long(SortedAmbiStringInfo_mm, savepath, "");
}
void CAmbitor::Collecting_Ambiguity_String_Info(vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	if(AmbiguityInfo_v.empty()){
		return;
	}
	//string ambiguitystr;
	char cAmbiBuf[Double_MAX_CLAUSE_LENGTH];
	for(vector<AmbiguintInfo*>::iterator vite = AmbiguityInfo_v.begin(); vite != AmbiguityInfo_v.end(); vite++){
		strcpy_s(cAmbiBuf, Double_MAX_CLAUSE_LENGTH, "");
		for(unsigned int i = (**vite).START; i <= (**vite).END; i++){
			strcat_s(cAmbiBuf, Double_MAX_CLAUSE_LENGTH, TokenCS_v[i].c_str());
		}
		if(AmbiStringInfo_m.find(cAmbiBuf) == AmbiStringInfo_m.end()){
			AmbiStringInfo_m.insert(make_pair(cAmbiBuf, 1));
		}
		else{
			AmbiStringInfo_m[cAmbiBuf]++;
		}
		if((*vite)->OAS_CAS_TYPE != CAS_TOKEN){
			strcpy_s(cAmbiBuf, Double_MAX_CLAUSE_LENGTH, "");
			for(unsigned int i = (**vite).A_START; i <= (**vite).A_END; i++){
				strcat_s(cAmbiBuf, Double_MAX_CLAUSE_LENGTH, TokenCS_v[i].c_str());
			}
			if(AmbiChainStringInfo_m.find(cAmbiBuf) == AmbiChainStringInfo_m.end()){
				AmbiChainStringInfo_m.insert(make_pair(cAmbiBuf, 1));
			}
			else{
				AmbiChainStringInfo_m[cAmbiBuf]++;
			}
		}
	}
}