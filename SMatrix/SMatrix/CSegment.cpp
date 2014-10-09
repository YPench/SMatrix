#include "StdAfx.h"
#include "CSegment.h"

extern size_t Segmenting_n_Gram;
extern bool Segmenting_CRF_Training_Flag;
extern bool Segmenting_CRF_Testing_Flag;

CSegment::CSegment(Clexical& m_Clexical) : m_Clexical(m_Clexical), m_CCutor(m_Clexical)
{
	un_amboguity_cnt = 0;
	totalsent_cnt = 0;
	Max_Word_Length = 0;
	CheckPoint = 0;

	//Read_CRF_Segmenting_Path_Resusts("F:\\YPench\\SMatrix\\CRF\\output.txt", m_SPathStr_vv);

	//MAXEN::Maxen_Training_with_Erasing(Training_v, pm_maxent, pCCEDT->m_CanditIte);
}
CSegment::~CSegment()
{
}

void CSegment::Sentence_Segmenting_Port(const char* SentBuf, CLAUSEPOS& SentPos)
{
	if(strlen(SentBuf) == 0){
		return;
	}
	list<string> TokenList;
	list<SEGMENT_TOKEN_TYPE> TokenMark;
	bool Ambiguity_Detected_Flag;

	//=====Segment=================按非中文字符切分...=================================
	m_CCutor.CCutor_Port(SentBuf, TokenList, TokenMark);

	//=====Ambiguity Detection
	Ambiguity_Detected_Flag = Sentence_Ambiguity_Detecting_and_Segmentation(TokenList, TokenMark);

	if(!Ambiguity_Detected_Flag){
		un_amboguity_cnt++;
	}
	totalsent_cnt++;

	//=============================歧义词的合并...===========================================
	//---Merge ambiguity UniCouple
	//CSegment_Port(TokenList, TokenMark);

	//=============================记录分词类型================================================
	//m_CAmbitor.Ambiguity_Type_Counter(TokenList, TokenMark);
	//=============================返回分词结果...===========================================
	for(list<string>::iterator lite = TokenList.begin(); lite != TokenList.end(); lite++){
		SentPos.Clause.push_back(*lite);
	}
}
bool CSegment::Sentence_Ambiguity_Detecting_and_Segmentation(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark)
{
	set<string>& SnotationSet = m_Clexical.IcwbWords_set;
	list<string>::iterator tokenite;
	list<SEGMENT_TOKEN_TYPE>::iterator tmarkite;
	bool Ambiguity_Detected_Flag = false;
	bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH];
	
	tokenite = TokenList.begin();
	tmarkite = TokenMark.begin();
	size_t loc_SentPosit;
	m_p_Sent;
	for( ; tmarkite != TokenMark.end(); m_SentPosit += loc_SentPosit){
		loc_SentPosit = NLPOP::Get_Chinese_Sentence_Length_Counter(tokenite->data());
		if(!(*tmarkite == CHINESE_TOKEN)){
			tokenite++; 
			tmarkite++;
			continue;
		}
		if(!(tokenite->length() > 2)){
			tokenite++; 
			tmarkite++;
			continue;
		}

		vector<string> TokenCS_v;
		vector<int> Seg_v;
		unsigned int mLength = 0;
		vector<AmbiguintInfo*> AmbiguityInfo_v;
		//---Generating segmentation Matrix
		m_CMaxmatch.Generate_Segmentation_Matrix(tokenite->data(), SnotationSet, TokenCS_v, Max_Word_Length, SegMatrix, mLength);

		//---Ambiguity Detection
		if(false){//
			m_CAmbitor.Segmentation_Ambiguity_Detecting_Port(SegMatrix, mLength, TokenCS_v, AmbiguityInfo_v);
		}

		//---Segmentation Path Ambiguity Labelling
		m_CAmbitor.Segmentation_Ambiguity_Labelling_Port(SegMatrix, mLength, Seg_v, TokenCS_v, AmbiguityInfo_v);
		
		if(Segmenting_CRF_Training_Flag || Segmenting_CRF_Testing_Flag){//---for Training
			Collecting_MOAS_Ambiugity_Segmenting_String_for_Training(mLength, Seg_v, TokenCS_v, AmbiguityInfo_v);
		}

		//---Segmentation
		//m_CMaxmatch.Maximum_Match_Segmentation_Port(SegMatrix, mLength, TokenCS_v, Seg_v);

	
		//---For Viewing
		//vector<string> outputsegment_v;
		//SMOP::Segmentation_Path_to_Segmentation_String(Seg_v, TokenCS_v, outputsegment_v);

		tokenite = TokenList.erase(tokenite);
		tmarkite = TokenMark.erase(tmarkite);

		//SMOP::Segmentation_Path_to_Segmentation_String(TokenList, TokenMark, tokenite, tmarkite, Seg_v, TokenCS_v);

		for(size_t i = 0; i < AmbiguityInfo_v.size(); i++){
			delete AmbiguityInfo_v[i];
		}
	}
	return Ambiguity_Detected_Flag;

}


