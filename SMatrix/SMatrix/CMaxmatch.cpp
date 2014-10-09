#include "StdAfx.h"
#include "CMaxmatch.h"


CMaxmatch::CMaxmatch()
{
	/*vector<string> segment_v;
	set<string> pmWordSet;
	string locstr = "我曾多次看《法门寺》这一出戏，我非常欣赏演员们的表演艺术";
	Maximum_Match_Segmentation(locstr.c_str(), segment_v, pmWordSet, 0);*/
}
CMaxmatch::~CMaxmatch()
{
}


void CMaxmatch::Maximum_Match_Segmentation_Port(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<string>& TokenCS_v, vector<int>& Seg_v)
{
	if(true){//FMM Segmentation
		Forword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, Seg_v);
	}
	if(false){//BMM Segmentation
		Backword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, Seg_v);
	}
	/*if(!m_CAmbitor.Bi_Directional_Maximum_Matching_Detector(SegMatrix, mLength)){
		tokenite = TokenList.erase(tokenite);
		tmarkite = TokenMark.erase(tmarkite);
		m_CMaxmatch.Forword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, Seg_v);
		for(vector<string>::iterator vite = TempPos.Clause.begin();  vite != TempPos.Clause.end(); vite++){
			TokenList.insert(tokenite, *vite);
			TokenMark.insert(tmarkite, CHINESE_TOKEN);
		}
	}
	else{
		m_CAmbitor.Combinational_Ambiguity_Detector_Use_Token_Mark(TokenList, TokenMark, tokenite, tmarkite, TempPos, SegMatrix, mLength);
		pmADectorFlag = true;
		//if(m_CAmbitor.Ambiguity_Detector_Use_Token_Mark(TokenList, TokenMark, tokenite, tmarkite, TempPos, SegMatrix)){
		//	pmADectorFlag = true;
		//	if(!pmADectorFlag){
		//		pmADectorFlag = true;
		//	}
		//}
	}*/
}

//0: Forward Maximum;
//1: Backward Maximum;
void CMaxmatch::Maximum_Match_Segmentation(const char* Sentbuf, vector<string>& segment_v, set<string>& pmWordSet, DWORD SegmentType)
{
	/*ZeroMemory(SegMatrix, MAX_CLAUSE_LENGTH*MAX_CLAUSE_LENGTH*sizeof(bool));
	unsigned int mLength = 0;
	segment_v.clear();

	vector<int> segInt_v;
	vector<string> TokenCS_v;
	if(NLPOP::Get_Chinese_Sentence_Length_Counter(Sentbuf) > MAX_CLAUSE_LENGTH){
		size_t length = strlen(Sentbuf);
		size_t cnt = 0;
		size_t i = 0;
		if(0 == SegmentType){
			char* locsenchar = new char[length+1];
			strcpy_s(locsenchar, length+1, Sentbuf);
			for( ; i < length; ){
				if(!(cnt++ < MAX_CLAUSE_LENGTH)){
					break;
				}
				if(locsenchar[i++] < 0 ){
					i++;
				}	
			}
			locsenchar[i] = 0;
			Generate_Segmentation_Matrix(locsenchar, pmWordSet, TokenCS_v, MAX_CLAUSE_LENGTH, SegMatrix, mLength);
			delete locsenchar;
		}
		else if(1 == SegmentType){
			size_t locBeg = NLPOP::Get_Chinese_Sentence_Length_Counter(Sentbuf)-MAX_CLAUSE_LENGTH;
			for( ; i < length; ){
				if(!(cnt++ < locBeg)){
					break;
				}
				if(Sentbuf[i++] < 0 ){
					i++;
				}				
			}
			Generate_Segmentation_Matrix(Sentbuf+i*sizeof(char), pmWordSet, TokenCS_v, MAX_CLAUSE_LENGTH, SegMatrix, mLength);
		}
	}
	else{
		Generate_Segmentation_Matrix(Sentbuf, pmWordSet, TokenCS_v, MAX_CLAUSE_LENGTH, SegMatrix, mLength);
	}
	if(0 == SegmentType){
		Forword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, segInt_v);
	}
	else if(1 == SegmentType){
		Backword_Max_Maximum_Matching_Segmentation(SegMatrix, mLength, segInt_v);
	}*/
}


