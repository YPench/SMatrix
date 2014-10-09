#include "stdafx.h"
#include "SMCom.h"

#include "ConvertUTF.h"



void AppCall::Secretary_Hide()
{
}
void AppCall::Secretary_Show()
{
}

void AppCall::Response_Diplay(string sout, DWORD MessageType = 0)
{
	AppCall::Secretary_Message_Box(sout, MessageType);
}

void AppCall::Maxen_Responce_Message(const char* poutstr)
{
	//STeller_Responce_Message(poutstr);
}
void AppCall::Maxen_Responce_Message_with_Save(const char* poutstr)
{
	//STeller_Responce_Message_with_Save(poutstr);
}

int AppCall::Secretary_Message_Box(const char* cmsg)
{
	return AppCall::Secretary_Message_Box(cmsg, MB_OK);
}

int AppCall::Secretary_Message_Box(string msgstr, UINT nType = MB_OK)
{
	//CConsensusApp *app = (CConsensusApp *)AfxGetApp();
	int rtn = AfxMessageBox(NLPOP::string2CString(msgstr), nType);
	return rtn;
}

void AppCall::Consensus_Responce_Message(string msgstr, UINT nType)
{
//	static string outpusmsg = "";
//	CConsensusApp *app = (CConsensusApp *)AfxGetApp();
//	app->pdlg->Output_MSG(msgstr, nType);
	//outpusmsg = msgstr;
//	DWORD msgphreadId =app->pdlg->msgphreadId;
	//PostThreadMessage(app->pdlg->msgphreadId, WM_MESSAGE, (WPARAM)nType, (LPARAM)outpusmsg.c_str());
}

void SMOP::AmbiguityInfo_v_to_Ambiguities_mm(vector<AmbiguintInfo*>& AmbiguityInfo_v, map<int, map<int, AmbiguintInfo*>>& Ambiguities_mm)
{
	size_t START;
	size_t END;
	for(size_t i = 0; i < AmbiguityInfo_v.size(); i++){
		if(UNDEFINED_TOKEN == AmbiguityInfo_v[i]->OAS_CAS_TYPE){
			continue;
		}
		AmbiguintInfo& loc_Ref = *AmbiguityInfo_v[i];
		START = loc_Ref.A_START;
		END = loc_Ref.A_END;
		if(Ambiguities_mm.find(START) == Ambiguities_mm.end()){
			Ambiguities_mm[START];
		}
		if(Ambiguities_mm[START].find(END) == Ambiguities_mm[START].end()){
			Ambiguities_mm[START][END];
			Ambiguities_mm[START][END] = AmbiguityInfo_v[i];
		}
		else{
			AfxMessageBox(_T(""), MB_OK);
		}
	}
}