void CSegment::Revised_Ambiguity_Info(vector<AmbiguintInfo*>& AmbiguityInfo_v, SEGMENT_TOKEN_TYPE TYPE)
{
	vector<int>& loc_SPath = *m_p_SegmentingPath;
	map<size_t, size_t> words_m;
	bool Nested_in_Word_Flag;
	size_t START;
	size_t END;
	START = 0;
	if(TYPE != CAS_TOKEN){
		for(size_t i = 0; i < loc_SPath.size(); i++){
			words_m.insert(make_pair(START, loc_SPath[i]));
			START = loc_SPath[i] + 1;
		}
	}
	for(size_t i = 0; i < AmbiguityInfo_v.size(); i++){
		if(TYPE != AmbiguityInfo_v[i]->OAS_CAS_TYPE){
			AmbiguityInfo_v[i]->OAS_CAS_TYPE = UNDEFINED_TOKEN;
			continue;
		}
		AmbiguintInfo& loc_Ref = *AmbiguityInfo_v[i];
		START = loc_Ref.START + m_SentPosit;
		END = loc_Ref.END + m_SentPosit;
		if(TYPE != CAS_TOKEN){
			Nested_in_Word_Flag = false;
			for(map<size_t, size_t>::iterator mite = words_m.begin(); mite != words_m.end(); mite++){
				if(mite->first <= START && mite->second >= END){
					AmbiguityInfo_v[i]->OAS_CAS_TYPE = UNDEFINED_TOKEN;
					Nested_in_Word_Flag = true;
					break;
				}
			}
			if(Nested_in_Word_Flag){
				continue;
			}
		}
		loc_Ref.A_START = START<Segmenting_n_Gram?0:START-Segmenting_n_Gram;
		loc_Ref.A_END = END + Segmenting_n_Gram;
		if(!(loc_Ref.A_END < m_SentLength)){
			loc_Ref.A_END = m_SentLength - 1;
		}
	}
}




