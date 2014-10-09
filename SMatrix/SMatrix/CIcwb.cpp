#include "StdAfx.h"
#include "..\\SMatrix.h"
#include "CIcwb.h"
#include "Clexical.h"

CIcwb::CIcwb(Clexical& m_Clexical): m_Clexical(m_Clexical)
{

}
CIcwb::~CIcwb()
{
}

string CIcwb::Executer_Port(string pmService, vector<string> sParam_v, vector<LPVOID> pParam_v)
{
	//char charresponse[1024];
	while(!m_Clexical.Init_Lexicon_Flag){
		Sleep(500);
	}
	if("Extract_Unequal_Segment_Result_In_Two_Files_And_Save" == pmService)
	{
		Extract_Unequal_Segment_Result_In_Two_Files_And_Save();
	}
	if("Computer_The_Correct_Rate_According_to_Icwb_In_Two_Files" == pmService)
	{
		Computer_The_Correct_Rate_According_to_Icwb_In_Two_Files();
	}
	if("Extract_Icwb_Training_Ambiguity_Segment_Standard" == pmService)
	{
		Extract_Icwb_Training_Ambiguity_Segment_Standard();
	}
	if("Extract_CIcwb_Training_Words_To_Set" == pmService)
	{
		Extract_CIcwb_Training_Words_To_Set();
	}
	if("Extract_CIcwb_Training_To_Set" == pmService)
	{
		set<string> saveset;
		Extract_CIcwb_Training_To_Set("", saveset);
	}

	return "";
}
//============================================================================
//--
void CIcwb::Extract_CIcwb_Training_Words_To_Set()
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	string CicwbWordPath;

	//-----------------------------------
	AfxMessageBox(_T("请指定Words文件路径..."), MB_OK);
	
	CFileDialog IcwbOpenDialog(TRUE, NULL, _T("*.txt"));
	int nModal = IcwbOpenDialog.DoModal();
	if (nModal == IDOK)
	{
		CicwbWordPath = NLPOP::CString2string(IcwbOpenDialog.GetPathName());
	}
	//--------------------------------------

	deque<string> CICWWords_d;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(CicwbWordPath, CICWWords_d, 0, '\n', false);

	set<string> ExtractedWords_s;
	for(deque<string>::iterator dite = CICWWords_d.begin(); dite != CICWWords_d.end(); dite++)
	{
		ExtractedWords_s.insert(*dite);
	}
	string sSavePath;
	sSavePath = CicwbWordPath;
	sSavePath = sSavePath.substr(0, CicwbWordPath.rfind('\\')+1);
	sSavePath += "CicwbWords.dat";

	NLPOP::Save_Set_String_With_Comma_Divide(sSavePath, ExtractedWords_s);

}
void CIcwb::Extract_CIcwb_Training_To_Set(string pmPath, set<string>& pmWordsSet)
{
	char SentenceBuf[MAX_SENTENCE];

	if(!NLPOP::Exist_of_This_File(pmPath))
	{
		//-----------------------------------
		AfxMessageBox(_T("请指定Words文件路径..."), MB_OK);
		
		CFileDialog IcwbOpenDialog(TRUE, NULL, _T("*.txt"));
		int nModal = IcwbOpenDialog.DoModal();
		if (nModal == IDOK)
		{
			pmPath = NLPOP::CString2string(IcwbOpenDialog.GetPathName());
		}
		//--------------------------------------
	}
	deque<string> TotalLine_d;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(pmPath, TotalLine_d, 0, '\n', false);

	for(deque<string>::iterator dite = TotalLine_d.begin(); dite != TotalLine_d.end(); dite++)
	{
		istringstream isstream(*dite);
		while(true)
		{
			while(isstream.peek() == ' ')
			{
				isstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(isstream.peek() == EOF)
			{	
				break;
			}
			isstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				pmWordsSet.insert(SentenceBuf);
		}
	}
	TotalLine_d.clear();

	string sSavePath;
	sSavePath = pmPath;
	sSavePath = sSavePath.substr(0, pmPath.rfind('\\')+1);
	sSavePath += "CicwbWords.dat";

	NLPOP::Save_Set_String_With_Comma_Divide(sSavePath, pmWordsSet);
}



void CIcwb::Extract_Icwb_Training_Ambiguity_Segment_Standard()
{
	string CicwbGold;
	string ResultPath;
	//-----------------------------------
	AfxMessageBox(_T("请指定“标准”测试结果文件路径..."), MB_OK);
	
	CFileDialog IcwbOpenDialog(TRUE, NULL, _T("*.txt"));
	int nModal = IcwbOpenDialog.DoModal();
	if (nModal == IDOK)
	{
		CicwbGold = NLPOP::CString2string(IcwbOpenDialog.GetPathName());
	}
	//--------------------------------------
	AfxMessageBox(_T("请指定“对比”测试结果文件路径..."), MB_OK);

	CFileDialog OpenDialog(TRUE, NULL, _T("*.txt"));
	nModal = OpenDialog.DoModal();
	if (nModal == IDOK)
	{
		ResultPath = NLPOP::CString2string(OpenDialog.GetPathName());
	}

	ostringstream ostrsteam;
	string sOutStr;
	char SentenceBuf[MAX_SENTENCE];
	
	if(!NLPOP::Exist_of_This_File(CicwbGold))
	{
		AfxMessageBox(_T("指定文件不存在..."), MB_OK);
		return;
	}
	if(!NLPOP::Exist_of_This_File(ResultPath))
	{
		AfxMessageBox(_T("指定文件不存在..."), MB_OK);
		return;
	}

	deque<string> CICWGold_d;
	deque<string> Static_d;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(CicwbGold, CICWGold_d, 0, '\n', false);
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(ResultPath, Static_d, 0, '\n', false);

	if(CICWGold_d.size() != Static_d.size())
	{
		ostrsteam.str("");
		ostrsteam << "行数不一致，请修正..." << endl;
		ostrsteam << "标准集行数：" << CICWGold_d.size() << endl;
		ostrsteam << "比较集行数：" << Static_d.size() << endl;
		sOutStr = ostrsteam.str();
		AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
		return;
	}
	vector<string> IcwbLine_v;
	vector<string> StaticLine_v;
	//--error detector--
	deque<CLAUSEPOS> ErrorSubSentIcwb_d;
	deque<CLAUSEPOS> ErrorSubSentStat_d;
	//bool ErrorFoundFlag;

	for(unsigned long lineseq = 0; lineseq < Static_d.size(); lineseq++)
	{
		istringstream Icwbisstream(CICWGold_d[lineseq]);
		while(true)
		{
			while(Icwbisstream.peek() == ' ')
			{
				Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Icwbisstream.peek() == EOF)
			{	
				break;
			}
			Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				IcwbLine_v.push_back(SentenceBuf);
		}

		istringstream Staticisstream(Static_d[lineseq]);

		//---Error
		//ErrorFoundFlag = false;
		CLAUSEPOS eSentIcwb;
		CLAUSEPOS eSentStat;
		while(true)
		{
			while(Staticisstream.peek() == ' ')
			{
				Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Staticisstream.peek() == EOF)
			{	
				break;
			}
			Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				StaticLine_v.push_back(SentenceBuf);
		}
		unsigned int Icwbint = 0;
		unsigned int Staticint = 0;
		unsigned int IcwbLength = 0;
		unsigned int StaticLength = 0;
		while((Icwbint < IcwbLine_v.size()) && (Staticint < StaticLine_v.size()))
		{
			if(IcwbLength == StaticLength)
			{
				if(IcwbLine_v[Icwbint] == StaticLine_v[Staticint])
				{
					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
					if(eSentIcwb.Clause.size() > 0)
					{
						ErrorSubSentIcwb_d.push_back(eSentIcwb);
						ErrorSubSentStat_d.push_back(eSentStat);
						eSentIcwb.Clause.clear();
						eSentStat.Clause.clear();
					}
				}
				else
				{
					//ErrorFoundFlag = true;
					eSentIcwb.Clause.push_back(IcwbLine_v[Icwbint]);
					eSentStat.Clause.push_back(StaticLine_v[Staticint]);

					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
				}
				Icwbint++;
				Staticint++;
			}
			if(IcwbLength < StaticLength)
			{
				if(Icwbint == IcwbLine_v.size())
				{
					ostrsteam.str("");
					ostrsteam << "分词出错，导致句子长度不一致..." << endl;
					ostrsteam << "原句为：";
					for(vector<string>::iterator vite = IcwbLine_v.begin(); vite != IcwbLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					ostrsteam << "错句为：";
					for(vector<string>::iterator vite = StaticLine_v.begin(); vite != StaticLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					sOutStr = ostrsteam.str();
					AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
					break;
				}
				eSentIcwb.Clause.push_back(IcwbLine_v[Icwbint]);
				IcwbLength += IcwbLine_v[Icwbint++].length();
			}
			if(IcwbLength > StaticLength)
			{
				if(Staticint == StaticLine_v.size())
				{
					ostrsteam.str("");
					ostrsteam << "分词出错，导致句子长度不一致..." << endl;
					ostrsteam << "原句为：";
					for(vector<string>::iterator vite = IcwbLine_v.begin(); vite != IcwbLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					ostrsteam << "错句为：";
					for(vector<string>::iterator vite = StaticLine_v.begin(); vite != StaticLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					sOutStr = ostrsteam.str();
					AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
					break;
				}
				eSentStat.Clause.push_back(StaticLine_v[Staticint]);
				StaticLength += StaticLine_v[Staticint++].length();
			}

		}
		IcwbLine_v.clear();
		StaticLine_v.clear();
	}
	CICWGold_d.clear();
	Static_d.clear();

	set<string> CSWords_Set;
	NLPOP::LoadWordsStringSet("F:\\YPench\\Secretary\\Data\\CSWordsSet.dat", CSWords_Set );

	multimap<string, string> disAmbiguity_mm;
	string phrasestr;
	string segmentstr;
	bool samesegmentflag;
	for(deque<CLAUSEPOS>::iterator dite = ErrorSubSentIcwb_d.begin(); dite != ErrorSubSentIcwb_d.end(); dite++)
	{
		phrasestr = "";
		segmentstr = "";
		for(vector<string>::iterator vite = (*dite).Clause.begin(); vite != (*dite).Clause.end(); vite++)
		{
			if(CSWords_Set.find(*vite) == CSWords_Set.end())
			{
				phrasestr = "";
				break;
			}
			phrasestr += *vite;
			segmentstr += *vite;
			segmentstr += ' ';
		}

		if(0 == strlen(phrasestr.c_str()))
			continue;

		if(disAmbiguity_mm.find(phrasestr) == disAmbiguity_mm.end())
		{
			disAmbiguity_mm.insert(make_pair(phrasestr, segmentstr));
		}
		else
		{
			samesegmentflag = false;
			pair<multimap<string, string>::iterator,multimap<string, string>::iterator> m_Multimap_Pair = disAmbiguity_mm.equal_range(phrasestr);

			while(m_Multimap_Pair.first != m_Multimap_Pair.second)
			{
				if(0 == strcmp(m_Multimap_Pair.first->second.c_str(),  segmentstr.c_str()))
				{
					samesegmentflag =  true;
					break;
				}
				m_Multimap_Pair.first++;
			}
			if(!samesegmentflag)
			{
				disAmbiguity_mm.insert(make_pair(phrasestr, segmentstr));
			}
		}
	}
	string oufilename = ResultPath.substr(0, ResultPath.rfind('\\')+1);
	oufilename += "disAmbiguity.txt";
	ofstream out(oufilename.c_str());
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);

	for(multimap<string, string>::iterator mmite = disAmbiguity_mm.begin(); mmite != disAmbiguity_mm.end(); mmite++)
	{
		out << (*mmite).first << endl;
		out << (*mmite).second << endl;
	}
	out.close();

}

void CIcwb::Check_Overlopping_and_Combination_Ambiguity_Error_Rate(const char* ResultFolder, deque<CLAUSEPOS> ErrorSubSentIcwb_d,	deque<CLAUSEPOS> ErrorSubSentStat_d)
{
	if(ErrorSubSentIcwb_d.size() != ErrorSubSentIcwb_d.size()){
		AfxMessageBox(_T("Unequal Data in: CIcwb::Check_Overlopping_and_Combination_Ambiguity_Error_Rate().."), MB_OK);
	}
	set<string>& Lexicon_s = m_Clexical.IcwbWords_set;
	size_t OverlapCnt = 0;
	size_t CombinatCnt = 0;
	size_t OOVCnt = 0;
	deque<CLAUSEPOS>::iterator Icwbite = ErrorSubSentIcwb_d.begin();
	deque<CLAUSEPOS>::iterator Sentite = ErrorSubSentStat_d.begin();
	deque<CLAUSEPOS> sOverlap_d;
	deque<CLAUSEPOS> sCombnat_d;
	deque<CLAUSEPOS> sOOV_d;
	for( ; Icwbite != ErrorSubSentIcwb_d.end(); Icwbite++, Sentite++){
		vector<string>& Icwb_v = Icwbite->Clause;
		vector<string>& Sent_v = Sentite->Clause;
		vector<string>::iterator Ivite = Icwb_v.begin();
		vector<string>::iterator Svite = Sent_v.begin();
		size_t Icwbposit = 0;
		size_t Sentposit = 0;
		for( ; Ivite != Icwb_v.end(); Ivite++){
			Icwbposit += Ivite->length();
		}
		for( ; Svite != Sent_v.end(); Svite++){
			Sentposit += Svite->length();
		}
		if(Icwbposit != Sentposit){
			AfxMessageBox(_T("Unequal Data in: CIcwb::Check_Overlopping_and_Combination_Ambiguity_Error_Rate().."), MB_OK);
		}
		Icwbposit= 0;
		Sentposit = 0;
		Ivite = Icwb_v.begin();
		Svite = Sent_v.begin();
		while((Ivite != Icwb_v.end()) || (Svite != Sent_v.end())){ //Computer_The_Correct_Rate_According_to_Icwb_In_Two_Files
			vector<string> Icwbstr_v;
			vector<string> Sentstr_v;
			if(Ivite->length() < Svite->length()){
				Sentstr_v.push_back(Svite->data());
				Sentposit += Svite->length();
				for( ; Ivite != Icwb_v.end(); Ivite++){
					Icwbstr_v.push_back(Ivite->data());
					Icwbposit += Ivite->length();
					if(Icwbposit >= Sentposit){
						break;
					}
				}
				if(Icwbposit == Sentposit){
					CombinatCnt++;
					CLAUSEPOS tem_v;
					tem_v.Clause = Icwbstr_v;
					tem_v.POS = Sentstr_v;
					sCombnat_d.push_back(tem_v);
				}
				else if(Icwbposit > Sentposit){
					while(Icwbposit != Sentposit){
						if((Icwbposit > Sentposit) && (Svite != Sent_v.end())){
							Svite++;
							if(Svite != Sent_v.end()){
								Sentstr_v.push_back(Svite->data());
								Sentposit += Svite->length();
							}
						}
						else if((Icwbposit < Sentposit) && (Ivite != Icwb_v.end())){
							Ivite++;
							if(Ivite != Icwb_v.end()){
								Icwbstr_v.push_back(Ivite->data());
								Icwbposit += Ivite->length();
							}
						}
					}
					OverlapCnt++;
					CLAUSEPOS tem_v;
					tem_v.Clause = Icwbstr_v;
					tem_v.POS = Sentstr_v;
					sOverlap_d.push_back(tem_v);
				}
				else{
					AfxMessageBox(_T("Warring"), MB_OK);
				}
			}
			else if(Ivite->length() > Svite->length()){
				Icwbstr_v.push_back(Ivite->data());
				Icwbposit += Ivite->length();

				for( ; Svite != Sent_v.end(); Svite++){
					Sentstr_v.push_back(Svite->data());
					Sentposit += Svite->length();
					if(Sentposit >= Icwbposit){
						break;
					}
				}
				if(Sentposit == Icwbposit){
					OOVCnt++;
					CLAUSEPOS tem_v;
					tem_v.Clause = Icwbstr_v;
					tem_v.POS = Sentstr_v;
					sOOV_d.push_back(tem_v);
				}
				else if(Sentposit > Icwbposit){
					while(Icwbposit != Sentposit){
						if((Icwbposit > Sentposit) && (Svite != Sent_v.end())){
							Svite++;
							if(Svite != Sent_v.end()){
								Sentstr_v.push_back(Svite->data());
								Sentposit += Svite->length();
							}
						}
						else if((Icwbposit < Sentposit) && (Ivite != Icwb_v.end())){
							Ivite++;
							if(Ivite != Icwb_v.end()){
								Icwbstr_v.push_back(Ivite->data());
								Icwbposit += Ivite->length();
							}
						}
					}
					OverlapCnt++;
					CLAUSEPOS tem_v;
					tem_v.Clause = Icwbstr_v;
					tem_v.POS = Sentstr_v;
					sOverlap_d.push_back(tem_v);
				}
			}
			else{
				AfxMessageBox(_T("Unequal Data in: CIcwb::Check_Overlopping_and_Combination_Ambiguity_Error_Rate().."), MB_OK);
			}
			Ivite++;
			Svite++;
		}//while
	}//for

	string sfilename = ResultFolder;
	sfilename += "\\OverlappingError.txt";
	Save_Error_Segment_Result_In_CLAUSEPOS_Deque(sfilename, sOverlap_d);
	sfilename = ResultFolder;
	sfilename += "\\CombinationError.txt";
	Save_Error_Segment_Result_In_CLAUSEPOS_Deque(sfilename, sCombnat_d);
	sfilename = ResultFolder;
	sfilename += "\\OOVError.txt";
	Save_Error_Segment_Result_In_CLAUSEPOS_Deque(sfilename, sOOV_d);

	ostringstream ostrsteam;
	size_t TotalCnt = OverlapCnt + CombinatCnt + OOVCnt;
	double OverlapRat = 1.0*OverlapCnt/TotalCnt;
	double CombinatRat = 1.0*CombinatCnt/TotalCnt;
	double OOVRat = 1.0*OOVCnt/TotalCnt;
	ostrsteam << "Total Number: " << TotalCnt << endl;
	ostrsteam << "Overlapping Counter: " << OverlapCnt << ";\t" << OverlapRat << endl;
	ostrsteam << "Combination Counter: " << CombinatCnt << ";\t" << CombinatRat << endl;
	ostrsteam << "OOV Counter:         " << OOVCnt << ";\t" << OOVRat << endl;
	AfxMessageBox(NLPOP::string2CString(ostrsteam.str()), MB_OK);
}

//======================================================================
//----Computer_The_Correct_Rate_According_to_Icwb_In_Two_Files()
void CIcwb::Computer_The_Correct_Rate_According_to_Icwb_In_Two_Files()
{
	string CicwbGold;
	string ResultPath;
	//-----------------------------------
	AfxMessageBox(_T("请指定“标准”测试结果文件路径..."), MB_OK);
	
	CFileDialog IcwbOpenDialog(TRUE, NULL, _T("*.txt"));
	int nModal = IcwbOpenDialog.DoModal();
	if (nModal == IDOK)
	{
		CicwbGold = NLPOP::CString2string(IcwbOpenDialog.GetPathName());
	}
	//--------------------------------------
	AfxMessageBox(_T("请指定“对比”测试结果文件路径..."), MB_OK);

	CFileDialog OpenDialog(TRUE, NULL, _T("*.txt"));
	nModal = OpenDialog.DoModal();
	if (nModal == IDOK)
	{
		ResultPath = NLPOP::CString2string(OpenDialog.GetPathName());
	}

	Statistic_Segment_Result_With_CIcwb_In_File(ResultPath, CicwbGold);
}
void CIcwb::Statistic_Segment_Result_With_CIcwb_In_File(string pmRetPath, string pmCicwbGold)
{

//	char charresponse[1024];
	ostringstream ostrsteam;
	string sOutStr;
	char SentenceBuf[MAX_SENTENCE];
	
	if(!NLPOP::Exist_of_This_File(pmRetPath))
	{
		AfxMessageBox(_T("指定文件不存在..."), MB_OK);
		return;
	}
	if(!NLPOP::Exist_of_This_File(pmCicwbGold))
	{
		AfxMessageBox(_T("指定文件不存在..."), MB_OK);
		return;
	}
	deque<string> CICWGold_d;
	deque<string> Static_d;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(pmCicwbGold, CICWGold_d, 0, '\n', false);
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(pmRetPath, Static_d, 0, '\n', false);

	if(CICWGold_d.size() != Static_d.size())
	{
		ostrsteam.str("");
		ostrsteam << "行数不一致，请修正..." << endl;
		ostrsteam << "标准集行数：" << CICWGold_d.size() << endl;
		ostrsteam << "比较集行数：" << Static_d.size() << endl;
		sOutStr = ostrsteam.str();
		AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
		return;
	}
	unsigned long CorrectCnt = 0;
	unsigned long RefTotalCnt = 0;
	unsigned long SegTotalCnt = 0;
	vector<string> IcwbLine_v;
	vector<string> StaticLine_v;
	//--error detector--
	deque<CLAUSEPOS> ErrorSubSentIcwb_d;
	deque<CLAUSEPOS> ErrorSubSentStat_d;
	//bool ErrorFoundFlag;

	for(unsigned long lineseq = 0; lineseq < Static_d.size(); lineseq++)
	{
		istringstream Icwbisstream(CICWGold_d[lineseq]);
		while(true)
		{
			while(Icwbisstream.peek() == ' ')
			{
				Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Icwbisstream.peek() == EOF)
			{	
				break;
			}
			Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				IcwbLine_v.push_back(SentenceBuf);
		}
		RefTotalCnt += IcwbLine_v.size();
		

		istringstream Staticisstream(Static_d[lineseq]);

		//---Error
		//ErrorFoundFlag = false;
		CLAUSEPOS eSentIcwb;
		CLAUSEPOS eSentStat;
		while(true)
		{
			while(Staticisstream.peek() == ' ')
			{
				Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Staticisstream.peek() == EOF)
			{	
				break;
			}
			Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				StaticLine_v.push_back(SentenceBuf);
		}
		SegTotalCnt += StaticLine_v.size();

		unsigned int Icwbint = 0;
		unsigned int Staticint = 0;
		unsigned int IcwbLength = 0;
		unsigned int StaticLength = 0;
		while((Icwbint < IcwbLine_v.size()) && (Staticint < StaticLine_v.size()))
		{
			if(IcwbLength == StaticLength)
			{
				if(IcwbLine_v[Icwbint] == StaticLine_v[Staticint])
				{
					CorrectCnt++;
					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
					if(eSentIcwb.Clause.size() > 0)
					{
						ErrorSubSentIcwb_d.push_back(eSentIcwb);
						ErrorSubSentStat_d.push_back(eSentStat);
						eSentIcwb.Clause.clear();
						eSentStat.Clause.clear();
					}
				}
				else
				{
					//ErrorFoundFlag = true;
					eSentIcwb.Clause.push_back(IcwbLine_v[Icwbint]);
					eSentStat.Clause.push_back(StaticLine_v[Staticint]);

					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
				}
				Icwbint++;
				Staticint++;
			}
			if(IcwbLength < StaticLength)
			{
				if(Icwbint == IcwbLine_v.size())
				{
					ostrsteam.str("");
					ostrsteam << "分词出错，导致句子长度不一致..." << endl;
					ostrsteam << "原句为：";
					for(vector<string>::iterator vite = IcwbLine_v.begin(); vite != IcwbLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					ostrsteam << "错句为：";
					for(vector<string>::iterator vite = StaticLine_v.begin(); vite != StaticLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					sOutStr = ostrsteam.str();
					AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
					break;
				}
				eSentIcwb.Clause.push_back(IcwbLine_v[Icwbint]);
				IcwbLength += IcwbLine_v[Icwbint++].length();
			}
			if(IcwbLength > StaticLength)
			{
				if(Staticint == StaticLine_v.size())
				{
					ostrsteam.str("");
					ostrsteam << "分词出错，导致句子长度不一致..." << endl;
					ostrsteam << "原句为：";
					for(vector<string>::iterator vite = IcwbLine_v.begin(); vite != IcwbLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					ostrsteam << "错句为：";
					for(vector<string>::iterator vite = StaticLine_v.begin(); vite != StaticLine_v.end(); vite++)
					{
						ostrsteam << *vite;
					}
					ostrsteam << endl;
					sOutStr = ostrsteam.str();
					AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
					break;
				}
				eSentStat.Clause.push_back(StaticLine_v[Staticint]);
				StaticLength += StaticLine_v[Staticint++].length();
			}

		}
		IcwbLine_v.clear();
		StaticLine_v.clear();
	}
	CICWGold_d.clear();
	Static_d.clear();

	//Subtract_Mark_Start_Ambiguity(RefTotalCnt, SegTotalCnt, ErrorSubSentIcwb_d,ErrorSubSentStat_d);
	
	Check_Overlopping_and_Combination_Ambiguity_Error_Rate(pmRetPath.substr(0, pmRetPath.rfind('\\')).c_str(), ErrorSubSentIcwb_d, ErrorSubSentStat_d);

	string sSegInfo;
	ostrsteam.str("");
	ostrsteam << "CorrectCnt：\t" << CorrectCnt << endl;
	ostrsteam << "SegTotalCnt：\t" << SegTotalCnt << endl;
	ostrsteam << "RefTotalCnt：\t" << RefTotalCnt << endl;
	ostrsteam << "Precision：\t" << 1.0*CorrectCnt/SegTotalCnt << endl;
	ostrsteam << "Recall：\t" << 1.0*CorrectCnt/RefTotalCnt << endl;
	ostrsteam << "F1: \t" <<  2*(1.0*CorrectCnt/SegTotalCnt)*(1.0*CorrectCnt/RefTotalCnt)/((1.0*CorrectCnt/SegTotalCnt) + (1.0*CorrectCnt/RefTotalCnt)) << endl;
	sSegInfo = ostrsteam.str();
	ostrsteam << endl;
	ostrsteam << "是否需要保存统计结果？" << endl;
	sOutStr = ostrsteam.str();
	if(IDYES == AfxMessageBox(NLPOP::string2CString(sOutStr), MB_YESNOCANCEL))
	{
		Save_Statistic_Segment_Result_Compare_With_CIcwb(pmRetPath, ErrorSubSentIcwb_d, ErrorSubSentStat_d, sSegInfo);
	}


	return;
}
void CIcwb::Subtract_Mark_Start_Ambiguity(unsigned long& RefTotalCnt, unsigned long& SegTotalCnt, deque<CLAUSEPOS>& ErrorSubSentIcwb_d, deque<CLAUSEPOS>& ErrorSubSentStat_d)
{
	string strstart;
	unsigned int CicwbPosit;
	unsigned int SegPosit;
	bool Start_Detected_Flag;
	bool Word_Detected_Flag;

	deque<CLAUSEPOS>::iterator Cicdite = ErrorSubSentIcwb_d.begin();

	for(deque<CLAUSEPOS>::iterator segdite = ErrorSubSentStat_d.begin(); segdite != ErrorSubSentStat_d.end(); segdite++, Cicdite++)
	{
		CicwbPosit = 0;
		SegPosit = 0;
		Start_Detected_Flag = false;
		Word_Detected_Flag = false;
		vector<string>::iterator cicvite = (*Cicdite).Clause.begin();
		vector<string>::iterator segvite = (*segdite).Clause.begin();
		for(; segvite != (*segdite).Clause.end(); segvite++)
		{
			strstart = "";
			for(unsigned int i = 0; i < (*segvite).length(); i++)
			{
				strstart += "*";	
			}
			if(0 == strcmp(strstart.c_str(), (*segvite).c_str()))
			{
				SegPosit += (*segvite).length();
				*segvite = "";
				SegTotalCnt--;
				Start_Detected_Flag = true;
			}
			else
			{
				while(CicwbPosit < SegPosit)
				{
					CicwbPosit += (*cicvite).length();
					if(Start_Detected_Flag)
					{
						*cicvite = "";
						RefTotalCnt--;
					}
					cicvite++;
				}
				Start_Detected_Flag = false;
				Word_Detected_Flag = true;
			}
		}
		while(CicwbPosit < SegPosit){
			CicwbPosit += (*cicvite).length();
			if(Start_Detected_Flag)
			{
				*cicvite = "";
				RefTotalCnt--;
			}
			cicvite++;
		}
		if(!Word_Detected_Flag){
			(*Cicdite).Clause.clear();
			(*segdite).Clause.clear();
		}
	}
}


void CIcwb::Save_Statistic_Segment_Result_Compare_With_CIcwb(string pmSaveFileName, deque<CLAUSEPOS>& ErrorSubSentIcwb_d, deque<CLAUSEPOS>& ErrorSubSentStat_d, string SegInfo)
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	LPSYSTEMTIME lpSystemTime = new  SYSTEMTIME;
	ostringstream ostrsteam;
	string datastr;
	GetLocalTime(lpSystemTime);
	ostrsteam << lpSystemTime->wYear << '_' << lpSystemTime->wMonth << '_' << lpSystemTime->wDay << ' ' << lpSystemTime->wHour << ':' << lpSystemTime->wMinute << ':' << lpSystemTime->wSecond;
	datastr = "日期：";
	datastr += ostrsteam.str();
	delete lpSystemTime;
	
	ofstream out;

	string sfilename = pmSaveFileName.substr(0, pmSaveFileName.rfind('\\'));
	sfilename += "\\";
	sfilename += "Info_Error.txt";

	if(!NLPOP::Exist_of_This_File(sfilename)){
		out.open(sfilename.c_str());
		if(out.bad())
			return;
		out.clear();
		out.seekp(0, ios::beg);
		out.close();
	}
	out.open(sfilename.c_str(), ios::app);
	if(out.bad())
		return;
	out << "-----------------------------------------" << endl;
	out << datastr << endl;
	out << SegInfo << endl;
	out.close();

	sfilename = sfilename.substr(0, sfilename.rfind('\\'));
	sfilename += "\\";
	sfilename += "Couple_Error.txt";

	deque<CLAUSEPOS> CombinedRtn_d;
	if(ErrorSubSentIcwb_d.size() != ErrorSubSentStat_d.size()){
		AfxMessageBox(_T("Error: in CIcwb::Save_Statistic_Segment_Result_Compare_With_CIcwb..."), MB_YESNOCANCEL);
	}
	for(size_t i = 0; i < ErrorSubSentIcwb_d.size(); i++){
		CLAUSEPOS temp;
		temp.Clause = ErrorSubSentIcwb_d[i].Clause;
		temp.POS = ErrorSubSentStat_d[i].Clause;
		CombinedRtn_d.push_back(temp);
	}
	Save_Error_Segment_Result_In_CLAUSEPOS_Deque(sfilename, CombinedRtn_d);

	if(IDYES != AfxMessageBox(_T("是否需要用结果修改分词词典？"), MB_YESNOCANCEL)){
		return ;
	}
	if(IDYES != AfxMessageBox(_T("请再次确认修改分词词典？..."), MB_YESNOCANCEL)){
		return ;
	}

	//Modify_WordsSet_Use_Icwb_Set(ErrorSubSentIcwb_d, ErrorSubSentStat_d);

	ErrorSubSentIcwb_d.clear();
	ErrorSubSentStat_d.clear();
}
void CIcwb::Save_Error_Segment_Result_In_CLAUSEPOS_Deque(string pmSaveFileName, deque<CLAUSEPOS>& ErrSegRet_d)
{
	ofstream out(pmSaveFileName.c_str());
	if(out.bad()){
		return;
	}
	out.clear();
	out.seekp(0, ios::beg);
	unsigned long Seq = 0;
	for(unsigned long i = 0; i < ErrSegRet_d.size(); i++){
		out << Seq++ << "." << "(C: ";
		for(unsigned long j = 0; j < ErrSegRet_d[i].Clause.size(); j++){
			out << ErrSegRet_d[i].Clause[j] << " ";
		}
		out << " / E: ";
		for(unsigned long j = 0; j < ErrSegRet_d[i].POS.size(); j++){
			out << ErrSegRet_d[i].POS[j] << " ";
		}
		out << ") ";
	}
	out << endl;
	out.close();
}



void CIcwb::Modify_WordsSet_Use_Icwb_Set(deque<CLAUSEPOS>& ErrorSubSentIcwb_d, deque<CLAUSEPOS>& ErrorSubSentStat_d)
{
	ostringstream ostrsteam;
	string sOutStr;
	char charresponse[1024];

	if(ErrorSubSentIcwb_d.size() != ErrorSubSentStat_d.size())
	{
		ostrsteam.str("");
		ostrsteam << "行数不一致，请修正..." << endl;
		ostrsteam << "ErrorSubSentIcwb_d：" << ErrorSubSentIcwb_d.size() << endl;
		ostrsteam << "ErrorSubSentStat_d：" << ErrorSubSentStat_d.size() << endl;
		sOutStr = ostrsteam.str();
		AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
		return;
	}

	strcpy_s(charresponse, "加载相关词典...");
//	::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);

	set<string> IcwbWordsSet;
	set<string> Punctuation_Set;
	set<string> English_Set;
	set<string> Number_Set;
	NLPOP::LoadWordsStringSet("F:\\YPench\\Secretary\\Data\\WordIcwb_set.dat", IcwbWordsSet);
	NLPOP::LoadWordsStringSet("F:\\YPench\\Secretary\\Data\\Punctuation_Set.dat", Punctuation_Set );
	NLPOP::LoadWordsStringSet("F:\\YPench\\Secretary\\Data\\English_Set.dat",		English_Set );
	NLPOP::LoadWordsStringSet("F:\\YPench\\Secretary\\Data\\nonChinaNumeralSet.dat",		Number_Set );

	unsigned int SubSentStatPosit;
	unsigned int SubSentIcwbPosit;
	string ChildWordsStr;
	vector<string> ChildWords_v;

	if(IDYES == AfxMessageBox(_T("是否要删除分词词典中“长”的词？"), MB_YESNOCANCEL))
	{
		strcpy_s(charresponse, "删除分词词典中“长”的词...");
//		::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);
		//-----delete words thar in IcwbWordsSet but not the testwords---------------------------------------------------------------
		for(unsigned long i = 0; i < ErrorSubSentStat_d.size(); i++)
		{
			SubSentStatPosit = 0;
			for(vector<string>::iterator statavite = ErrorSubSentStat_d[i].Clause.begin(); statavite != ErrorSubSentStat_d[i].Clause.end(); statavite++)
			{
				ChildWordsStr = "";
				ChildWords_v.clear();
				SubSentIcwbPosit = 0;

				for(vector<string>::iterator icwbvite = ErrorSubSentIcwb_d[i].Clause.begin(); icwbvite != ErrorSubSentIcwb_d[i].Clause.end(); icwbvite++)
				{
					if((*icwbvite).length() > (*statavite).length())
						break;
					SubSentIcwbPosit += (*icwbvite).length();

					if(SubSentIcwbPosit > SubSentStatPosit)
					{
						ChildWordsStr += *icwbvite;
						ChildWords_v.push_back(*icwbvite);
						if((*statavite).length() != ChildWordsStr.length())
							continue;
						if(0 == strcmp((*statavite).c_str(), ChildWordsStr.c_str()))
						{
							if(ChildWords_v.size() > 1)
							{
								if(IcwbWordsSet.find(ChildWordsStr) != IcwbWordsSet.end())
								{
									IcwbWordsSet.erase(ChildWordsStr);

									for(vector<string>::iterator evite = ChildWords_v.begin(); evite != ChildWords_v.end(); evite++)
									{
										IcwbWordsSet.insert(*evite);
									}
								}
							}
						}
					}
				}
				SubSentStatPosit += (*statavite).length();
			}
		}
	}
	if(IDYES == AfxMessageBox(_T("是否要添加分词词典中缺少、训练集中有的词？"), MB_YESNOCANCEL))
	{
		strcpy_s(charresponse, "添加分词词典中缺少、训练集中有的词...");
		//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);
		//----Add Word not have in -IcwbWordsSet----------------------------------------------------------------------------
		for(unsigned long i = 0; i < ErrorSubSentIcwb_d.size(); i++)
		{
			SubSentIcwbPosit = 0;
			for(vector<string>::iterator icwbvite = ErrorSubSentIcwb_d[i].Clause.begin(); icwbvite != ErrorSubSentIcwb_d[i].Clause.end(); icwbvite++)
			{
				ChildWordsStr = "";
				ChildWords_v.clear();
				SubSentStatPosit = 0;

				for(vector<string>::iterator statavite = ErrorSubSentStat_d[i].Clause.begin(); statavite != ErrorSubSentStat_d[i].Clause.end(); statavite++)
				{
					if((*statavite).length() > (*icwbvite).length())
						break;
					SubSentStatPosit += (*statavite).length();

					if(SubSentStatPosit > SubSentIcwbPosit)
					{
						ChildWordsStr += *statavite;
						ChildWords_v.push_back(*statavite);
						if((*icwbvite).length() != ChildWordsStr.length())
							continue;
						if(0 == strcmp((*icwbvite).c_str(), ChildWordsStr.c_str()))
						{
							if(ChildWords_v.size() > 1)
							{
								if(IcwbWordsSet.find(ChildWordsStr) == IcwbWordsSet.end())
								{
									unsigned long SentLength = ChildWordsStr.length();
									char sChar[3];
									sChar[2]=0;
									bool FoundnonCharacherFlag = false;
									for(unsigned long posit = 0; posit < SentLength; )
									{
										sChar[0] = ChildWordsStr.c_str()[posit++];
										sChar[1] = 0;	
										if(sChar[0] < 0 )
										{
											sChar[1] =  ChildWordsStr.c_str()[posit++];
										}
										if(Punctuation_Set.find(sChar) != Punctuation_Set.end())
										{
											FoundnonCharacherFlag = true;
											break;
										}
										if(Number_Set.find(sChar) != Number_Set.end())
										{
											FoundnonCharacherFlag = true;
											break;
										}
										if(English_Set.find(sChar) != English_Set.end())
										{
											FoundnonCharacherFlag = true;
											break;
										}
									}
									if(!FoundnonCharacherFlag)
									{
										IcwbWordsSet.insert(ChildWordsStr);
									}
								}
							}
						}
					}
				}
				SubSentIcwbPosit += (*icwbvite).length();
			}
		}
	}
	if(IDYES == AfxMessageBox(_T("注意：是否要改词典？"), MB_YESNOCANCEL))
	{
		strcpy_s(charresponse, "保存结果...");
		//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);

		NLPOP::Save_Set_String_With_Comma_Divide("", IcwbWordsSet);
		IcwbWordsSet.clear();
	}

	strcpy_s(charresponse, "命令结束...");
	//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);

	
}

//========================================================================================
//----Extract_Unequal_Segment_Result_In_Two_Files_And_Save--------------------------------
void CIcwb::Extract_Unequal_Segment_Result_In_Two_Files_And_Save()
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	string CicwbGold;
	string ResultPath;
	string SavePath;
	ostringstream ostrsteam;
	string sOutStr;
	//-----------------------------------
	AfxMessageBox(_T("请指定“标准”测试结果文件路径..."), MB_OK);
	
	CFileDialog IcwbOpenDialog(TRUE, NULL, _T("*.txt"));
	int nModal = IcwbOpenDialog.DoModal();
	if (nModal == IDOK)
	{
		CicwbGold = NLPOP::CString2string(IcwbOpenDialog.GetPathName());
	}
	//--------------------------------------
	AfxMessageBox(_T("请指定“对比”测试结果文件路径..."), MB_OK);

	CFileDialog OpenDialog(TRUE, NULL, _T("*.txt"));
	nModal = OpenDialog.DoModal();
	if (nModal == IDOK)
	{
		ResultPath = NLPOP::CString2string(OpenDialog.GetPathName());
	}

	deque<string> Static_d;
	deque<string> CICWGold_d;

	deque<string> Error_d;
	deque<string> Correct_d;

	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(CicwbGold, CICWGold_d, 0, '\n', false);
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(ResultPath, Static_d, 0, '\n', false);

	Extract_The_Error_Result_In_Deque(Static_d, CICWGold_d, Error_d, Correct_d);

	if(Error_d.size() !=  Correct_d.size())
	{
		ostrsteam.str("");
		ostrsteam << "行数不一致，请修正..." << endl;
		ostrsteam << "标准集行数：" << CICWGold_d.size() << endl;
		ostrsteam << "比较集行数：" << Static_d.size() << endl;
		sOutStr = ostrsteam.str();
		AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
		return;
	}
	SavePath = ResultPath.substr(0, ResultPath.rfind('\\'));
	SavePath += "\\";
	SavePath += "Correct_Error.txt";
	ofstream out(SavePath.c_str());
	if(out.bad())
		return;
	out.clear();
	out.seekp(0, ios::beg);

	for(unsigned long i = 0; i < Error_d.size(); i++)
	{
		out << "Correct:" ;
		out << Correct_d[i] << endl;
		out << "Error  :" ;
		out << Error_d[i] << endl;
		//out << "------->" << endl;
	}
	out.close();

	CICWGold_d.clear();
	Static_d.clear();
	Error_d.clear();
	Correct_d.clear();
}

void CIcwb::Extract_The_Error_Result_In_Deque(deque<string>& Static_d, deque<string>& CICWGold_d, deque<string>& Error_d, deque<string>& Correct_d)
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	ostringstream ostrsteam;
	string sOutStr;
	char SentenceBuf[MAX_SENTENCE];
	
	if(CICWGold_d.size() != Static_d.size())
	{
		ostrsteam.str("");
		ostrsteam << "行数不一致，请修正..." << endl;
		ostrsteam << "标准集行数：" << CICWGold_d.size() << endl;
		ostrsteam << "比较集行数：" << Static_d.size() << endl;
		sOutStr = ostrsteam.str();
		AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
		return;
	}

	vector<string> IcwbLine_v;
	vector<string> StaticLine_v;
	for(unsigned long lineseq = 0; lineseq < Static_d.size(); lineseq++)
	{
		istringstream Icwbisstream(CICWGold_d[lineseq]);
		while(true)
		{
			while(Icwbisstream.peek() == ' ')
			{
				Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Icwbisstream.peek() == EOF)
			{	
				break;
			}
			Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				IcwbLine_v.push_back(SentenceBuf);
		}

		istringstream Staticisstream(Static_d[lineseq]);
		while(true)
		{
			while(Staticisstream.peek() == ' ')
			{
				Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Staticisstream.peek() == EOF)
			{	
				break;
			}
			Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				StaticLine_v.push_back(SentenceBuf);
		}
		unsigned int Icwbint = 0;
		unsigned int Staticint = 0;
		unsigned int IcwbLength = 0;
		unsigned int StaticLength = 0;
		bool ErrordetectedFlag = false;
		while((Icwbint < IcwbLine_v.size()) && (Staticint < StaticLine_v.size()))
		{
			if(IcwbLength == StaticLength)
			{
				if(IcwbLine_v[Icwbint] == StaticLine_v[Staticint])
				{
					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
				}
				else
				{
					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
					ErrordetectedFlag = true;
					break;
				}
				Icwbint++;
				Staticint++;
			}
			if(IcwbLength < StaticLength)
			{
				IcwbLength += IcwbLine_v[Icwbint++].length();
			}
			if(IcwbLength > StaticLength)
			{
				StaticLength += StaticLine_v[Staticint++].length();
			}
		}
		IcwbLine_v.clear();
		StaticLine_v.clear();
		if(ErrordetectedFlag)
		{
			Error_d.push_back(Static_d[lineseq]);
			Correct_d.push_back(CICWGold_d[lineseq]);
		}
	}

}

//============================================================================
//-----sticking to Icwb training Corpus
void CIcwb::Extract_CIcwb_Training_TXT_to_Blank_TXT(string pmPath)
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	char SentenceBuf[MAX_SENTENCE];

	if(!NLPOP::Exist_of_This_File(pmPath))
	{
		AfxMessageBox(_T("指定文件不存在..."), MB_OK);
		return;
	}
	deque<string> TotalLine_d;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(pmPath, TotalLine_d, 0, '\n', false);

	deque<string> ResultDeque;
	string DequeLine;
	for(deque<string>::iterator dite = TotalLine_d.begin(); dite != TotalLine_d.end(); dite++)
	{
		DequeLine = "";
		istringstream isstream(*dite);
		while(true)
		{
			while(isstream.peek() == ' ')
			{
				isstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(isstream.peek() == EOF)
			{	
				break;
			}
			isstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				DequeLine += SentenceBuf;
		}
		ResultDeque.push_back(DequeLine);
	}
	pmPath = pmPath.substr(0, pmPath.rfind('\\'));
	pmPath += "\\";
	pmPath += "Blank_Training.txt";
	Write_Vector_Deque_List_To_File<deque<string>>(pmPath, ResultDeque, 0, '\n', "");

	TotalLine_d.clear();
	ResultDeque.clear();
}




//================================================================================
//------------------------------------------------------------------------------------
/*
void CIcwb::Compare_The_Result_Whith_Icwb_In_Deque(string pmPath, string pmIcwbPath)
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	char charresponse[1024];

	if(!NLPOP::Exist_of_This_File(pmPath))
	{
		AfxMessageBox(_T("指定文件不存在..."), MB_OK);
		return;
	}
	strcpy_s(charresponse, "开始加载文件...");
	::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);
	
	deque<string> BePasedSent_d;
	deque<unsigned long> ReserveSentSeq_d;
	unsigned long ReserveSentSeq = 0;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(pmPath, BePasedSent_d, 0, '\n',false);
	

	deque<CLAUSEPOS> SecrePasedPos_d;
	deque<CLAUSEPOS> ICTCLASPasedPos_d;

	//--for every line in the file
	for(deque<string>::iterator dite = BePasedSent_d.begin(); dite != BePasedSent_d.end(); dite++)
	{
		bool Ambiguity_Detected_Flag = false;
		CLAUSEPOS SentPos;
		Sentence_Segmenting_Port((*dite).c_str(), SentPos, Ambiguity_Detected_Flag);
		
		if(!Ambiguity_Detected_Flag)
		{
			SecrePasedPos_d.push_back(SentPos);
			SentPos.Clause.clear();
			SentPos.POS.clear();

			m_CSP_ICTCLAS.ICTCLAS_Pase((*dite).c_str(), SentPos);
			ICTCLASPasedPos_d.push_back(SentPos);
			SentPos.Clause.clear();
			SentPos.POS.clear();

			ReserveSentSeq_d.push_back(ReserveSentSeq);
		}
		ReserveSentSeq++;
	}
	BePasedSent_d.clear();
//----------------------------------------------------------------------
	deque<string> IcwbBePasedSent_d;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(pmIcwbPath, IcwbBePasedSent_d, 0, '\n',false);

	deque<string> Icwb_d;
	deque<string> ICTCAS_d;
	deque<string> Secretary_d;
	for(deque<unsigned long>::iterator dite = ReserveSentSeq_d.begin(); dite != ReserveSentSeq_d.end(); dite++)
	{
		Icwb_d.push_back(IcwbBePasedSent_d[*dite]);
	}
	for(unsigned long linecnt= 0;  linecnt < SecrePasedPos_d.size();  linecnt++)
	{
		string tempstr = "";
		for(vector<string>::iterator vite = SecrePasedPos_d[linecnt].Clause.begin(); vite != SecrePasedPos_d[linecnt].Clause.end(); vite++)
		{
			tempstr += *vite;
			tempstr += "  ";
		}
		Secretary_d.push_back(tempstr);
		tempstr = "";
		for(vector<string>::iterator vite = ICTCLASPasedPos_d[linecnt].Clause.begin(); vite != ICTCLASPasedPos_d[linecnt].Clause.end(); vite++)
		{
			tempstr += *vite;
			tempstr += "  ";
		}
		ICTCAS_d.push_back(tempstr);
	}
	SecrePasedPos_d.clear();
	ICTCLASPasedPos_d.clear();

	Statistic_Segment_Result_With_CIcwb_In_Deque(ICTCAS_d, Icwb_d);
	Statistic_Segment_Result_With_CIcwb_In_Deque(Secretary_d, Icwb_d);

}
*/

void CIcwb::Statistic_Segment_Result_With_CIcwb_In_Deque(deque<string>& Static_d, deque<string>& CICWGold_d)
{

//	char charresponse[1024];
	ostringstream ostrsteam;
	string sOutStr;
	char SentenceBuf[MAX_SENTENCE];
	
	if(CICWGold_d.size() != Static_d.size())
	{
		ostrsteam.str("");
		ostrsteam << "行数不一致，请修正..." << endl;
		ostrsteam << "标准集行数：" << CICWGold_d.size() << endl;
		ostrsteam << "比较集行数：" << Static_d.size() << endl;
		sOutStr = ostrsteam.str();
		AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
		return;
	}
	unsigned long CorrectCnt = 0;
	unsigned long RefTotalCnt = 0;
	vector<string> IcwbLine_v;
	vector<string> StaticLine_v;
	for(unsigned long lineseq = 0; lineseq < Static_d.size(); lineseq++)
	{
		istringstream Icwbisstream(CICWGold_d[lineseq]);
		while(true)
		{
			while(Icwbisstream.peek() == ' ')
			{
				Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Icwbisstream.peek() == EOF)
			{	
				break;
			}
			Icwbisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				IcwbLine_v.push_back(SentenceBuf);
		}
		RefTotalCnt += IcwbLine_v.size();

		istringstream Staticisstream(Static_d[lineseq]);
		while(true)
		{
			while(Staticisstream.peek() == ' ')
			{
				Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			}
			if(Staticisstream.peek() == EOF)
			{	
				break;
			}
			Staticisstream.getline(SentenceBuf, MAX_SENTENCE, ' ');
			if(strlen(SentenceBuf) != 0)
				StaticLine_v.push_back(SentenceBuf);
		}
		unsigned int Icwbint = 0;
		unsigned int Staticint = 0;
		unsigned int IcwbLength = 0;
		unsigned int StaticLength = 0;
		while((Icwbint < IcwbLine_v.size()) && (Staticint < StaticLine_v.size()))
		{
			if(IcwbLength == StaticLength)
			{
				if(IcwbLine_v[Icwbint] == StaticLine_v[Staticint])
				{
					CorrectCnt++;
					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
					
				}
				else
				{
					IcwbLength += IcwbLine_v[Icwbint].length();
					StaticLength += StaticLine_v[Staticint].length();
				}
				Icwbint++;
				Staticint++;
			}
			if(IcwbLength < StaticLength)
			{
				IcwbLength += IcwbLine_v[Icwbint++].length();
			}
			if(IcwbLength > StaticLength)
			{
				StaticLength += StaticLine_v[Staticint++].length();
			}

		
		}
		IcwbLine_v.clear();
		StaticLine_v.clear();
	}
	ostrsteam.str("");
	ostrsteam << "数据集计数：" << RefTotalCnt << endl;
	ostrsteam << "正确计数：" << CorrectCnt << endl;
	ostrsteam << "错误计数：" << RefTotalCnt-CorrectCnt << endl;
	ostrsteam << "正确率：" << 1.0*CorrectCnt/RefTotalCnt << endl;
	sOutStr = ostrsteam.str();
	AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);

}

