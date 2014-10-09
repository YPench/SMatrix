#pragma once
#include "afxmt.h"
#include "SMCom.h"

class Clexical;
class CCutor
{

public:
	Clexical& m_Clexical;

public:

	CCutor(Clexical& m_Clexical);
	~CCutor();
	void CCutor_Port(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark);

	void Cutting_Sentence_by_Given_Segmentation_Symbol_Set(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark, set<string>& SegmentSymbol_s);

	void Sentence_Segmenter_With_English_Number_Punctuation(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark);
	void Sentence_Segmenter_Use_Token_Mark(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark);
	void Sentence_Segmenter(const char* pmSentBuf, list<string>& pmTokenList, list<bool>& pmTokenMark);

};
