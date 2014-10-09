#include "StdAfx.h"
#include "CCutor.h"
#include "Clexical.h"
CCutor::CCutor(Clexical& m_Clexical) : m_Clexical(m_Clexical)
{
}
CCutor::~CCutor()
{
}

void CCutor::CCutor_Port(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark)
{
	Cutting_Sentence_by_Given_Segmentation_Symbol_Set(pmSentBuf, pmTokenList, pmTokenMark, m_Clexical.mniPunct_Set);
}

void CCutor::Cutting_Sentence_by_Given_Segmentation_Symbol_Set(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark, set<string>& SegmentSymbol_s)
{
	unsigned long SentLength = strlen(pmSentBuf);
	//if(SentLength > MAX_CLAUSE_LENGTH){
		char SubSentBuf[MAX_SENTENCE];
		char sChar[3];
		sChar[2]=0;
		strcpy_s(SubSentBuf, MAX_SENTENCE, "");
		for(unsigned long posit = 0; posit < SentLength; ){
			sChar[0]=pmSentBuf[posit++];
			sChar[1]=0;	
			if(sChar[0] < 0 ){
				sChar[1]=pmSentBuf[posit++];
			}
			if(SegmentSymbol_s.find(sChar) != SegmentSymbol_s.end()){
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(CHINESE_TOKEN);
				pmTokenList.push_back(sChar);
				pmTokenMark.push_back(PUNCTUATION_TOKEN);
				
				strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			}
			else{
				strcat_s(SubSentBuf, MAX_SENTENCE, sChar);
				if(!(NLPOP::Get_Chinese_Sentence_Length_Counter(SubSentBuf) < MAX_CLAUSE_LENGTH)){
					//AfxMessageBox(_T("Out of MAX_CLAUSE_LENGTH in CCutor::Cutting_Sentence_by_Given_Segmentation_Symbol_Set()..."), MB_OK);
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(CHINESE_TOKEN);
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		if(0 != strlen(SubSentBuf)){
			pmTokenList.push_back(SubSentBuf);
			pmTokenMark.push_back(CHINESE_TOKEN);
		}
	/*}
	else{
		//AfxMessageBox(_T("Out of MAX_CLAUSE_LENGTH in CCutor::Cutting_Sentence_by_Given_Segmentation_Symbol_Set()..."), MB_OK);
		pmTokenList.push_back(pmSentBuf);
		pmTokenMark.push_back(CHINESE_TOKEN);
	}*/
	return;
}

void CCutor::Sentence_Segmenter_With_English_Number_Punctuation(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark)
{
	unsigned long SentLength = strlen(pmSentBuf);
	char SubSentBuf[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	strcpy_s(SubSentBuf, MAX_SENTENCE, "");

	set<string>& Number_Set = m_Clexical.Number_Set;
	set<string>& English_Set = m_Clexical.English_Set;
	set<string>& ChinChar_Set = m_Clexical.ChinChar_Set;
	set<string>& Punctuation_Set = m_Clexical.Punctuation_Set;

	bool FoundChineseTokenFlag = false;
	bool FoundNumeralTokenFlag = false;
	bool FoundEnglishTokenFlag = false;

	for(unsigned long posit = 0; posit < SentLength; )
	{
		sChar[0]=pmSentBuf[posit++];
		sChar[1]=0;	
		if(sChar[0] < 0 )
		{
			sChar[1]=pmSentBuf[posit++];
		}
		//Clause length check...
		if(!((strlen(SubSentBuf) + strlen(sChar)) < MAX_CLAUSE_LENGTH))
		{
			if(FoundChineseTokenFlag)
			{
				pmTokenMark.push_back(CHINESE_TOKEN);
				FoundChineseTokenFlag = false;
			}
			else if(FoundNumeralTokenFlag)
			{
				pmTokenMark.push_back(NUMERAL_TOKEN);
				FoundNumeralTokenFlag = false;
			}
			else if(FoundEnglishTokenFlag)
			{
				pmTokenMark.push_back(ENGLISH_TOKEN);
				FoundEnglishTokenFlag = false;
			}
			pmTokenList.push_back(SubSentBuf);
			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
		}
		if(Number_Set.find(sChar) != Number_Set.end())
		{
			if(!FoundNumeralTokenFlag)
			{
				FoundNumeralTokenFlag = true;
				if(FoundChineseTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(CHINESE_TOKEN);
					FoundChineseTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundEnglishTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(ENGLISH_TOKEN);
					FoundEnglishTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(English_Set.find(sChar) != English_Set.end())
		{
			if(!FoundEnglishTokenFlag)
			{
				FoundEnglishTokenFlag = true;
				if(FoundChineseTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(CHINESE_TOKEN);
					FoundChineseTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundNumeralTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(NUMERAL_TOKEN);
					FoundNumeralTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(ChinChar_Set.find(sChar) != ChinChar_Set.end())
		{
			if(!FoundChineseTokenFlag)
			{
				FoundChineseTokenFlag = true;
				if(FoundEnglishTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(ENGLISH_TOKEN);
					FoundEnglishTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundNumeralTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(NUMERAL_TOKEN);
					FoundNumeralTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(Punctuation_Set.find(sChar) != Punctuation_Set.end())
		{
			if(FoundChineseTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(CHINESE_TOKEN);
				FoundChineseTokenFlag = false;
			}
			else if(FoundNumeralTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(NUMERAL_TOKEN);
				FoundNumeralTokenFlag = false;
			}
			else if(FoundEnglishTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(ENGLISH_TOKEN);
				FoundEnglishTokenFlag = false;
			}
			pmTokenList.push_back(sChar);
			pmTokenMark.push_back(PUNCTUATION_TOKEN);

			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			continue;
		}
		else
		{
			if(FoundChineseTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(CHINESE_TOKEN);
				FoundChineseTokenFlag = false;
			}
			else if(FoundNumeralTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(NUMERAL_TOKEN);
				FoundNumeralTokenFlag = false;
			}
			else if(FoundEnglishTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(ENGLISH_TOKEN);
				FoundEnglishTokenFlag = false;
			}

			
			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			continue;
		}
		strcat_s(SubSentBuf, MAX_SENTENCE, sChar);
	}
	if(FoundChineseTokenFlag)
	{
		pmTokenList.push_back(SubSentBuf);
		pmTokenMark.push_back(CHINESE_TOKEN);
		FoundChineseTokenFlag = false;
	}
	else if(FoundNumeralTokenFlag)
	{
		pmTokenList.push_back(SubSentBuf);
		pmTokenMark.push_back(NUMERAL_TOKEN);
		FoundNumeralTokenFlag = false;
	}
	else if(FoundEnglishTokenFlag)
	{
		pmTokenList.push_back(SubSentBuf);
		pmTokenMark.push_back(ENGLISH_TOKEN);
		FoundEnglishTokenFlag = false;
	}

}





void CCutor::Sentence_Segmenter_Use_Token_Mark(const char* pmSentBuf, list<string>& pmTokenList, list<SEGMENT_TOKEN_TYPE>& pmTokenMark)
{
	set<string>& Number_Set = m_Clexical.Number_Set;
	set<string>& English_Set = m_Clexical.English_Set;
	set<string>& ChinChar_Set = m_Clexical.ChinChar_Set;
	set<string>& Punctuation_Set = m_Clexical.Punctuation_Set;
	set<string>& SpecialSegment_Set = m_Clexical.SpecialSegment_Set;

	unsigned long SentLength = strlen(pmSentBuf);
	char SubSentBuf[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	strcpy_s(SubSentBuf, MAX_SENTENCE, "");

	bool FoundChineseTokenFlag = false;
	bool FoundNumeralTokenFlag = false;
	bool FoundEnglishTokenFlag = false;

	for(unsigned long posit = 0; posit < SentLength; )
	{
		sChar[0]=pmSentBuf[posit++];
		sChar[1]=0;	
		if(sChar[0] < 0 )
		{
			sChar[1]=pmSentBuf[posit++];
		}
		//Clause length check...
		if(!((strlen(SubSentBuf) + strlen(sChar)) < MAX_CLAUSE_LENGTH))
		{
			if(FoundChineseTokenFlag)
			{
				pmTokenMark.push_back(CHINESE_TOKEN);
				FoundChineseTokenFlag = false;
			}
			else if(FoundNumeralTokenFlag)
			{
				pmTokenMark.push_back(NUMERAL_TOKEN);
				FoundNumeralTokenFlag = false;
			}
			else if(FoundEnglishTokenFlag)
			{
				pmTokenMark.push_back(ENGLISH_TOKEN);
				FoundEnglishTokenFlag = false;
			}
			pmTokenList.push_back(SubSentBuf);
			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
		}
		if(SpecialSegment_Set.find(sChar) !=  SpecialSegment_Set.end())
		{
			if(FoundChineseTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(CHINESE_TOKEN);
				FoundChineseTokenFlag = false;
			}
			else if(FoundNumeralTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(NUMERAL_TOKEN);
				FoundNumeralTokenFlag = false;
			}
			else if(FoundEnglishTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(ENGLISH_TOKEN);
				FoundEnglishTokenFlag = false;
			}
			pmTokenList.push_back(sChar);
			pmTokenMark.push_back(SPECIAL_TOKEN);

			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			continue;
		}
		if(Number_Set.find(sChar) != Number_Set.end())
		{
			if(!FoundNumeralTokenFlag)
			{
				FoundNumeralTokenFlag = true;
				if(FoundChineseTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(CHINESE_TOKEN);
					FoundChineseTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundEnglishTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(ENGLISH_TOKEN);
					FoundEnglishTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(English_Set.find(sChar) != English_Set.end())
		{
			if(!FoundEnglishTokenFlag)
			{
				FoundEnglishTokenFlag = true;
				if(FoundChineseTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(CHINESE_TOKEN);
					FoundChineseTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundNumeralTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(NUMERAL_TOKEN);
					FoundNumeralTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(ChinChar_Set.find(sChar) != ChinChar_Set.end())
		{
			if(!FoundChineseTokenFlag)
			{
				FoundChineseTokenFlag = true;
				if(FoundEnglishTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(ENGLISH_TOKEN);
					FoundEnglishTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundNumeralTokenFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(NUMERAL_TOKEN);
					FoundNumeralTokenFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(Punctuation_Set.find(sChar) != Punctuation_Set.end())
		{
			if(FoundChineseTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(CHINESE_TOKEN);
				FoundChineseTokenFlag = false;
			}
			else if(FoundNumeralTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(NUMERAL_TOKEN);
				FoundNumeralTokenFlag = false;
			}
			else if(FoundEnglishTokenFlag)
			{
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(ENGLISH_TOKEN);
				FoundEnglishTokenFlag = false;
			}
			pmTokenList.push_back(sChar);
			pmTokenMark.push_back(PUNCTUATION_TOKEN);

			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			continue;
		}
		else{
			if(FoundChineseTokenFlag){
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(CHINESE_TOKEN);
				FoundChineseTokenFlag = false;
			}
			else if(FoundNumeralTokenFlag){
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(NUMERAL_TOKEN);
				FoundNumeralTokenFlag = false;
			}
			else if(FoundEnglishTokenFlag){
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(ENGLISH_TOKEN);
				FoundEnglishTokenFlag = false;
			}

			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			continue;
		}
		strcat_s(SubSentBuf, MAX_SENTENCE, sChar);
	}
	if(FoundChineseTokenFlag)
	{
		pmTokenList.push_back(SubSentBuf);
		pmTokenMark.push_back(CHINESE_TOKEN);
		FoundChineseTokenFlag = false;
	}
	else if(FoundNumeralTokenFlag)
	{
		pmTokenList.push_back(SubSentBuf);
		pmTokenMark.push_back(NUMERAL_TOKEN);
		FoundNumeralTokenFlag = false;
	}
	else if(FoundEnglishTokenFlag)
	{
		pmTokenList.push_back(SubSentBuf);
		pmTokenMark.push_back(ENGLISH_TOKEN);
		FoundEnglishTokenFlag = false;
	}

}


void CCutor::Sentence_Segmenter(const char* pmSentBuf, list<string>& pmTokenList, list<bool>& pmTokenMark)
{
	set<string>& Number_Set = m_Clexical.Number_Set;
	set<string>& English_Set = m_Clexical.English_Set;
	set<string>& ChinChar_Set = m_Clexical.ChinChar_Set;
	set<string>& Punctuation_Set = m_Clexical.Punctuation_Set;
	set<string>& SpecialSegment_Set = m_Clexical.SpecialSegment_Set;
	set<string>& Quantifier_Set = m_Clexical.Quantifier_Set;

	pmTokenList.clear();
	pmTokenMark.clear();
	unsigned long SentLength = strlen(pmSentBuf);
	char SubSentBuf[MAX_SENTENCE];
	char sChar[3];
	sChar[2]=0;
	strcpy_s(SubSentBuf, MAX_SENTENCE, "");

	bool FoundChinCharFlag = false;
	bool FoundNumberFlag = false;
	bool FoundEnglishFlag = false;
	bool FoundPunctuaFlag = false;
	bool FoundQuantifierFlag = false;
	bool FoundUnRecruitCharFlag = false;
	for(unsigned long posit = 0; posit < SentLength; )
	{
		sChar[0]=pmSentBuf[posit++];
		sChar[1]=0;	
		if(sChar[0] < 0 )
		{
			sChar[1]=pmSentBuf[posit++];
		}
		if(!((strlen(SubSentBuf) + strlen(sChar)) < MAX_CLAUSE_LENGTH))
		{
			pmTokenList.push_back(SubSentBuf);
			if(FoundChinCharFlag)
			{
				pmTokenMark.push_back(true);
			}
			else
			{
				pmTokenMark.push_back(false);
			}
			FoundChinCharFlag = false;
			FoundNumberFlag = false;
			FoundEnglishFlag = false;
			FoundPunctuaFlag = false;
			FoundQuantifierFlag = false;
			FoundUnRecruitCharFlag = false;
			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
		}
		if(0 == strcmp("µÚ",  sChar))
		{
			if(FoundChinCharFlag || FoundNumberFlag || FoundEnglishFlag )
			{
				pmTokenList.push_back(SubSentBuf);
				if(FoundChinCharFlag)
				{
					pmTokenMark.push_back(true);
				}
				else
				{
					pmTokenMark.push_back(false);
				}
				FoundChinCharFlag = false;
				FoundNumberFlag = false;
				FoundEnglishFlag = false;
			}
			pmTokenList.push_back(sChar);
			pmTokenMark.push_back(false);
			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			continue;
		}
		if(Quantifier_Set.find(sChar) != Quantifier_Set.end())
		{
			if(FoundNumberFlag)
			{
				strcat_s(SubSentBuf, MAX_SENTENCE, sChar);
				FoundNumberFlag = false;
				pmTokenList.push_back(SubSentBuf);
				pmTokenMark.push_back(false);
				strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				continue;
			}
		}
		if(Number_Set.find(sChar) != Number_Set.end())
		{
			if(!FoundNumberFlag)
			{
				FoundNumberFlag = true;
				if(FoundChinCharFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(true);
					FoundChinCharFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundEnglishFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(false);
					FoundEnglishFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundPunctuaFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(false);
					FoundPunctuaFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(ChinChar_Set.find(sChar) != ChinChar_Set.end())
		{
			if(!FoundChinCharFlag)
			{
				FoundChinCharFlag = true;
				if(FoundNumberFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(false);
					FoundNumberFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundEnglishFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(false);
					FoundEnglishFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundPunctuaFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(false);
					FoundPunctuaFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(English_Set.find(sChar) != English_Set.end())
		{
			if(!FoundEnglishFlag)
			{
				FoundEnglishFlag = true;
				if(FoundChinCharFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(true);
					FoundChinCharFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundNumberFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(false);
					FoundNumberFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
				else if(FoundPunctuaFlag)
				{
					pmTokenList.push_back(SubSentBuf);
					pmTokenMark.push_back(false);
					FoundPunctuaFlag = false;
					strcpy_s(SubSentBuf, MAX_SENTENCE, "");
				}
			}
		}
		else if(Punctuation_Set.find(sChar) != Punctuation_Set.end())
		{
			if(FoundChinCharFlag || FoundNumberFlag || FoundEnglishFlag )
			{
				pmTokenList.push_back(SubSentBuf);
				if(FoundChinCharFlag)
				{
					pmTokenMark.push_back(true);
				}
				else
				{
					pmTokenMark.push_back(false);
				}
				FoundChinCharFlag = false;
				FoundNumberFlag = false;
				FoundEnglishFlag = false;
			}
			pmTokenList.push_back(sChar);
			pmTokenMark.push_back(false);
			strcpy_s(SubSentBuf, MAX_SENTENCE, "");
			continue;
			
		}
		else
		{
			FoundUnRecruitCharFlag = true;
			if(FoundChinCharFlag || FoundNumberFlag || FoundEnglishFlag || FoundPunctuaFlag )
			{
				pmTokenList.push_back(SubSentBuf);
				if(FoundChinCharFlag)
				{
					pmTokenMark.push_back(true);
				}
				else
				{
					pmTokenMark.push_back(false);
				}
				FoundChinCharFlag = false;
				FoundNumberFlag = false;
				FoundEnglishFlag = false;
				FoundPunctuaFlag = false;
			}
			pmTokenList.push_back(sChar);
			pmTokenMark.push_back(false);
			strcpy_s(SubSentBuf, MAX_SENTENCE, "");

	
			continue;
		}
		strcat_s(SubSentBuf, MAX_SENTENCE, sChar);
	}
	if(FoundChinCharFlag || FoundNumberFlag || FoundEnglishFlag || FoundPunctuaFlag)
	{
		pmTokenList.push_back(SubSentBuf);
		if(FoundChinCharFlag)
		{
			pmTokenMark.push_back(true);
		}
		else
		{
			pmTokenMark.push_back(false);
		}
	}

	if(pmTokenList.size() != pmTokenMark.size())
	{
		string sOutStr = "Segmenter error in CCutor::Sentence_Segmenter()...";
		AfxMessageBox(NLPOP::string2CString(sOutStr), MB_OK);
	}
}





