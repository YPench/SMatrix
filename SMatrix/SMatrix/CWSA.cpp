#include "StdAfx.h"
#include "CWSA.h"
#include "..\\SMatrix.h"
#include "ConvertUTF.h"

#define CRF_FOLDER "..\\CRF\\"

bool BiMM_Detecting_Flag;
bool SM_MOAS_Detecting_Flag;
bool SM_OAS_Detecting_Flag;
bool SM_Compromise_CAS_Detecting_Flag;
bool SM_CAS_Detecting_Flag;
bool Omni_or_MNAG_Detecting_Flag;
size_t Segmenting_n_Gram;
int Debug_Sentence_Number;
bool Segmenting_CRF_Training_Flag;
bool Segmenting_CRF_Testing_Flag;


CWSA::CWSA() : m_CIcwb(m_Clexical),m_CSegment(m_Clexical)
{
	BiMM_Detecting_Flag = false;
	SM_MOAS_Detecting_Flag = false;
	SM_OAS_Detecting_Flag = false;
	SM_Compromise_CAS_Detecting_Flag = false;
	SM_CAS_Detecting_Flag = false;
	Omni_or_MNAG_Detecting_Flag = true;
	Segmenting_n_Gram = 3;
	Debug_Sentence_Number = 100;
	Segmenting_CRF_Training_Flag = false;
	Segmenting_CRF_Testing_Flag = true;

	if(!NLPOP::FolderExist(_T(LEXICON_DATA_FOLDER))){
		_mkdir(LEXICON_DATA_FOLDER);
	}
	//m_out.open("F:\\YPench\\SMatrix\\pku_test\\test.txt");
	
	/*string pmService = "Sentence_Segmenting_Training_and_Testing" ;
	vector<string> sParam_v;
	vector<LPVOID> pParam_v;
	Executer_Port(pmService, sParam_v, pParam_v);*/

	//Generating_Penn_Chinese_Treebank_Lexicon("F:\\Data\\TreebankAmbiguity\\segmentedSentence.txt", "F:\\Data\\TreebankAmbiguity\\TreeBankWords.dat");
	m_Clexical.Init();
}
CWSA::~CWSA()
{
	//m_out.close();
}

string CWSA::Segmentation_Matrix_Port(const char* charinput)
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	char charresponse[1024];
	strcpy_s(charresponse, 1024, "Running...");
	::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse, 1);


	string Service = charinput;
	vector<string> sParam_v;
	vector<LPVOID> pParam_v;

	if(Executer_Port(Service, sParam_v, pParam_v).length() != 0){
		return "Done...";
	}
	//if(m_CIcwb.Executer_Port(Service, sParam_v, pParam_v).length() != 0){
	//	return "Done...";
	//}
	return "";
}

string CWSA::Executer_Port(string pmService, vector<string> sParam_v, vector<LPVOID> pParam_v)
{
	//if("Sentence_Segmenting_Training_and_Testing" == pmService){
	if(true){
		string pmPath = ".\\..\\pku_test\\";
		Sentence_Segmenting_Training_and_Testing(pmPath);
	}

	if("Segment_The_Pointed_Floder_Save_Statistical_Info" == pmService){
		string pmPath = "F:\\TreebankAmbiguity\\RowSentence\\";
		Segment_The_Pointed_Floder_Save_Statistical_Info(pmPath);
	}
	if("CWSA::Ambiguity_Space_of_Given_Lexicon" == pmService){
		//set<string> testset;
		//testset.insert("新政");
		//testset.insert("政要");
		//testset.insert("要害");
		//testset.insert("中国人");
		//:YPench
		//Ambiguity_Space_of_Given_Lexicon(m_Cerebra.SnotationSet);
	}
	if("CWSA::Penn_Chinese_Treebank_Experiment" == pmService){
		Penn_Chinese_Treebank_Experiment("F:\\Data\\TreebankAmbiguity\\RowSentence\\RowSentence.txt", "F:\\Data\\TreebankAmbiguity\\SegFolder\\");
	}
	return "";
}