void SMOP::Generating_Segmentation_Path(vector<string>& RefSPath_v, vector<vector<int>*>& SPath_vv, vector<vector<string>*>& SegmentingPathStr_vv)
{
	char SentenceBuf[MAX_SENTENCE];
	int k;
	for(size_t i = 0; i < RefSPath_v.size(); i++){
		k = -1;
		istringstream sstream(RefSPath_v[i].c_str());
		vector<int>* ploc_v = new vector<int>;
		vector<string>* plocstr_v = new vector<string>;
		while(true){
			while(sstream.peek() == ' '){	
				sstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(sstream.peek() == EOF){
				break;
			}
			sstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			size_t limit = strlen(SentenceBuf);
			for(size_t j = 0; j < limit; ){
				if(SentenceBuf[j++] < 0){
					j++;
				}
				k++;
			}
			ploc_v->push_back(k);
			plocstr_v->push_back(SentenceBuf);
		}
		SPath_vv.push_back(ploc_v);
		SegmentingPathStr_vv.push_back(plocstr_v);
	}
}


void SMOP::Segmentation_Path_to_Segmentation_String(vector<int>& pmSeg_v, vector<string>& pmTokenCS, vector<string>& pmSegs_v)
{
	unsigned int startpoint = 0;
	unsigned int backpoint = 0;
	char cWordBuf[Double_MAX_CLAUSE_LENGTH];
	for(unsigned int i = 0; i < pmSeg_v.size(); i++){
		strcpy_s(cWordBuf, Double_MAX_CLAUSE_LENGTH, "");
		backpoint = pmSeg_v[i];
		for(unsigned int j = startpoint; j <= backpoint; j++){
			strcat_s(cWordBuf, Double_MAX_CLAUSE_LENGTH, pmTokenCS[j].c_str());
		}
		pmSegs_v.push_back(cWordBuf);
		startpoint = backpoint;
		startpoint++;
	}
}

void SMOP::Segmentation_Path_to_Segmentation_String(list<string>& TokenList, list<SEGMENT_TOKEN_TYPE>& TokenMark,list<string>::iterator& tokenite, list<SEGMENT_TOKEN_TYPE>::iterator& tmarkite, vector<int>& pmSeg_v, vector<string>& pmTokenCS)
{
	unsigned int startpoint = 0;
	unsigned int backpoint = 0;
	char cWordBuf[Double_MAX_CLAUSE_LENGTH];
	for(unsigned int i = 0; i < pmSeg_v.size(); i++){
		strcpy_s(cWordBuf, Double_MAX_CLAUSE_LENGTH, "");
		backpoint = pmSeg_v[i];
		for(unsigned int j = startpoint; j <= backpoint; j++){
			strcat_s(cWordBuf, Double_MAX_CLAUSE_LENGTH, pmTokenCS[j].c_str());
		}
		TokenList.insert(tokenite, cWordBuf);
		TokenMark.insert(tmarkite, CHINESE_TOKEN);
		startpoint = backpoint;
		startpoint++;
	}
}


void SMOP::Save_CLAUSEPOS_deque_Without_Icwb_POS(string pmSavePath, vector<CLAUSEPOS>& pmPasedPos)
{
	ofstream out(pmSavePath.c_str());
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);
	for(vector<CLAUSEPOS>::iterator vite = pmPasedPos.begin(); vite != pmPasedPos.end(); vite++)
	{
		for(vector<string>::iterator vvite = (*vite).Clause.begin(); vvite != (*vite).Clause.end(); vvite++)
		{
			out << *vvite << "  ";
		}
		out << endl;
	}
	out.close();
}

void SMOP::Save_txt_map_string_long_with_Freq(map<string, unsigned long>& pmmap, string pmSavePath, string pmmemostr = "")
{
	if(pmmap.empty())
		return;
	//CSecretaryApp *app = (CSecretaryApp *)AfxGetApp();
	char charresponse[1024];
	char intchar[64];
	map<string, unsigned long>::iterator mmite = pmmap.begin();
	unsigned int filenamesequence = 0;
	ostringstream ostrsteam;

	while(true)
	{
		string sOutFileName;
		ostrsteam.str("");
		ostrsteam << filenamesequence++ << ".txt";
		sOutFileName = ostrsteam.str();
		sOutFileName = pmSavePath +sOutFileName;
					
		ofstream out(sOutFileName.c_str());
		if(out.bad())
			continue;
		out.clear();
		out.seekp(0, ios::beg);
//--------------------------save result file;
		unsigned int sentecnecnt = 0;
		while(true)
		{
			out << (*mmite).first  << '\t' << (*mmite).second << "\n";
			sentecnecnt++;
			mmite++;
			if(!(sentecnecnt < EVERY_FILE_NUMBER) || (mmite == pmmap.end()))
				break;
		}
		out.close();

		_itoa_s(filenamesequence,intchar,64,10);
		strcpy_s(charresponse, "保存结果:");
		strcat_s(charresponse, intchar);
		strcat_s(charresponse, ".txt");
		//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);

		if(mmite == pmmap.end())
		{
			break;
		}
	}
	
}


