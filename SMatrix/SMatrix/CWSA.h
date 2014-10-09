#pragma once
#include "afxmt.h"
#include "SMCom.h"
#include "CMaxmatch.h"
#include "CCutor.h"
#include "CSegment.h"
#include "CAmbitor.h"
#include "CIcwb.h"
#include "Clexical.h"

class CWSA
{
public:
	Clexical m_Clexical;
	CSegment m_CSegment;
	ofstream m_out;

	CIcwb m_CIcwb;

public:
	string Executer_Port(string pmService, vector<string> sParam_v, vector<LPVOID> pParam_v);
	
public:
	CWSA();
	~CWSA();

	string Segmentation_Matrix_Port(const char* charinput);

	void Generating_Penn_Chinese_Treebank_Lexicon(const char* charpath, const char* charsave);
	void Penn_Chinese_Treebank_Experiment(string pmFilePath, string pmSaveFolder);
	void Ambiguity_Space_of_Given_Lexicon(set<string>& pmLexicon);
	
	void CRF_Training_Port(const char* parameters, const char* cFilePath);
	void CRF_Running_with_Output_Redirection(const char* cImplementFolder, const char* parameters,  const char* outputFile);
	//----Pase_Sent_File
	void Segment_The_Pointed_Floder_Save_Statistical_Info(string pmFolderPath);
	void Sentence_Segmenting_Training_and_Testing(string pmPath);
	void Segment_Sent_File_to_Deque_With_Secretary(string pmPath, deque<CLAUSEPOS>& PasedPos_d);
	

};