void CSegment::Collecting_MOAS_Ambiugity_Segmenting_String_for_Training(unsigned int mLength, vector<int>& Seg_v, vector<string>& TokenCS_v, vector<AmbiguintInfo*>& AmbiguityInfo_v)
{
	map<int, map<int, AmbiguintInfo*>> Ambiguities_mm;

	Revised_Ambiguity_Info(AmbiguityInfo_v, MOAS_TOKEN);

	SMOP::AmbiguityInfo_v_to_Ambiguities_mm(AmbiguityInfo_v, Ambiguities_mm);
	
	vector<int>& loc_SPath = *m_p_SegmentingPath;
	vector<string>& loc_SPathStr = *m_p_SPathStr_v;
	if(Ambiguities_mm.empty()){
		return;
	}
	map<int, map<int, AmbiguintInfo*>>::iterator beginite = Ambiguities_mm.begin();
	map<int,AmbiguintInfo*>::iterator secite  = beginite->second.begin();
	vector<string>* ploc_SP_v = new vector<string>;
	//-------------Testing--------------
	if(++CheckPoint == 2){
		bool check_flag = false;
	}
	for(; m_i < loc_SPath.size(); m_i++){
		if(loc_SPath[m_i] >= beginite->first){
			for(size_t j = m_i + 1; j < loc_SPath.size(); j++){
				if(loc_SPath[j] > secite->first){
					ploc_SP_v->push_back(loc_SPathStr[j-1]);
					if(loc_SPath[j-1]+1 <= secite->first){
						ploc_SP_v->push_back(loc_SPathStr[j]);
					}
					//---push back
					m_SPathStr_vv.push_back(ploc_SP_v);
					ploc_SP_v = new vector<string>;
					secite = beginite->second.erase(secite);
					if(secite == beginite->second.end()){
						beginite = Ambiguities_mm.erase(beginite);
						if(beginite != Ambiguities_mm.end()){
							secite = beginite->second.begin();
							while(loc_SPath[m_i] >= beginite->first && m_i > 0){
								m_i--;
							}
						}
						else{
							delete ploc_SP_v;
						}
					}
					break;
				}
				else{
					ploc_SP_v->push_back(loc_SPathStr[j-1]);
				}
				if(secite == beginite->second.end()){
					break;
				}
			}
		}
		if(beginite == Ambiguities_mm.end()){
			break;
		}
	}
}


void CSegment::Output_SPathStr_for_CRF(const char* cFoldPath)
{
	char sChar[3];
	sChar[2]=0;
	ofstream out(cFoldPath);
	if(out.bad()){
		return;
	}
	out.clear();
	out.seekp(0, ios::beg);

	for(size_t i = 0; i < m_SPathStr_vv.size(); i++){
		vector<string>& locRef_v = *m_SPathStr_vv[i];
		if(locRef_v.empty()){
			continue;
		}
		for(size_t j = 0; j < locRef_v.size(); j++){
			const char* cSentBuf = locRef_v[j].c_str();
			size_t limit = locRef_v[j].length();
			bool START_Flag = true;
			for(size_t k = 0; k < limit; ){
				sChar[0] = cSentBuf[k++];
				sChar[1] = 0;
				if(sChar[0] < 0){
					sChar[1] = cSentBuf[k++];
				}
				out << sChar << "\t";
				if(START_Flag){
					out << "S";
					START_Flag = false;
				}
				else{
					out << "O";
				}
				out << endl;

			}
		}
		out << endl;
	}
		
	out.close();

}

void CSegment::Read_CRF_Segmenting_Path_Resusts(const char* outputpath, vector<vector<string>*>& pm_SPathStr_vv)
{
	char getlineBuf[MAX_SENTENCE];
	char charBuf[MAX_SENTENCE];
	char wordBuf[MAX_SENTENCE];

	ifstream in(outputpath);
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);

	while(true){
		vector<string>* ploc_v = new vector<string>;
		strcpy_s(wordBuf, MAX_SENTENCE, "");
		while(true){
			in.getline(getlineBuf, MAX_SENTENCE, '\n');
			if(0 == strlen(getlineBuf)){
				if(0 != strlen(wordBuf)){
					ploc_v->push_back(wordBuf);
				}
				break;
			}
			istringstream instream(getlineBuf);
			instream.getline(getlineBuf, MAX_SENTENCE,'\t');
			strcpy_s(charBuf, MAX_SENTENCE, getlineBuf);
			instream.getline(getlineBuf, MAX_SENTENCE,'\t');
			instream.getline(getlineBuf, MAX_SENTENCE,'\n');
			if(!strcmp(getlineBuf, "S")){
				if(0 != strlen(wordBuf)){
					ploc_v->push_back(wordBuf);
					strcpy_s(wordBuf, MAX_SENTENCE, "");
				}
			}
			strcat_s(wordBuf, MAX_SENTENCE, charBuf);
		}
		pm_SPathStr_vv.push_back(ploc_v);
		ploc_v = new vector<string>;
		if(in.peek() == EOF){
			delete ploc_v;
			break;
		}
	}
	in.close();

}


void CSegment::Output_Ambiguity_Info(const char* cFoldPath)
{
	m_CAmbitor.Save_Ambiguity_Statistics_Info(cFoldPath);
}