void CMaxmatch::Generate_Segmentation_Matrix(const char* SentBuf, set<string>& locCSWords_Set, vector<string>& TokenCS_v, int Max_Word_Length, bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int& mLength)
{
	ZeroMemory(SegMatrix, MAX_CLAUSE_LENGTH*MAX_CLAUSE_LENGTH*sizeof(bool));
	
	char sChar[3];
	int strLen = strlen(SentBuf);
	if(Max_Word_Length <= 0 || Max_Word_Length > MAX_CLAUSE_LENGTH){
		Max_Word_Length = strLen;
	}
	int rowtokenlength = 0;
	int coltokenlength = 0;
	string wordstr;
	sChar[2]=0;
	for(int forposit = 0; forposit < strLen; ){
		wordstr = "";
		for(int bacposit = 0; ((forposit + bacposit) < strLen) && (bacposit < (int)Max_Word_Length); ){
			sChar[0]=SentBuf[forposit + bacposit++];
			sChar[1]=0;	
			if(sChar[0] < 0 ){
				sChar[1]=SentBuf[forposit + bacposit++];
			}
			wordstr += sChar;
			if(locCSWords_Set.find(wordstr) != locCSWords_Set.end()){
				SegMatrix[rowtokenlength][coltokenlength] = true;;
			}
			coltokenlength++;
		}
		sChar[0]=SentBuf[forposit++];
		sChar[1]=0;	
		if(sChar[0] < 0 ){
			sChar[1]=SentBuf[forposit++];
		}
		TokenCS_v.push_back(sChar);
		rowtokenlength++;
		coltokenlength = rowtokenlength;
	}
	mLength = rowtokenlength;
}

void CMaxmatch::Forword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v)
{
	for(int row = 0; row < (int)mLength; ){
		int col = mLength - 1;
		for( ; col >= row; col--){
			if(SegMatrix[row][col]){
				Seg_v.push_back(col);
				break;
			}
			else if(col == row){
				Seg_v.push_back(col);
				break;
			}
		}
		row = col + 1;
	}
}

void CMaxmatch::Backword_Max_Maximum_Matching_Segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<int>& Seg_v)
{
	vector<int> rSeg_v;
	for(int col = mLength - 1 ; col >= 0; ){
		int row;
		for(row = 0; row < (int)mLength; row++){
			if(SegMatrix[row][col]){
				rSeg_v.push_back(col);
				break;
			}
			else if(col == row){
				rSeg_v.push_back(col);
				break;
			}
		}
		col = row - 1;
	}
	for(unsigned int i = 0; i < rSeg_v.size(); i++){
		Seg_v.push_back(rSeg_v[rSeg_v.size()-1-i]);
	}
}



//------
void CMaxmatch::Omni_segmentation(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, int asSTART, int asEND, vector<vector<int>*>& Rtndeque_vv)
{
	multimap<int,int> wordinfo_mm;
	for(int i = asSTART; i <= asEND; i++){
		for(int j = i; ((j < (int)mLength) && (j <= asEND)); j++){
			if(SegMatrix[i][j]){
				wordinfo_mm.insert(make_pair(i,j));
			}

		}
	}
	vector<int>* pS_v = new vector<int>;
	int length = 0;
	Recersive_Breadth_First_Omni_segmentation(asSTART, asEND, wordinfo_mm, pS_v, Rtndeque_vv);
}