//===========================================================================================
//---Segment_Sent_File
void CWSA::Sentence_Segmenting_Training_and_Testing(string pmPath = "")
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	char charresponse[1024];
	string ResultPath;
	if("" == pmPath){
		//AfxMessageBox(_T("请指定文件路径..."), MB_OK);
		CFileDialog IcwbOpenDialog(TRUE, NULL, _T("*.txt"));
		int nModal = IcwbOpenDialog.DoModal();
		if (nModal == IDOK){
			ResultPath = NLPOP::CString2string(IcwbOpenDialog.GetPathName());
		}
	}
	else{
		ResultPath = pmPath;
	}
	vector<vector<int>*> SegmentingPath_vv;
	vector<vector<string>*> SegmentingPathStr_vv;
	vector<string> BePasedSent_v;
	if(Segmenting_CRF_Training_Flag){
		pmPath += "Training\\";
	}
	else if(Segmenting_CRF_Testing_Flag){
		pmPath += "Testing\\";
	}
	Read_Line_InFile_To_Vector_Deque_List<vector<string>>(pmPath+"pku_gold.txt", BePasedSent_v, 0, '\n',false);
	SMOP::Generating_Segmentation_Path(BePasedSent_v, SegmentingPath_vv, SegmentingPathStr_vv);
	BePasedSent_v.clear();

	Read_Line_InFile_To_Vector_Deque_List<vector<string>>(pmPath+"pku.txt", BePasedSent_v, 0, '\n',false);
	if(BePasedSent_v.size() != SegmentingPath_vv.size()){
		AfxMessageBox(_T("数据不一致..."), MB_OK);
		return;
	}
	vector<CLAUSEPOS> PasedPos_v;
	for(size_t i = 0; i < BePasedSent_v.size(); i++){
		CLAUSEPOS SentPos;

		static size_t check_point = 0;
		check_point++;
		if(100 == check_point){
			break;
		}
		/*
		ostringstream ostrsteam;
		ostrsteam << check_point;
		m_out << check_point << "\n";
		m_out << BePasedSent_v[i].c_str() << endl;
		//TRACE(NLPOP::string2CString(ostrsteam.str()));
		//TRACE("\n");*/
		m_CSegment.m_p_SegmentingPath = SegmentingPath_vv[i];
		m_CSegment.m_p_SPathStr_v = SegmentingPathStr_vv[i];
		m_CSegment.m_p_Sent = BePasedSent_v[i].c_str();
		m_CSegment.m_SentPosit = 0;
		m_CSegment.m_i = 0;
		m_CSegment.m_SentLength =  NLPOP::Get_Chinese_Sentence_Length_Counter(BePasedSent_v[i].c_str());
		m_CSegment.Sentence_Segmenting_Port(BePasedSent_v[i].c_str(), SentPos);
		
		PasedPos_v.push_back(SentPos);
	}
	BePasedSent_v.clear();

	strcpy_s(charresponse, "分词结束，保存结果...");
	::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,1);

	ResultPath = CRF_FOLDER;
	if(Segmenting_CRF_Training_Flag){
		ResultPath += "TrainingData.txt";
	}
	else if(Segmenting_CRF_Testing_Flag){
		ResultPath += "TestingData.txt";
	}

	m_CSegment.Output_SPathStr_for_CRF(ResultPath.c_str());
	ostringstream parameterstream;

	if(Segmenting_CRF_Training_Flag){
		parameterstream << "template" << " " << ResultPath.c_str() << " " << "CRF_Model";
		CRF_Training_Port(parameterstream.str().c_str(), ResultPath.c_str());
	}
	else if(Segmenting_CRF_Testing_Flag){
		string outputtest = CRF_FOLDER;
		outputtest += "output.txt";
		parameterstream << "F:\\YPench\\SMatrix\\CRF\\crf_test.exe" << " -m " << "CRF_Model" << " " << ResultPath.c_str();
		CRF_Running_with_Output_Redirection(CRF_FOLDER, parameterstream.str().c_str(), outputtest.c_str());
	}

}