void SMOP::Save_Info_multimap_long_string(multimap<unsigned long, string>& pmMultimap, string pmSaveName, string pmmemostr = "")
{
	if(pmMultimap.empty())
		return;
	LPSYSTEMTIME lpSystemTime = new  SYSTEMTIME;
	ostringstream ostrsteam;
	set<unsigned long> LenthTypSet;
	unsigned long TotalClauseFreqCount;
	pair<multimap<unsigned long, string>::iterator, multimap<unsigned long, string>::iterator> m_Multimap_Pair;

	TotalClauseFreqCount = 0;
	for(multimap<unsigned long, string>::iterator mmite = pmMultimap.begin();  mmite != pmMultimap.end(); mmite++)
	{
		TotalClauseFreqCount += (*mmite).first;
		LenthTypSet.insert((*mmite).first);
	}

	ofstream out(pmSaveName.c_str());
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);
	ostrsteam.str("");

	out << "#";//-------------------date;
	GetLocalTime(lpSystemTime);
	ostrsteam << lpSystemTime->wYear << '_' << lpSystemTime->wMonth << '_' << lpSystemTime->wDay << ' ' << lpSystemTime->wHour << ':' << lpSystemTime->wMinute << ':' << lpSystemTime->wSecond << "\n";
	string datastr = "日期：";
	datastr += ostrsteam.str();
	out << datastr;
	out << "#";//-------------------author
	out << "Author:" << "YPench" << "\n";
	out << "#";//-------------------
	out << "\n";
	out << "#";//-------------------
	out << "句子总数：" << TotalClauseFreqCount << "\n";
	out << "#";//-------------------
	out << "\n";
	out << "#csv格式";
	out << "#";//-------------------
	out << "\n";
	if("" == pmmemostr)
		out << "频率" << ",\t" << "种数"<< ",\t" <<"数量" << ",\t" <<"比率" << ",\t" << "累加比率" << "\n";
	else
		out << pmmemostr << endl;
	unsigned long PreTotalCnt = 0;
	 
	out.setf(ios::fixed | ios::left );
	out.precision(6);
	for(set<unsigned long>::reverse_iterator rsite = LenthTypSet.rbegin(); rsite != LenthTypSet.rend(); rsite++)
	{
		m_Multimap_Pair = pmMultimap.equal_range(*rsite);
		PreTotalCnt += (*rsite)*pmMultimap.count(*rsite);
		out << *rsite << "," << pmMultimap.count(*rsite) << "," << (*rsite)*pmMultimap.count(*rsite) << "," << 1.0*(*rsite)*(pmMultimap.count(*rsite))/TotalClauseFreqCount << "," << 1.0*PreTotalCnt/TotalClauseFreqCount << "\n";
	}
	out.close();
	delete lpSystemTime;
}


void SMOP::Save_Sorted_dat_multimap_string_long(multimap<unsigned long, string>& pmMultimap, string pmSavePath, string pmmemostr)
{
	if(pmMultimap.empty())
		return;
	ostringstream ostrsteam;
	set<unsigned long> LenthTypSet;
	for(multimap<unsigned long, string>::iterator mmite = pmMultimap.begin();  mmite != pmMultimap.end(); mmite++)
		LenthTypSet.insert((*mmite).first);

	set<unsigned long>::reverse_iterator rsite = LenthTypSet.rbegin();
	pair<multimap<unsigned long, string>::iterator, multimap<unsigned long, string>::iterator> m_Multimap_Pair;
	unsigned int filenamesequence = 0;
	unsigned long saveFPcnt = 0;
	bool BreakFileFlag = false;
	//CSecretaryApp *app = (CSecretaryApp *)AfxGetApp();
	char charresponse[1024];
	char intchar[64];	

	while(true)
	{
		string sOutFileName;
		ostrsteam.str("");
		ostrsteam << filenamesequence++ << ".dat";
		sOutFileName = ostrsteam.str();
		sOutFileName = pmSavePath +sOutFileName;
		ofstream out(sOutFileName.c_str());
		if(out.bad())
			continue;
		out.clear();
		out.seekp(0, ios::beg);
		//--------------------------save result file;
		unsigned int sentecnecnt = 0;
		while(true)
		{
			if(!BreakFileFlag)
			{
				m_Multimap_Pair = pmMultimap.equal_range(*rsite);
				out << *rsite << "\t" << pmMultimap.count(*rsite) << "\n";
			}
			int newline = 0;
			BreakFileFlag = false;
			while(m_Multimap_Pair.first != m_Multimap_Pair.second)
			{
				out << (*m_Multimap_Pair.first).second << "\t";
				m_Multimap_Pair.first++;
				saveFPcnt++;
				if(!(newline++ < 10))
				{
					out << "\n";
					newline = 0;
					if(!(saveFPcnt < EVERY_FILE_NUMBER))
					{
						break;
					}
				}
			}
			if(!(saveFPcnt < EVERY_FILE_NUMBER))
			{
				BreakFileFlag = true;
				saveFPcnt = 0;
				break;
			}
			out << "\n";
			out << "//--------------------";
			out << "\n";
			sentecnecnt++;
			rsite++;

			if(!(sentecnecnt < 1000) || (rsite == LenthTypSet.rend()))
			{
				saveFPcnt = 0;
				break;
			}
		}
		out.close();

		_itoa_s(filenamesequence,intchar,64,10);
		strcpy_s(charresponse, "保存结果:");
		strcat_s(charresponse, intchar);
		strcat_s(charresponse, ".dat");
		//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);

		if(rsite == LenthTypSet.rend())
		{
			break;
		}
	}

}