void CMaxmatch::Recersive_Breadth_First_Omni_segmentation(int position, int endposit, multimap<int, int>& pmMultiMap, vector<int>* pS_v, vector<vector<int>*>& Rtndeque_vv)
{
	if(!(Rtndeque_vv.size() < 8)){
		size_t max_length = 0;
		for(size_t i = 0; i < Rtndeque_vv.size(); i++){
			if(Rtndeque_vv[i]->size() > max_length){
				max_length = Rtndeque_vv[i]->size();
			}
		}
		if(pS_v->size()+1 >= max_length){
			delete pS_v;
			return;
		}
	}
	if(pmMultiMap.find(position) != pmMultiMap.end()){
		pair<multimap<int, int>::iterator,multimap<int, int>::iterator> m_Multimap_Pair = pmMultiMap.equal_range(position);
		multimap<int, int>::iterator back_ite = m_Multimap_Pair.second;
		back_ite--;
		while(true){
			vector<int>* ploc_S = new vector<int>;
			*ploc_S = *pS_v;
			ploc_S->push_back(back_ite->second);
			Recersive_Breadth_First_Omni_segmentation(back_ite->second+1, endposit, pmMultiMap, ploc_S, Rtndeque_vv);
			if(back_ite == m_Multimap_Pair.first){
				break;
			}
			back_ite--;
		}
		delete pS_v;
	}
	else if(position != endposit){
		delete pS_v;
		return;
	}
	else {
		if(Rtndeque_vv.size() < 8){
			Rtndeque_vv.push_back(pS_v);
		}
		else{
			bool Inserted_Flag = false;
			for(size_t i = 0; i < Rtndeque_vv.size(); i++){
				if(Rtndeque_vv[i]->size() > pS_v->size()){
					delete Rtndeque_vv[i];
					Rtndeque_vv[i] = pS_v;
					Inserted_Flag = true;
					break;
				}
			}
			if(!Inserted_Flag){
				delete pS_v;
			}
		}
	}
}
void CMaxmatch::Recersive_Omni_segmentation(int position, multimap<int, int>& pmMultiMap, vector<int> S, deque<vector<int>>& Rtndeque)
{
	
	if(pmMultiMap.find(position) != pmMultiMap.end())
	{
		pair<multimap<int, int>::iterator,multimap<int, int>::iterator> m_Multimap_Pair = pmMultiMap.equal_range(position);
		while(m_Multimap_Pair.first != m_Multimap_Pair.second)
		{
			S.push_back((*m_Multimap_Pair.first).second);
			Recersive_Omni_segmentation((*m_Multimap_Pair.first).second+1, pmMultiMap, S, Rtndeque);
			S.pop_back();
			m_Multimap_Pair.first++;
		}
	}
	else
	{
		Rtndeque.push_back(S);
	}
}
//------
void CMaxmatch::Maximum_no_cover_Ambiguity_Graph(bool SegMatrix[MAX_CLAUSE_LENGTH][MAX_CLAUSE_LENGTH], unsigned int mLength, vector<string>& TokenCS_v, vector<vector<int>*>& Rtndeque_vv)
{
	multimap<int,int> wordinfo_mm;
	for(unsigned int i = 0; i < mLength; i++){
		for(unsigned int j = i; j < mLength; j++){
			if(SegMatrix[i][j]){
				wordinfo_mm.insert(make_pair(i,j));
			}
		}
	}
	pair<multimap<int, int>::iterator,multimap<int, int>::iterator> m_Multimap_Pair = wordinfo_mm.equal_range(wordinfo_mm.begin()->first);
	multimap<int, int>::iterator back_ite = m_Multimap_Pair.second;
	back_ite--;
	while(m_Multimap_Pair.first != m_Multimap_Pair.second){
		vector<int>* pS_v = new vector<int>;
		pS_v->push_back(m_Multimap_Pair.first->second);
		Recersive_Maximum_no_cover_Ambiguity_Graph(m_Multimap_Pair.first->second+1, back_ite->second, mLength, wordinfo_mm, pS_v, Rtndeque_vv);
		m_Multimap_Pair.first++;
	}
}
void CMaxmatch::Recersive_Maximum_no_cover_Ambiguity_Graph(int START, int back_point, int mLength, multimap<int, int>& pmMultiMap, vector<int>* pS_v, vector<vector<int>*>& Rtndeque_vv)
{
	if(pmMultiMap.find(START) != pmMultiMap.end()){
		pair<multimap<int, int>::iterator,multimap<int, int>::iterator> m_Multimap_Pair = pmMultiMap.equal_range(START);
		multimap<int, int>::iterator back_ite = m_Multimap_Pair.second;
		back_ite--;
		while(m_Multimap_Pair.first != m_Multimap_Pair.second){
			if(m_Multimap_Pair.first->second <= back_point){
				m_Multimap_Pair.first++;
				continue;
			}
			vector<int>* ploc_S = new vector<int>;
			*ploc_S = *pS_v;
			ploc_S->push_back(m_Multimap_Pair.first->second);
			Recersive_Maximum_no_cover_Ambiguity_Graph(m_Multimap_Pair.first->second+1, back_ite->second, mLength, pmMultiMap, ploc_S, Rtndeque_vv);
			m_Multimap_Pair.first++;
		}
		delete pS_v;
	}
	else if(START != mLength){
		delete pS_v;
	}
	else{
		Rtndeque_vv.push_back(pS_v);
	}

}
