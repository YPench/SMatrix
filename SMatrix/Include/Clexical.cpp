#include "StdAfx.h"
#include "Clexical.h"


DWORD WINAPI Init_Lexicon_Thread(LPVOID pParam) ;

Clexical::Clexical()
{
	Init_Lexicon_Flag = false;
	
}
Clexical::~Clexical()
{

}

void Clexical::Init()
{
	HANDLE hThread;
	DWORD ThreadId;
	hThread = CreateThread(NULL, 0, Init_Lexicon_Thread, (LPVOID)this, 0, &ThreadId);
}


DWORD WINAPI Init_Lexicon_Thread(LPVOID pParam) 
{
	Clexical* plexical = (Clexical*)pParam;
	char charlexicalpath[1024];
	strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "ChineseChar.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->ChinChar_Set);

	strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "English_Set.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->English_Set);
	
	strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "Number_Set.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->Number_Set);

	strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "Punctuation_Set.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->Punctuation_Set);

	//strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	//strcat_s(charlexicalpath, 1024, "CVMC.dat");
	//NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->CVMC_Set);

	//strcpy_s(charlexicalpath, 1024, "F:\\Data\\TreebankAmbiguity\\TreebankWords.dat");
	//NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->IcwbWords_set);

	/*strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "pkuTestWords.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->IcwbWords_set);*/

	strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "pkuTrainWords.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->IcwbWords_set);

	/*strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "CVMC.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->IcwbWords_set);

	strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "Worddb_set.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->IcwbWords_set);

	strcpy_s(charlexicalpath, 1024, LEXICON_DATA_FOLDER);
	strcat_s(charlexicalpath, 1024, "CSWordsSet.dat");
	NLPOP::LoadWordsStringSet(charlexicalpath,	plexical->IcwbWords_set);*/

	plexical->mniPunct_Set.insert("¡£");
	plexical->mniPunct_Set.insert("£»");
	plexical->mniPunct_Set.insert("£¡");
	plexical->mniPunct_Set.insert("£¿");
	plexical->mniPunct_Set.insert("£¬");
	plexical->mniPunct_Set.insert("¡¢");

	//plexical->mniPunct_Set.insert(".");
	plexical->mniPunct_Set.insert(";");
	plexical->mniPunct_Set.insert("!");
	plexical->mniPunct_Set.insert("?");
	plexical->mniPunct_Set.insert(",");


	char sChar[3];
	sChar[2]=0;
	for(set<string>::iterator ssite = plexical->mniPunct_Set.begin(); ssite != plexical->mniPunct_Set.end(); ssite++){
		for(set<string>::iterator site = plexical->IcwbWords_set.begin(); site != plexical->IcwbWords_set.end(); site++){
			for(unsigned long posit = 0; posit < site->length(); ){
				sChar[0] = site->data()[posit++];
				sChar[1] = 0;	
				if(sChar[0] < 0 ){
					sChar[1] = site->data()[posit++];
				}
				if(!strcmp(ssite->data(), sChar) && (ssite->length() != site->length())){
					//AfxMessageBox(_T("uneuqal data in IcwbWords_set and mniPunct_Set..."), MB_OK);
					site = plexical->IcwbWords_set.erase(site);
					if(site != plexical->IcwbWords_set.begin()){
						site--;
					}
					break;
				}
			}
		}
	}

	plexical->Init_Lexicon_Flag = true;
	ExitThread(0);
}