void CWSA::CRF_Running_with_Output_Redirection(const char* cImplementFolder, const char* parameters,  const char* outputFile)
{
	if(strlen(parameters) == 0){
		AfxMessageBox(_T("File or Parameters not exist in: CCRF::CRF_Training_Port()"), MB_OK);
		return;
	}

	wchar_t awParameters[256];
	wchar_t awOutputFileName[256];

	SCONVERT::AnsiToUnicode(outputFile, awOutputFileName);
	SCONVERT::AnsiToUnicode(parameters, awParameters);

	TCHAR cmdline[1024];
	_tcscpy_s(cmdline, 1024, awParameters);

	DeleteFile(awOutputFileName);

	SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};
    SECURITY_ATTRIBUTES *psa=NULL; 
    DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;  
    OSVERSIONINFO osVersion={0};  
    osVersion.dwOSVersionInfoSize =sizeof ( osVersion );  
    if(GetVersionEx(&osVersion)){  
        if(osVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT)  {  
            psa=&sa;
            dwShareMode|=FILE_SHARE_DELETE;  
        }  
    }
	HANDLE hConsoleRedirect = CreateFile(  
								awOutputFileName,  
								GENERIC_WRITE,  
								dwShareMode,  
								psa,  
								OPEN_ALWAYS,  
								FILE_ATTRIBUTE_NORMAL,  
								NULL );  
	ASSERT(hConsoleRedirect!=INVALID_HANDLE_VALUE );


    STARTUPINFO si={sizeof(si)};  
    si.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;//使用标准柄和显示窗口  
    si.hStdOutput =hConsoleRedirect;//将文件作为标准输出句柄  
    si.wShowWindow =SW_SHOW;//隐藏控制台窗口  
    PROCESS_INFORMATION pi={0};
	memset(&pi, 0, sizeof(pi));

//	if(CreateProcess(NULL, cmdline,NULL,NULL,TRUE,NULL,NULL, _T(cImplementFolder),&si,&pi))  { 
	if(CreateProcess(NULL, cmdline,NULL,NULL,TRUE,NULL,NULL,NLPOP::string2CString(cImplementFolder),&si,&pi))  {
        WaitForSingleObject(pi.hProcess, INFINITE);  
        CloseHandle(pi.hProcess);  
        CloseHandle(pi.hThread);
    }  
    CloseHandle(hConsoleRedirect);
}

void CWSA::CRF_Training_Port(const char* parameters, const char* cFilePath)
{
	wchar_t awTrainingFileName[256];
	wchar_t awParameters[256];

	if(!NLPOP::Exist_of_This_File(cFilePath) || (strlen(parameters) == 0)){
		AfxMessageBox(_T("File or Parameters not exist in: CCRF::CRF_Training_Port()"), MB_OK);
		return;
	}
	SCONVERT::AnsiToUnicode(parameters, awParameters);
	SCONVERT::AnsiToUnicode(cFilePath, awTrainingFileName);

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpDirectory = _T(CRF_FOLDER);
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShExecInfo.lpFile = _T("crf_learn.exe");

	ShExecInfo.lpParameters = awParameters;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
}

void CWSA::Penn_Chinese_Treebank_Experiment(string pmFilePath, string pmSaveFolder)
{
	vector<string> BePasedSent_v;
	vector<string> SegedSent_v;
	//vector<string> ConpareSegedSent_v;

	Read_Line_InFile_To_Vector_Deque_List<vector<string>>(pmFilePath, BePasedSent_v, 0, '\n',false);
	//Read_Line_InFile_To_Vector_Deque_List<vector<string>>("F:\\Data\\TreebankAmbiguity\\segmentedSentence.txt", SegedSent_v, 0, '\n',false);

	//if(BePasedSent_v.size() != SegedSent_v.size()){
	//	AfxMessageBox(_T("数据不一致in CWSA::Penn_Chinese_Treebank_Experiment..."), MB_OK);
	//	return;
	//}
	vector<CLAUSEPOS> PasedPos_v;
	vector<string>::iterator segevite = SegedSent_v.begin();

	//AfxMessageBox(_T("Checking opint...."), MB_OK);
	static size_t check_point = 0;
	
	for(vector<string>::iterator vite = BePasedSent_v.begin();  vite != BePasedSent_v.end(); vite++){//,segevite++){
		CLAUSEPOS SentPos;
		bool Ambiguity_Flag = false;

		//m_out << check_point << "\\" <<  BePasedSent_v.size()-check_point++ << "\t";
		//m_out << vite->data() << endl;

		//m_CSegment.Sentence_Segmenting_Port("－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－", SentPos, Ambiguity_Flag);
		m_CSegment.Sentence_Segmenting_Port((*vite).c_str(), SentPos);

		if(Ambiguity_Flag){
			continue;
		}
		//ConpareSegedSent_v.push_back(*segevite);
		//PasedPos_v.push_back(SentPos);
	}

	//Write_Vector_Deque_List_To_File<vector<string>>(pmSaveFolder + "CompareSeged.txt", ConpareSegedSent_v, 0, '\n', "");

	//SMOP::Save_CLAUSEPOS_deque_Without_Icwb_POS((pmSaveFolder + "Secretary_test.txt") , PasedPos_v);
	PasedPos_v.clear();

	m_CSegment.Output_Ambiguity_Info("F:\\Data\\TreebankAmbiguity\\Info\\");

}