string NLPOP::Get_Substr_by_Chinese_Character_Cnt(const char* charstr, int position, int Cnt)
{
	char sentencechar[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	int intercnt;
	size_t strlength = strlen(charstr);
	if(Cnt < 0){
		Cnt = strlength;
	}
	intercnt = 0;
	strcpy_s(sentencechar, MAX_SENTENCE, "");
	for(unsigned int cntNum = 0; cntNum < strlength; ){
		sChar[0] = charstr[cntNum++];
		sChar[1] = 0;	
		if(sChar[0] < 0 ){
			sChar[1]=charstr[cntNum++];
		}
		intercnt++;
		if(intercnt > (position + Cnt)){
			break;
		}
		if(position < intercnt){
			strcat_s(sentencechar, MAX_SENTENCE, sChar);
		}
	}
	return sentencechar;
}
void NLPOP::charseq_to_vector(const char* pcharseq, vector<string>& pvector)
{
	char sChar[3];
	sChar[2]=0;
	size_t seqlength = strlen(pcharseq);

	for(size_t i = 0; i < seqlength; ){
		sChar[0] = pcharseq[i++];
		sChar[1] = 0;
		if(sChar[0] < 0 ){
			sChar[1] = pcharseq[i++];
		}
		pvector.push_back(sChar);
	}
}
void NLPOP::ChineseToken(const char* SentBuf, deque<string>& dTokenCS)
{
	char sChar[3];
	unsigned int strLen = strlen(SentBuf);
	unsigned int posit = 0;
	sChar[2]=0;
	while(posit < strLen)
	{
		sChar[0]=SentBuf[posit++];
		sChar[1]=0;	
		if(sChar[0] < 0 )
		{
			sChar[1]=SentBuf[posit++];
		}
		dTokenCS.push_back(sChar);
	}
}
size_t NLPOP::Get_Chinese_Sentence_Length_Counter(const char* contentchar){

	size_t cnt;
	size_t length = strlen(contentchar);
	cnt = 0;
	for(size_t i = 0; i < length; ){	
		if(contentchar[i++] < 0 ){
			i++;
		}
		cnt++;
	}
	return cnt;
}

void NLPOP::merging_numbers(string& stringref, set<string>& Number_Set)
{
	stringref = NLPOP::merging_numbers(stringref.c_str(), Number_Set);
}

string NLPOP::merging_numbers(const char* pcharseq, set<string>& Number_Set)
{
	char sChar[3];
	sChar[2]=0;
	size_t seqlength = strlen(pcharseq);
	string rtnstr = "";
	bool Pre_Number_Flag = false;

	for(size_t i = 0; i < seqlength; ){
		sChar[0] = pcharseq[i++];
		sChar[1] = 0;
		if(sChar[0] < 0 ){
			sChar[1] = pcharseq[i++];
		}
		if(Number_Set.find(sChar) != Number_Set.end()){
			if(Pre_Number_Flag){
				//
			}
			else{
				rtnstr += "1";
				Pre_Number_Flag = true;
			}
		}
		else if(!strcmp(sChar, ".") || !strcmp(sChar, ":") || !strcmp(sChar, "/")){
			if(Pre_Number_Flag){
				//
			}
			else{
				rtnstr += sChar;
			}
		}
		else{
			rtnstr += sChar;
			Pre_Number_Flag = false;
		}
	}
	return rtnstr;
}
void NLPOP::merging_English(string& stringref, set<string>& English_Set)
{
	stringref = NLPOP::merging_English(stringref.c_str(), English_Set);
}
string NLPOP::merging_English(const char* pcharseq, set<string>& English_Set)
{
	char sChar[3];
	sChar[2]=0;
	size_t seqlength = strlen(pcharseq);
	string rtnstr = "";
	bool Pre_English_Flag = false;

	for(size_t i = 0; i < seqlength; ){
		sChar[0] = pcharseq[i++];
		sChar[1] = 0;
		if(sChar[0] < 0 ){
			sChar[1] = pcharseq[i++];
		}
		if(English_Set.find(sChar) != English_Set.end()){
			if(Pre_English_Flag){
				//
			}
			else{
				rtnstr += "E";
				Pre_English_Flag = true;
			}
		}
		else if(!strcmp(sChar, ".") || !strcmp(sChar, " ")){
			if(Pre_English_Flag){
				//
			}
			else{
				rtnstr += sChar;
			}
		}
		else{
			rtnstr += sChar;
			Pre_English_Flag = false;
		}
	}
	return rtnstr;

}

bool NLPOP::Find_Substr_In_Chinese_Sent(const char* TargetBuf, const char* CandidateBuf)
{
	unsigned int Tlength = strlen(TargetBuf);
	unsigned int Clength = strlen(CandidateBuf);
	if(0 == Clength)
		return true;
	if(Tlength < Clength)
		return false;
	unsigned int Dlength = Tlength - Clength;
	unsigned int Tposit = 0;
	unsigned int Cposit = 0;
	while(Tposit <= Dlength)
	{
		for(Cposit =0;  Cposit< Clength; )
		{
			if(TargetBuf[Tposit + Cposit] != CandidateBuf[Cposit])
			{
				break;
			}
			if(CandidateBuf[Cposit++] < 0)
			{
				if(TargetBuf[Tposit + Cposit] != CandidateBuf[Cposit++])
				{
					break;
				}
			}
			if(Cposit == Clength)
			{
				return true;
			}
		}
		if(TargetBuf[Tposit++] < 0)
		{
			Tposit++;
		}
	}

	return false;
}


void  NLPOP::Save_Set_String_With_Comma_Divide(string pmSaveName, set<string>& pmSaveSet)
{
	//CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	ostringstream ostrsteam;
	string sOutStr;

	if(!NLPOP::Exist_of_This_File(pmSaveName)){
		ostrsteam << "提示：新建文件：" << pmSaveName << endl;
		ostrsteam << "In Save_Set_String_With_Comma_Divide()";
		sOutStr = ostrsteam.str();
		//AppCall::Secretary_Message_Box(sOutStr, MB_OK);
	}
	
	ofstream out(pmSaveName.c_str());
	if(out.bad()){
		ostrsteam.str("");
		ostrsteam << "保存文件出错：" << pmSaveName << endl;
		ostrsteam << "函数：NLPOP::Save_Set_String_With_Comma_Divide(string pmSaveName, set<string>& pmSaveSet)";
		sOutStr = ostrsteam.str();
		//AppCall::Secretary_Message_Box(sOutStr, MB_OK);
		return;
	}
	out.clear();
	out.seekp(0, ios::beg);
	
	for(set<string>::iterator site = pmSaveSet.begin(); site != pmSaveSet.end(); site++)
	{
		out << *site << ',';
	}

	out.close();



}
void  NLPOP::Output_Processing_Message(bool Total_Flag, size_t TotalCnt, size_t CurCnt)
{
	ostringstream ostrsteam;
	ostrsteam.precision(2);
	if(Total_Flag){
		ostrsteam << endl;
		ostrsteam << "===================Processing Message=====================" << endl;
		ostrsteam << "Total Processing Cases: " << TotalCnt << endl;
	}
	if(TotalCnt == 0){
		return;
	}
	ostrsteam << "Processed: " << 1.0*CurCnt/TotalCnt << "; " << "Remained: " << TotalCnt-CurCnt << endl;
	
}

void NLPOP::LoadWordsStringSet(string pathstr, set<string>& strset)
{
	ifstream in(pathstr.c_str());
	if(in.bad()){
		in.close();
		ostringstream ostrsteam;
		string sOutStr;
		ostrsteam.str("");
		ostrsteam << "打开文件出错：" << pathstr << endl;
		ostrsteam << "函数：NLPOP::LoadWordsStringSet(string pathstr, set<string>& strset)";
		sOutStr = ostrsteam.str();
		//AppCall::Secretary_Message_Box(sOutStr, MB_OK);
		return ;
	}
	char getlineBuf[MAX_SENTENCE];
	in.clear();
	in.seekg(0, ios::beg);
	if(in.peek() == EOF){
		in.close();
		return;
	}
	while(true){
		if(in.peek() == EOF){
			break;
		}
		in.getline(getlineBuf, MAX_SENTENCE,',');
		if(strlen(getlineBuf) == 0)
			continue;
		strset.insert(getlineBuf);
	} 
	in.clear();
	in.close();
}


void NLPOP::LoadWordData_AS_Map_string_MorphstrSet(string pmWDpath, map<string, set<string>>& pmWordMor_ms)
{
	ifstream in(pmWDpath.c_str());
	if(in.bad())
		return;
	in.clear();
	in.seekg(0, ios::beg);

	char getlineBuf[MAX_SENTENCE];
	string wordstr;
	string morphstr;

	if(in.peek() == EOF)
	{
		in.close();
		return;
	}
	while(!in.eof())
	{
		in.getline(getlineBuf, MAX_SENTENCE,'\t');
		in.getline(getlineBuf, MAX_SENTENCE,'\t');
		wordstr = getlineBuf;
		in.getline(getlineBuf, MAX_SENTENCE,' ');
		in.getline(getlineBuf, MAX_SENTENCE,' ');
		in.getline(getlineBuf, MAX_SENTENCE,'\n');
		morphstr = getlineBuf;

		if(wordstr == "")
			continue;
		if(pmWordMor_ms.find(wordstr) == pmWordMor_ms.end())
		{
			pmWordMor_ms[wordstr];
			pmWordMor_ms[wordstr].insert(morphstr);
		}
		else
		{
			pmWordMor_ms[wordstr].insert(morphstr);
		}
	} //while end;
	in.close();
	return;
}


void NLPOP::Get_Specified_Files(string pmFolderPath, vector<string>& FilesRoot_v, string DotAddSuffix = ".txt")
{
	string proxstr;
	CString csFilePath = NLPOP::string2CString(pmFolderPath);
	CFileFind foldpath;
	BOOL bFind = foldpath.FindFile(csFilePath + _T("*.*")); 
	if(!bFind)
	{
		return;
	}
	while(bFind = foldpath.FindNextFile())  
    {  
		csFilePath = foldpath.GetFilePath();
		if(foldpath.IsDots())   
			continue;
		if(foldpath.IsDirectory())
		{
			csFilePath += _T("\\");
			Get_Specified_Files(NLPOP::CString2string(csFilePath) , FilesRoot_v, DotAddSuffix);
		}
		

		proxstr = NLPOP::CString2string(csFilePath);
		proxstr = proxstr.substr(proxstr.rfind('\\') + 1, proxstr.length());
		if(proxstr.size() == 0)
			continue;
		if(proxstr.rfind('.') == -1)
		{
			FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
			continue;
		}
		if(0 == DotAddSuffix.length()){
			FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
			continue;
		}
		if(csFilePath.Right(csFilePath.GetLength() - csFilePath.ReverseFind('.')) != DotAddSuffix.c_str())
			continue;

		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
    }
	csFilePath = foldpath.GetFilePath();
	if(foldpath.IsDots())   
			return;
	if(foldpath.IsDirectory())
	{
		csFilePath += _T("\\");
		Get_Specified_Files(NLPOP::CString2string(csFilePath) , FilesRoot_v, DotAddSuffix);
	}
	proxstr = NLPOP::CString2string(csFilePath);
	proxstr = proxstr.substr(proxstr.rfind('\\') + 1, proxstr.length());
	if(proxstr.size() == 0)
		return;
	if(proxstr.rfind('.') == -1)
	{
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
		return;
	}
	proxstr = proxstr.substr(proxstr.rfind('.'), proxstr.length());
	
	if(0 == DotAddSuffix.length()){
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
		return;
	}

	if((strcmp(proxstr.c_str(), DotAddSuffix.c_str()) == 0 ) && (proxstr.size() != 0))
		FilesRoot_v.push_back(NLPOP::CString2string(csFilePath));
}
string NLPOP::CString2string(CString str)
{

//  CStringW   str(L"test");   
  CStringA   stra(str.GetBuffer(0));   
  str.ReleaseBuffer();       
  std::string   strs   (stra.GetBuffer(0));   
  stra.ReleaseBuffer();
  return strs;
}

CString NLPOP::string2CString(string str)
{
	CString cstr(str.c_str());
	return cstr;//Format("%s", str.c_str());
}

bool NLPOP::Exist_of_This_File(string filename)
{
	CString FPFile = NLPOP::string2CString(filename);
	CFileFind finder;
	BOOL bFind = finder.FindFile(FPFile);
	finder.Close();
	if(bFind)
		return true;
	return false;
}



BOOL NLPOP::FolderExist(CString strPath)
{
	if((strPath.GetLength() >= 2) && (strPath.ReverseFind('\\') == (strPath.GetLength() - 1)))
	{
		strPath = strPath.Left(strPath.GetLength() - 1);
	}
     WIN32_FIND_DATA   wfd;
     BOOL rValue = FALSE;
     HANDLE hFind = FindFirstFile(strPath, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
              rValue = TRUE;   
     }
     FindClose(hFind);
    return rValue;
}
BOOL NLPOP::FileExist(CString strFileName)
{
      CFileFind fFind;
     return fFind.FindFile(strFileName); 
}

BOOL NLPOP::CreateFolder(CString strPath)
{
     SECURITY_ATTRIBUTES attrib;
     attrib.bInheritHandle = FALSE;
     attrib.lpSecurityDescriptor = NULL;
     attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
    //上面定义的属性可以省略。 直接return ::CreateDirectory( path, NULL); 即可
     return ::CreateDirectory( strPath, &attrib);
} 

void NLPOP::DeleteDir(CString strDelDir)
{
    CFileFind ff;
    CString strDir, strFile;
    strDir = strDelDir;

	if(strDir.Right(1) != "\\")
		strDir += "\\";
    strDir += "*.*";
    BOOL bFind = ff.FindFile(strDir);
    while ( bFind )
    {
        bFind = ff.FindNextFile();
        if ( ff.IsDots() )
            continue;
        CString strFileName = ff.GetFileName();

        strFile = strDelDir;
        if ( strFile.Right(1) != "\\" )
			strFile += "\\";
		strFile += strFileName;
        if ( ff.IsDirectory() )
            DeleteDir(strFile);
        else
        {
            if ( ff.IsReadOnly() )
                SetFileAttributes(strFile, GetFileAttributes(strFile) & (~FILE_ATTRIBUTE_READONLY) );
            DeleteFile(strFile);
        }
    }
    ff.Close();
    RemoveDirectory(strDelDir);
 
} 
void NLPOP::Delete_Files_In_Dir(CString strDelDir)
{

    CFileFind ff;
    CString strDir, strFile;
    strDir = strDelDir;

	if(strDir.Right(1) != "\\")
		strDir += "\\";
    strDir += "*.*";
    BOOL bFind = ff.FindFile(strDir);
    while ( bFind )
    {
        bFind = ff.FindNextFile();
        if ( ff.IsDots() )
            continue;
        CString strFileName = ff.GetFileName();

        strFile = strDelDir;
        if ( strFile.Right(1) != "\\" )
			strFile += "\\";
		strFile += strFileName;
        if ( ff.IsDirectory() )
            DeleteDir(strFile);
        else
        {
            if ( ff.IsReadOnly() )
                SetFileAttributes(strFile, GetFileAttributes(strFile) & (~FILE_ATTRIBUTE_READONLY) );
            DeleteFile(strFile);
        }
    }
    ff.Close();

}

