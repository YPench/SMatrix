#pragma once
#include "SMCom.h"
#include "afxmt.h"



class Clexical
{
public:
	bool Init_Lexicon_Flag;

	map<string, string> TC2SC_Map;
	
	map<string, string> MorphCast;

	set<string> Punctuation_Set;
	set<string> English_Set;
	set<string> Number_Set;
	set<string> PrimWords_Set;
	set<string> Confirm_Set;
	set<string> Pronouns_Set;
	set<string> CommandSet;
	set<string> Polite_Set;
	set<string> APeriod_Set;
	set<string> Quantifier_Set;
	set<string> Numeral_Set;
	set<string> SpecialSegment_Set;

	set<string> CVMC_Set;
	set<string> CSWords_Set;
	set<string> ChinChar_Set;
	set<string> IcwbWords_set;

	set<string> mniPunct_Set;
public:
	Clexical();
	~Clexical();

	void Init();



};