void CWSA::Generating_Penn_Chinese_Treebank_Lexicon(const char* charpath, const char* charsave)
{
	char SentenceBuf[Double_MAX_CLAUSE_LENGTH];
	vector<string> sLine_v;
	set<string> Words_s;
	Read_Line_InFile_To_Vector_Deque_List<vector<string>>(charpath, sLine_v, 0, '\n',false);

	for(vector<string>::iterator vite = sLine_v.begin(); vite != sLine_v.end(); vite++){
		istringstream sstream(vite->data());
		while(sstream.peek() != EOF){	
			sstream.getline(SentenceBuf, Double_MAX_CLAUSE_LENGTH, ' ');
			if(strlen(SentenceBuf) != 0){
				Words_s.insert(SentenceBuf);
			}
		}
	}
	for(set<string>::iterator site = Words_s.begin(); site != Words_s.end(); ){
		if((*site).find(',') != string::npos){
			site = Words_s.erase(site);
		}
		else{
			site++;
		}
	}
	NLPOP::Save_Set_String_With_Comma_Divide(charsave, Words_s);
}



//===========================================================================================
//---Segment_The_Pointed_Floder_Save_Statistical_Info()
void CWSA::Segment_The_Pointed_Floder_Save_Statistical_Info(string pmPath = "")
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	char charresponse[1024];
	char intchar[64];
	unsigned long RemainderFile;
	string ResultPath;

	string str = "江泽民在北京人民大会堂会见参加全国法院工作会议和全国法院系统打击经济犯罪先进集体表彰大会代表时要求大家要充分认识打击经济犯罪工作的艰巨性和长期性";
	//string str = "江泽民在北京";
	CLAUSEPOS SentPos;
	m_CSegment.Sentence_Segmenting_Port(str.c_str(), SentPos);
	return;

	if("" == pmPath)
	{
		//-----------------------------------
		AfxMessageBox(_T("请指定文件路径..."), MB_OK);
		
		CFileDialog IcwbOpenDialog(TRUE, NULL, _T("*.txt"));
		int nModal = IcwbOpenDialog.DoModal();
		if (nModal == IDOK)
		{
			ResultPath = NLPOP::CString2string(IcwbOpenDialog.GetPathName());
		}
	}
	else
	{
		ResultPath = pmPath;
	}

	vector<string> FilesRoot_v;
	NLPOP::Get_Specified_Files(ResultPath, FilesRoot_v, ".txt");
	RemainderFile = FilesRoot_v.size();

	deque<string> BePasedSent_d;
	for(vector<string>::iterator vite = FilesRoot_v.begin(); vite != FilesRoot_v.end(); vite++)
	{
		_itoa_s(RemainderFile--,intchar,64,10);
		strcpy_s(charresponse, 1024, "Remainding Files: ");
		strcat_s(charresponse, 1024, intchar);
		//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);


		Read_Line_InFile_To_Vector_Deque_List<deque<string>>(*vite, BePasedSent_d, 0, '\n',false);

		for(deque<string>::iterator dite = BePasedSent_d.begin();  dite != BePasedSent_d.end(); dite++)
		{
			CLAUSEPOS SentPos;
			bool Ambiguity_Flag = false;
			m_CSegment.Sentence_Segmenting_Port((*dite).c_str(), SentPos);
		}
		BePasedSent_d.clear();
	}

/*
	ostringstream ostrsteam;
	ostrsteam.str("");
	ostrsteam << "双向扫描算法统计信息" << endl;
	ostrsteam << "句子总数：\t" << totalsent_cnt << endl;
	ostrsteam << "无歧句子数：\t" << un_amboguity_cnt << endl;
	string sOutStr = ostrsteam.str();
	(*(app->pdlg)).SecretaryHide();
	AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
	(*(app->pdlg)).SecretaryShow();
*/
}

void CWSA::Ambiguity_Space_of_Given_Lexicon(set<string>& pmLexicon)
{

//	char charresponse[1024];
//	char intchar[64];
//	unsigned long RemainderWords;
//-------adjacent words
	/*
	RemainderWords = pmLexicon.size();
	string str;
	for(set<string>::iterator site = pmLexicon.begin(); site != pmLexicon.end(); site++)
	{
		_itoa_s(RemainderWords--,intchar,64,10);
		strcpy_s(charresponse, 1024, "Remainding Words: ");
		strcat_s(charresponse, 1024, intchar);
		::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);

		for(set<string>::iterator insite = pmLexicon.begin(); insite != pmLexicon.end(); insite++)
		{
			str = *site;
			str += *insite;
			CLAUSEPOS SentPos;
			bool Ambiguity_Flag = false;
			Sentence_Segmenting_Port(str.c_str(), SentPos, Ambiguity_Flag);
		}
	}
*/
//-------overlaping words
/*
	char prixChar[3];
	char proxChar[3];

	string overlapingstr;
	prixChar[2] = 0;
	proxChar[2] = 0;
	unsigned int prixposit;
	unsigned int proxposit;
	bool equal_overlapping_flag;

	RemainderWords = pmLexicon.size();

	for(set<string>::iterator site = pmLexicon.begin(); site != pmLexicon.end(); site++)
	{
		_itoa_s(RemainderWords--,intchar,64,10);
		strcpy_s(charresponse, 1024, "Remainding Words: ");
		strcat_s(charresponse, 1024, intchar);
		::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);

		for(set<string>::iterator insite = pmLexicon.begin(); insite != pmLexicon.end(); insite++)
		{
			prixposit = 0;
			if((*site).c_str()[prixposit++] < 0)
			{
				prixposit++;
			}
			while(prixposit < (*site).size())
			{
				
				if((*insite).size() <= ((*site).size() - prixposit))
				{
					if((*site).c_str()[prixposit++] < 0)
					{
						prixposit++;
					}
					continue;
				}
				proxposit = 0;
				overlapingstr = "";
				equal_overlapping_flag = true;

				for(unsigned int i = prixposit; i < (*site).size(); )
				{
					prixChar[0] = (*site).c_str()[i++];
					if(prixChar[0] < 0)
					{
						prixChar[1] = (*site).c_str()[i++];
					}
					else
					{
						prixChar[1] = 0;
					}

					proxChar[0] = (*insite).c_str()[proxposit++];
					if(proxChar[0] < 0)
					{
						proxChar[1] = (*insite).c_str()[proxposit++];
					}
					else
					{
						proxChar[1] = 0;
					}
					if(0 == strcmp(prixChar, proxChar))
					{
						overlapingstr += prixChar;
					}
					else
					{
						equal_overlapping_flag = false;
						break;
					}
				}
				if(equal_overlapping_flag)
				{
					overlapingstr = *site;
					for(unsigned int j = proxposit; j < (*insite).size(); )
					{
						overlapingstr += (*insite).c_str()[j++];
					}

					CLAUSEPOS SentPos;
					bool Ambiguity_Flag = false;
					Sentence_Segmenting_Port(overlapingstr.c_str(), SentPos, Ambiguity_Flag);
				}
				if((*site).c_str()[prixposit++] < 0)
				{
					prixposit++;
				}
			}
		}
	}
*/

	for(set<string>::iterator site = pmLexicon.begin(); site != pmLexicon.end(); site++)
	{
		CLAUSEPOS SentPos;
		m_CSegment.Sentence_Segmenting_Port((*site).c_str(), SentPos);
	}
}


void CWSA::Segment_Sent_File_to_Deque_With_Secretary(string pmPath, deque<CLAUSEPOS>& PasedPos_d)
{
	CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
	char charresponse[1024];

	if(!NLPOP::Exist_of_This_File(pmPath)){
		AfxMessageBox(_T("指定文件不存在..."), MB_OK);
		return;
	}
	strcpy_s(charresponse, "开始加载文件...");
	//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);
	
	deque<string> BePasedSent_d;
	Read_Line_InFile_To_Vector_Deque_List<deque<string>>(pmPath, BePasedSent_d, 0, '\n',false);

	//--for every line in the file
	strcpy_s(charresponse, "开始处理文件...");
	//::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse,NORMAL_RESPONCE_MSG);
	for(deque<string>::iterator dite = BePasedSent_d.begin(); dite != BePasedSent_d.end(); dite++){
		CLAUSEPOS SentPos;
		
		m_CSegment.Sentence_Segmenting_Port((*dite).c_str(), SentPos);

		//if(!Ambiguity_Detected_Flag)
		PasedPos_d.push_back(SentPos);
	}

	BePasedSent_d.clear();
}


