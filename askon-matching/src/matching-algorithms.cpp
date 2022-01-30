/*
 * matching-algorithms.cpp
 *
 *  Created on: 16 Kas 2021
 *      Author: koray.korkut
 */

#include "matching-algorithms.h"
#include <string-util.h>
#include "damerau-levenshtein-distance.h"
#include "dictionary-txt.h"
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>.

MatchingAlgorithms::MatchingAlgorithms() {
	// TODO Auto-generated constructor stub

	this->dictionary = new DictionaryTxt();
	this->parser = new Parser();
}

MatchingAlgorithms::~MatchingAlgorithms() {
	if (dictionary != NULL)
		delete this->dictionary;

	if (parser != NULL)
		delete this->parser;
}

size_t MatchingAlgorithms::LevenshteinDistance(const string& s1, const string& s2) 
{
	const size_t m = s1.size();
	const size_t n = s2.size();
	if (m == 0)
		return n;
	if (n == 0)
		return m;
	std::vector<size_t> costs(n + 1);
	std::iota(costs.begin(), costs.end(), 0);
	size_t i = 0;
	for (auto c1 : s1) {
		costs[0] = i + 1;
		size_t corner = i;
		size_t j = 0;
		for (auto c2 : s2) {
			size_t upper = costs[j + 1];
			costs[j + 1] = (c1 == c2) ? corner
				: 1 + std::min(std::min(upper, corner), costs[j]);
			corner = upper;
			++j;
		}
		++i;
	}
	return costs[n];
}

bool abreviation_match(const string& word, const string& abr) {
	// GENL GENEL

	if (word.size() <= abr.size())
		return false;

	if (StringUtil::StartsWith(word, abr))
		return true;

	string s = word.substr(0, abr.size() - 1);
	s = s + word[abr.size()];

	if (s == abr && abr.length()>3)
		return true;
	else
		return false;

}




int MatchingAlgorithms::WordsMatch(const string& name1, const string& name2, bool refMin,
		int orderPenalty, int missingWordPenalty, int initialsMatchScore,
		int abbrevation_match_score, bool rawMatch)
{
	if (name1.empty() || name2.empty())
		return 0;

	vector<string> temp_Words1;
	int word_Count1;

	vector<string> temp_Words2;
	int word_Count2;

	vector<string> temp_Words_Max;
	vector<string> temp_Words_Min;

	int max_Word_Count;
	int min_Word_Count;

	string v_Str1;
	string v_Str2;

	int n = 0;
	int penalty = 0;
	int p;
	int k = 0;

	int word_Matches[50];
	int word_Matches_2[50];
	int word_Match_Scores[50];
	int word_Match_Scores_2[50];

	v_Str1 = StringUtil::ReplaceTurkishChars(StringUtil::ToUpper(StringUtil::Trim(name1)));
	v_Str2 = StringUtil::ReplaceTurkishChars(StringUtil::ToUpper(StringUtil::Trim(name2)));

	v_Str1 = StringUtil::Replace(v_Str1, ",", " ");
	v_Str1 = StringUtil::Replace(v_Str1, ".", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "-", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "/", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "  ", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "  ", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "  ", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "  ", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "  ", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "  ", " ");
	v_Str1 = StringUtil::Replace(v_Str1, "  ", " ");


	v_Str2 = StringUtil::Replace(v_Str2, ",", " ");
	v_Str2 = StringUtil::Replace(v_Str2, ".", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "-", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "/", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "  ", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "  ", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "  ", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "  ", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "  ", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "  ", " ");
	v_Str2 = StringUtil::Replace(v_Str2, "  ", " ");


	temp_Words1 = StringUtil::Split(v_Str1,' ');
	temp_Words2 = StringUtil::Split(v_Str2,' ');
	word_Count1 = temp_Words1.size();
	word_Count2 = temp_Words2.size();

	if (word_Count1 == 0 || word_Count2 == 0)
		return 0;

	if (word_Count1 >= word_Count2) {
		temp_Words_Max = temp_Words1;
		temp_Words_Min = temp_Words2;
		max_Word_Count = word_Count1;
		min_Word_Count = word_Count2;
	} else {
		temp_Words_Max = temp_Words2;
		temp_Words_Min = temp_Words1;
		max_Word_Count = word_Count2;
		min_Word_Count = word_Count1;
	}

	if (max_Word_Count >= 50)
		return 0;

	for (p = 0; p < max_Word_Count; p++) {
		word_Matches[p] = -1;
		word_Matches_2[p] = -1;
		word_Match_Scores[p] = 0;
		word_Match_Scores_2[p] = 0;
	}

	for (p = 0; p < max_Word_Count; p++)
		for (k = 0; k < min_Word_Count; k++) {
			bool isNumber = parser->IsNumber(temp_Words_Max[p])
					|| parser->IsNumber(temp_Words_Min[k]);

			if (!isNumber
					&& (temp_Words_Max[p].length() == 1)
					&& (temp_Words_Min[k].length() > 1)
					&& temp_Words_Max[p].substr(0, 1) == temp_Words_Min[k].substr(0, 1))
				n = initialsMatchScore;
			else if (!isNumber
					&& (temp_Words_Max[p].length() > 1)
					&& (temp_Words_Min[k].length() == 1)
					&& temp_Words_Max[p].substr(0, 1) ==
							temp_Words_Min[k].substr(0, 1))
				n = initialsMatchScore;
			else if (std::min(temp_Words_Max[p].length(),
					temp_Words_Min[k].length()) == 0)
				return 0;
			else {
				n = StdWordMatch(temp_Words_Max[p], temp_Words_Min[k], true);
					
				if (n < 70)
					n = 0;
				if (n >= 15
						&& temp_Words_Max[p].substr(0, 1) != temp_Words_Min[k].substr(0, 1))
					n = n - 15;
			}

			if (isNumber && n < 100)
				n = 0;

			if (n < abbrevation_match_score && !isNumber)
				if ((temp_Words_Max[p].length() == 3 || temp_Words_Max[p]
						.length() == 4)
						&& StringUtil::StartsWith(temp_Words_Min[k], temp_Words_Max[p]))
					n = abbrevation_match_score;

			if (n < abbrevation_match_score && !isNumber)
				if ((temp_Words_Min[k].length() == 3 || temp_Words_Min[k]
						.length() == 4)
						&& StringUtil::StartsWith(temp_Words_Max[p], temp_Words_Min[k]))
					n = abbrevation_match_score;

			if (n > word_Match_Scores[p] && n > word_Match_Scores_2[k]) {
				if (word_Matches[p] != -1)
					word_Matches_2[word_Matches[p]] = -1;

				if (word_Matches_2[k] != -1)
					word_Matches[word_Matches_2[k]] = -1;

				word_Match_Scores[p] = n;
				word_Matches[p] = k;
				word_Match_Scores_2[k] = n;
				word_Matches_2[k] = p;
			}
		}

	n = 0;

	int previous_match = 0;
	for (p = 0; p < max_Word_Count; p++) {
		if (word_Matches[p] != -1) {
			n = n + word_Match_Scores[p];
			if (previous_match > word_Matches[p])
				penalty = penalty + orderPenalty;
			previous_match = word_Matches[p];
		}
	}

	penalty = penalty + (max_Word_Count - min_Word_Count)
			* missingWordPenalty;

	p = 0;

	if (rawMatch) {
		string temp_Str_Max = "";
		string temp_Str_Min = "";

		for (int i = 0; i < max_Word_Count; i++)
			if (!parser->IsNumber(temp_Words_Max[i])
					|| word_Match_Scores[i] == 100)
				temp_Str_Max = temp_Str_Max + temp_Words_Max[i] + " ";
			else
				temp_Str_Max = temp_Str_Max + "AAA" + " ";

		temp_Str_Max = StringUtil::Trim(temp_Str_Max);

		for (int i = 0; i < min_Word_Count; i++)
			if (!parser->IsNumber(temp_Words_Min[i])
					|| word_Match_Scores_2[i] == 100)
				temp_Str_Min = temp_Str_Min + temp_Words_Min[i] + " ";
			else
				temp_Str_Min = temp_Str_Min + "BBB" + " ";

		temp_Str_Min = StringUtil::Trim(temp_Str_Min);

		p = StdWordMatch(StringUtil::Replace(temp_Str_Max, "  ", ""), StringUtil::Replace(temp_Str_Min, " ", ""), false);
	}

	return (int) std::max(
			std::max(
					static_cast<int>(std::floor(n
							/ (refMin ? min_Word_Count : max_Word_Count)
							- penalty)), 0), p);

}

int MatchingAlgorithms::PersonNameMatch(const string& match_level, const string& name1, const string& name2)
{	
	if (this->personNameMatchDefinitions.empty())
		LoadScoringDefinition();

	if (personNameMatchDefinitions.find(match_level) != personNameMatchDefinitions.end())
	{
		PersonNameMatchDefinition p = this->personNameMatchDefinitions[match_level];
		return WordsMatch(name1,name2,p.refmin,p.orderPenalty,p.missingWordPenalty,p.initialsMatchScore,p.abbrevationMatchScore,false);
	}
	else
		return 0;
}




void MatchingAlgorithms::LoadScoringDefinition()
{
		companyNameMatchPatterns.clear();
		personNameMatchDefinitions.clear();
//		addressMatchDefinitions.clear();

		if (!dictionary->GetScoringDefinitions().empty())
			LoadScoringDefinition(dictionary->GetScoringDefinitions(), companyNameMatchPatterns, personNameMatchDefinitions);
}


void MatchingAlgorithms::LoadScoringDefinition(string definition,
		map<string,map<string,CompanyNameMatchPattern>>& companyNameMatchPatterns,
		map<string,PersonNameMatchDefinition>& personNameMatchDefinitions)
{
	string state="BEGIN";
	set<string> allPatternSet = {"ABCXX","AXX","ABNXX","ANCBX","ABNEX","ACDEB","ANCBN","ABXXX","ADCN","ADCNX","ACBED","ABNNN","ADBCX","ACBDX","ABNNE","ADCNN","ABDCX","ABNNC","ADCB","ANBXX","ABNND","ANCDE","ANCDB","ABEDX",
			"ADBCE","ABEDC","ACEXX","ACBDN","ADCX","ABDCE","ABNNX","ANBD","ABEDN","ADBCN","ANBC","ABDCN","ACBDE","AEBCD","ABDN","ACDBE","A","ADCEB","ACX","ADBN","ABECX","ADBEC","ABDC","ABNDN","ADBC","ACDBN","ACNDX","ADBX","AXXX","ABNDC","ACB","ANBX","ABNDE","ADBNC",
			"ABECD","ACNXX","ADBNE","ANCDX","ADCNB","ACXX","ACDBX","ADCNE","ANCDN","ANCB","ACN","ABDX","ABNCX","ADBNN","ANCD","ABECN","ABDXX","ADBEX","ABCN","ABX","ABNEN","ADCEX","AECXX","ABCD","ABEND","ANCX","ABC","ABNED","ABENC","ANBDX","ADCEN","ABCNN","ABNEC",
			"ABCNX","ABN","ADBEN","ABCX","ABNDX","ADBXX","ACBXX","ACNDB","ACNDE","ADCBE","ANBCN","ABCEN","ABCND","ABCNE","ADCBN","ANBCX","ABCED","ACEDX","AEBXX","ADCBX","ANBDE","ANCXX","ABENN","ANBDC","ABNN","ACNB","ACEDN","ABDNX","ANBDN","ABDNN","ABENX","ANC",
			"ABCEX","ACEDB","ANB","ACND","ACNDN","ABNX","AXXXX","ABNC","ABND","ACEBX","ABCDN","AX","ADBNX","ACNBX","ABNCN","ACDXX","AECBX","ABCDE","ACNX","ANBCE","ANBCD","AB","AECBN","ACEBN","ABCDX","ABNCE","ABNCD","ACEBD","AECBD","ACNBE","ACNBD","AEBCN","AECDB",
			"ACBEN","AEBDC","ACBEX","ACNBN","ACBNN","ACDNX","ACDNN","ADXX","AEBCX","ACBNX","ACBND","ACDX","ACDEN","ABEXX","ACBNE","ACDNE","ABDEN","ABDNC","AEBDN","ABXX","ABDNE","AECDX","ACBX","ANCBE","ABDEC","ACDNB","ANCBD","ACBD","AECDN","ADCXX","ACDB","ABDEX",
			"ACBN","ACDN","AEBDX","ACDEX"};

	set<string> allValidPropertiesSet = {"Match_Type", "Match_Level","Pattern","Refmin","MissingWordPenalty",
			"OrderPenalty","InitialsMatchScore","AdjustScore","FirstWordScoreLimits","SecondWordScoreLimits","FirstWordNonCommonBonus","FirstWordCommonPenalty",
			"SecondWordNonCommonBonus","ScoreNonMatching","SecondWordCommonPenalty","EnableMultiPositiveAdjustment","EnableMultiNegativeAdjustment","AbbrevationMatchScore","SanTicMismatchPenalty","IgnoreXXIfPossible","MandatoryMatchWordThreshold","Rule","Additional_Rule_Sequence",
			"Mahalle_Match_Threshold","Belde_Koy_Match_Threshold","Yol_Match_Threshold","Site_Match_Threshold","Bina_Match_Threshold",
			"Blok_Match_Threshold","Diger_Match_Threshold","Kurum_Match_Threshold"};

/*	set<string> allValidAddressConditionTokens = {"&&","||","(",")",
			"IL1_NULL","ILCE1_NULL","SEMT1_NULL","MAHALLE1_NULL","REF_MAHALLE1_NULL","BELDE_KOY1_NULL","REF_BELDE_KOY1_NULL","YOL1_NULL","REF_YOL1_NULL","SITE1_NULL","BINA1_NULL","BLOK1_NULL","NO1_NULL","DAIRE1_NULL","DIGER1_NULL","KURUM1_NULL",
			"IL2_NULL","ILCE2_NULL","SEMT2_NULL","MAHALLE2_NULL","REF_MAHALLE2_NULL","BELDE_KOY2_NULL","REF_BELDE_KOY2_NULL","YOL2_NULL","REF_YOL2_NULL","SITE2_NULL","BINA2_NULL","BLOK2_NULL","NO2_NULL","DAIRE2_NULL","DIGER2_NULL","KURUM2_NULL",
			"IL_MATCH","ILCE_MATCH","SEMT_MATCH","MAHALLE_MATCH","BELDE_KOY_MATCH","YOL_MATCH","SITE_MATCH","BINA_MATCH","BLOK_MATCH","NO_MATCH","DAIRE_MATCH","DIGER_MATCH","KURUM_MATCH",
			"IL_MISMATCH","ILCE_MISMATCH","SEMT_MISMATCH","MAHALLE_MISMATCH","BELDE_KOY_MISMATCH","YOL_MISMATCH","SITE_MISMATCH","BINA_MISMATCH","BLOK_MISMATCH","NO_MISMATCH","DAIRE_MISMATCH",
			"MAHALLE_SCORE","BELDE_KOY_SCORE","YOL_SCORE","SITE_SCORE","BINA_SCORE","BLOK_SCORE","DIGER_SCORE","KURUM_SCORE"};*/

	map<string,CompanyNameMatchPattern> *currentCompanyNameMatchLevelPatterns;
	vector<CompanyNameMatchPattern> *currentCompanyNameMatchPatternList;

//	string currentAddressRuleSequence="Default";

	PersonNameMatchDefinition *currentPersonNameMatchDefinition;
//	AddressMatchDefinition currentAddressMatchDefinition;

	int lineNumber = 0;

	try {
		string matchType="";
		string matchLevel="";
		map<string,CompanyNameMatchPattern> currentMatchPatterns;
		istringstream  inputStream(definition);

		string strLine;
		vector<string> fields;
		while (std::getline(inputStream, strLine)) {
			lineNumber++;
			if (!StringUtil::StartsWith(StringUtil::Trim(strLine), "#"))
			{

				map<string,string> prop;

				vector<string> lineItems = StringUtil::Split(strLine, '=');

				if (lineItems.size() != 2)
					continue;

				lineItems[0] = StringUtil::Trim(lineItems[0]);
				lineItems[1] = StringUtil::Trim(lineItems[1]);

				if (allValidPropertiesSet.find(lineItems[0]) == allValidPropertiesSet.end())
					throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Property " + lineItems[0] +" is not valid");


				if (lineItems[0] == "Match_Type")
				{
					if (state != "GET_PROPERTIES" && state != "BEGIN")
					   throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Match_Type property is not expected at this state");

					matchType = lineItems[1];
					if (matchType == "PersonName" || matchType == "CompanyName" || matchType =="Address")
						state="GET_MATCH_LEVEL";
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid Match_Type : "+ matchType);
				}


				if (lineItems[0] == "Match_Level")
				{
					if (state != "GET_MATCH_LEVEL")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Match_Level property is not expected at this state");

					matchLevel = lineItems[1];

					if (matchLevel !="Strict" && matchLevel != "Loose" && matchLevel !="Typical")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid Match_Level : "+ matchLevel);

					state="GET_PROPERTIES";

					if (matchType == "CompanyName")
					{
						if (companyNameMatchPatterns.find(matchLevel) == companyNameMatchPatterns.end())
							companyNameMatchPatterns.insert(std::pair<string, map<string,CompanyNameMatchPattern>>(matchLevel, map<string,CompanyNameMatchPattern>()));
						else
							throw std::runtime_error("Error on Line:" + to_string(lineNumber) + "-" + "Duplicate " + matchLevel + " Match_Level definition for " + matchType);

						currentCompanyNameMatchLevelPatterns = &companyNameMatchPatterns[matchLevel];

					}
					else if (matchType =="PersonName")
					{
						if (personNameMatchDefinitions.find(matchLevel) == personNameMatchDefinitions.end()) {
							PersonNameMatchDefinition p;
							p.refmin=true;
							p.missingWordPenalty=7;
							p.orderPenalty=10;
							p.initialsMatchScore=90;
							p.abbrevationMatchScore=0;
							personNameMatchDefinitions.insert(std::pair<string, PersonNameMatchDefinition>(matchLevel, p));
						}
						else
							throw std::runtime_error("Error on Line:" + to_string(lineNumber) + "-" + "Duplicate " + matchLevel + " Match_Level definition for " + matchType);

						currentPersonNameMatchDefinition= &personNameMatchDefinitions[matchLevel];
					}
/*					else if (matchType == "Address")
					{
						if (addressMatchDefinitions.find(matchLevel) == addressMatchDefinitions.end())
							addressMatchDefinitions.insert(std::pair<string, AddressMatchDefinition>(matchLevel, AddressMatchDefinition());
						else
							throw std::runtime_error("Error on Line:" + to_string(lineNumber) + "-" + "Duplicate " + matchLevel + " Match_Level definition for " + matchType);

						currentAddressMatchDefinition=addressMatchDefinitions[matchLevel];

						currentAddressRuleSequence="Default";
					}
*/
				}


				if (lineItems[0] == "Refmin")
				{
					if (state !="GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Refmin property is not expected at this state");

					if (matchType =="PersonName")
					{
						string value = StringUtil::ToUpper(lineItems[1]);
						if (value == "TRUE")
							currentPersonNameMatchDefinition->refmin=true;
						else if (value == "FALSE")
							currentPersonNameMatchDefinition->refmin=false;
						else
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid Refmin property value = " + lineItems[1]);
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Refmin property not applicable to " + matchType);
				}


				if (lineItems[0] == "MissingWordPenalty")
				{
					if (state !="GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "MissingWordPenalty property is not expected at this state");

					if (matchType == "PersonName") {
						currentPersonNameMatchDefinition->missingWordPenalty=std::stoi(lineItems[1]);
						if (currentPersonNameMatchDefinition->missingWordPenalty < 0 || currentPersonNameMatchDefinition->missingWordPenalty > 100)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "MissingWordPenalty value must be between 0-100");
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "MissingWordPenalty property not applicable to " + matchType);
				}


				if (lineItems[0] == "OrderPenalty")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "OrderPenalty property is not expected at this state");

					if (matchType =="PersonName")
					{
						currentPersonNameMatchDefinition->orderPenalty= std::stoi(lineItems[1]);
						if (currentPersonNameMatchDefinition->orderPenalty < 0 || currentPersonNameMatchDefinition->orderPenalty > 100)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "OrderPenalty value must be between 0-100");
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "OrderPenalty property not applicable to " + matchType);
				}

				if (lineItems[0] == "InitialsMatchScore")
				{
					if (state !="GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "InitialsMatchScore property is not expected at this state");

					if (matchType == "PersonName") {
						currentPersonNameMatchDefinition->initialsMatchScore=std::stoi(lineItems[1]);
						if (currentPersonNameMatchDefinition->initialsMatchScore < 0 || currentPersonNameMatchDefinition->initialsMatchScore > 100)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "InitialsMatchScore value must be between 0-100");
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "InitialsMatchScore property not applicable to " + matchType);
				}

				if (lineItems[0] == "AbbrevationMatchScore")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "AbbrevationMatchScore property is not expected at this state");

					if (matchType == "PersonName") {
						currentPersonNameMatchDefinition->abbrevationMatchScore=std::stoi(lineItems[1]);
						if (currentPersonNameMatchDefinition->abbrevationMatchScore < 0 || currentPersonNameMatchDefinition->abbrevationMatchScore > 100)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "AbbrevationMatchScore value must be between 0-100");
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "AbbrevationMatchScore property not applicable to " + matchType);
				}

				if (lineItems[0] == "Pattern")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Pattern property is not expected at this state");

					if (matchType == "CompanyName")
					{
						string patternString = lineItems[1];
						vector<string> patternStringList = StringUtil::Split(patternString, ',');
						currentCompanyNameMatchPatternList = new vector<CompanyNameMatchPattern>();
						if (patternStringList.size() == 0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "No pattern defined");

						for (string pattern:patternStringList)
						{
							string p = StringUtil::Trim(pattern);
							if (allPatternSet.find(p) == allPatternSet.end())
								throw std::runtime_error("Error on Line:" + to_string(lineNumber) + "-" + "Pattern " + p + " is not valid");

							if (currentCompanyNameMatchLevelPatterns->find(p) == currentCompanyNameMatchLevelPatterns->end())
								currentCompanyNameMatchLevelPatterns->insert(std::pair<string,CompanyNameMatchPattern>(p, CompanyNameMatchPattern(p)));
							else
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Duplicate pattern " + p);

							currentCompanyNameMatchPatternList->push_back((*currentCompanyNameMatchLevelPatterns)[p]);
						}
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Pattern property not applicable to " + matchType);
				}

				if (lineItems[0] == "AdjustScore")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "AdjustScore property is not expected at this state");

					if (matchType == "CompanyName")
					{
						string adjustScoreString = lineItems[1];
						vector<string> adjustScoreStringList = StringUtil::Split(adjustScoreString, ',');

						if (adjustScoreStringList.size() == 0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "No score adjustment defined");

						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
						{
							currentPattern.scoreAdjustmentList.clear();
							for (string s:adjustScoreStringList)
							{
								vector<string> tokens = StringUtil::Split(s, '>');
								if (tokens.size() != 2)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid score adjustment value");

								vector<string> range = StringUtil::Split(tokens[0], '-');
								if (range.size() != 2)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid score adjustment value");

								ScoreAdjustmentRange scoreAdjustmentRange(
										stoi(StringUtil::Trim(range[0])),
										stoi(StringUtil::Trim(range[1])),
										stoi(StringUtil::Trim(tokens[1])));

								if (scoreAdjustmentRange.minScore < 0 || scoreAdjustmentRange.minScore > 100)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Score adjustment min score value must be between 0-100");

								if (scoreAdjustmentRange.maxScore < 0 || scoreAdjustmentRange.maxScore > 100)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Score adjustment max score value must be between 0-100");

								if (scoreAdjustmentRange.maxScore < scoreAdjustmentRange.minScore)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Score adjustment max score value must be greater or equal to min score");

								if (scoreAdjustmentRange.adjustmentScore < 0 || scoreAdjustmentRange.adjustmentScore > 100)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Score adjustment value must be between 0-100");

								currentPattern.scoreAdjustmentList.push_back(scoreAdjustmentRange);
							}
						}
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "AdjustScore property not applicable to " + matchType);
				}

				if (lineItems[0] == "FirstWordScoreLimits")
				{
					if (state !="GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordScoreLimits property is not expected at this state");

					if (matchType == "CompanyName")
					{
						string scoreLimitString = lineItems[1];
						vector<string> scoreLimitStringList = StringUtil::Split(scoreLimitString, ',');

						if (scoreLimitStringList.size() == 0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "No score limit defined");

						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
						{
							currentPattern.firstWordScoreLimitList.clear();
							for (string s:scoreLimitStringList)
							{
								vector<string> tokens = StringUtil::Split(s, '<');
								if (tokens.size() != 2)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid score limit value");

								FirstWordScoreLimit firstWordScoreLimit(
										stoi(StringUtil::Trim(tokens[1])),
										stoi(StringUtil::Trim(tokens[0])));

								if (firstWordScoreLimit.limitScore < 0 || firstWordScoreLimit.limitScore > 100)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Score limit value must be between 0-100");

								if (firstWordScoreLimit.adjustmentScore < 0 || firstWordScoreLimit.adjustmentScore > 100)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Adjustment score value must be between 0-100");

								currentPattern.firstWordScoreLimitList.push_back(firstWordScoreLimit);
							}
						}
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordScorelimits property not applicable to " + matchType);
				}


				if (lineItems[0] == "SecondWordScoreLimits")
				{
					if (state !="GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordScoreLimits property is not expected at this state");

					if (matchType =="CompanyName")
					{
						string scoreLimitString = lineItems[1];
						vector<string> scoreLimitStringList = StringUtil::Split(scoreLimitString, ',');

						if (scoreLimitStringList.size() == 0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "No score limit defined");

						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
						{
							currentPattern.secondWordScoreLimitList.clear();
							for (string s:scoreLimitStringList)
							{
								vector<string> tokens = StringUtil::Split(s,'<');
								if (tokens.size() != 2)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid score limit value");

								FirstWordScoreLimit secondWordScoreLimit(
										stoi(StringUtil::Trim(tokens[1])),
										stoi(StringUtil::Trim(tokens[0])));

								if (secondWordScoreLimit.limitScore < 0 || secondWordScoreLimit.limitScore > 100)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Score limit value must be between 0-100");

								if (secondWordScoreLimit.adjustmentScore < 0 || secondWordScoreLimit.adjustmentScore > 100)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Adjustment score value must be between 0-100");

								currentPattern.secondWordScoreLimitList.push_back(secondWordScoreLimit);
							}
						}
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordScoreLimits property not applicable to " + matchType);
				}


				if (lineItems[0] == "ScoreNonMatching")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "ScoreNonMatching property is not expected at this state");

					if (matchType == "CompanyName")
					{
						string scoreNonMatchingString = lineItems[1];
						vector<string> scoreNonMatchingStringList = StringUtil::Split(scoreNonMatchingString, ',');

						if (scoreNonMatchingStringList.size() == 0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "No score nonmatching option defined");

						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
						{
							currentPattern.nonMatchingOptions.clear();
							for (string s:scoreNonMatchingStringList)
							{
								if (s == "FirstTwoWords")
									currentPattern.nonMatchingOptions.push_back(EnumScoreNonMatchingOptions::FIRST_THREE);
								else if (s == "LongerThanTwo")
									currentPattern.nonMatchingOptions.push_back(EnumScoreNonMatchingOptions::LONGER_THAN_TWO);
								else if (s =="All")
									currentPattern.nonMatchingOptions.push_back(EnumScoreNonMatchingOptions::ALL);
								else if (s == "None")
									currentPattern.nonMatchingOptions.push_back(EnumScoreNonMatchingOptions::NONE);
								else
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid score nonmatching option " + s);
							}
						}
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "ScoreNonMatching property not applicable to " + matchType);
				}


				if (lineItems[0] == "IgnoreXXIfPossible")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "IgnoreXXIfPossible property is not expected at this state");

					if (matchType == "CompanyName")
					{
						string value = StringUtil::ToUpper(lineItems[1]);
						if (value == "TRUE")
							for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
									currentPattern.ignoreXXIfPossible=true;
						else if (value == "FALSE")
							for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
								currentPattern.ignoreXXIfPossible=false;
						else
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid IgnoreXXIfPossible value " + value);
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "IgnoreXXIfPossible property not applicable to " + matchType);
				}

				if (lineItems[0] == "EnableMultiPositiveAdjustment")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "EnableMultiPositiveAdjustment property is not expected at this state");

					if (matchType == "CompanyName")
					{
						string value = StringUtil::ToUpper(lineItems[1]);
						if (value == "TRUE")
							for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
									currentPattern.enableMultiPositiveAdjustment=true;
						else if (value == "FALSE")
							for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
								currentPattern.enableMultiPositiveAdjustment=false;
						else
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid EnableMultiPositiveAdjustment value " + value);
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "EnableMultiPositiveAdjustment property not applicable to " + matchType);
				}

				if (lineItems[0] == "EnableMultiNegativeAdjustment")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "EnableMultiNegativeAdjustment property is not expected at this state");

					if (matchType == "CompanyName")
					{
						string value = StringUtil::ToUpper(lineItems[1]);
						if (value == "TRUE")
							for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
									currentPattern.enableMultiNegativeAdjustment=true;
						else if (value == "FALSE")
							for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
								currentPattern.enableMultiNegativeAdjustment=false;
						else
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid EnableMultiNegativeAdjustment value " + value);
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "EnableMultiNegativeAdjustment property not applicable to " + matchType);
				}

				if (lineItems[0] == "FirstWordNonCommonBonus")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordNonCommonBonus property is not expected at this state");

					if (!parser->IsNumber(lineItems[1]))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordNonCommonBonus value must be a number between 0-100");

					int intVal = stoi(lineItems[1]);

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordNonCommonBonus value must be between 0-100");

					if (matchType == "CompanyName")
						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
							currentPattern.FirstWordNonCommonBonus = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordNonCommonBonus property not applicable to " + matchType);
				}

				if (lineItems[0] == "SecondWordNonCommonBonus")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordNonCommonBonus property is not expected at this state");

					if (!parser->IsNumber(lineItems[1]))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordNonCommonBonus value must be a number between 0-100");

					int intVal = stoi(lineItems[1]);

					if (intVal < 0 || intVal > 100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordNonCommonBonus value must be between 0-100");

					if (matchType == "CompanyName")
						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
							currentPattern.SecondWordNonCommonBonus = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordNonCommonBonus property not applicable to " + matchType);
				}

				if (lineItems[0] == "FirstWordCommonPenalty")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordCommonPenalty property is not expected at this state");

					if (!parser->IsNumber(lineItems[1]))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordCommonPenalty value must be a number between 0-100");

					int intVal = stoi(lineItems[1]);

					if (intVal < 0 || intVal > 100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordCommonPenalty value must be between 0-100");

					if (matchType == "CompanyName")
						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
							currentPattern.FirstWordCommonPenalty = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "FirstWordCommonPenalty property not applicable to " + matchType);
				}

				if (lineItems[0] == "SecondWordCommonPenalty")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordCommonPenalty property is not expected at this state");

					if (!parser->IsNumber(lineItems[1]))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordCommonPenalty value must be a number between 0-100");

					int intVal = stoi(lineItems[1]);

					if (intVal < 0 || intVal > 100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordCommonPenalty value must be between 0-100");

					if (matchType == "CompanyName")
						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
							currentPattern.SecondWordCommonPenalty = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SecondWordCommonPenalty property not applicable to " + matchType);
				}

				if (lineItems[0] == "MandatoryMatchWordThreshold")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "MandatoryMatchWordThreshold property is not expected at this state");

					if (!parser->IsNumber(lineItems[1]))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "MandatoryMatchWordThreshold value must be a number between 0-100");

					int intVal = stoi(lineItems[1]);

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "MandatoryMatchWordThreshold value must be between 0-100");

					if (matchType == "CompanyName")
						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
							currentPattern.MandatoryMatchWordThreshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "MandatoryMatchWordThreshold property not applicable to " + matchType);
				}

				if (lineItems[0] == "SanTicMismatchPenalty")
				{
					if (state != "GET_PROPERTIES")
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SanTicMismatchPenalty property is not expected at this state");

					if (!parser->IsNumber(lineItems[1]))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SanTicMismatchPenalty value must be a number between 0-100");

					int intVal = stoi(lineItems[1]);

					if (intVal < 0 || intVal > 100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SanTicMismatchPenalty value must be between 0-100");

					if (matchType == "CompanyName")
						for (CompanyNameMatchPattern currentPattern: *currentCompanyNameMatchPatternList)
							currentPattern.SanTicMismatchPenalty = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "SanTicMismatchPenalty property not applicable to " + matchType);
				}

/*
				if (lineItems[0] == "Kurum_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Kurum_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Kurum_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Kurum_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).kurum_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Kurum_Match_Threshold property not applicable to " + matchType));
				}

				if (lineItems[0] == "Diger_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Diger_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Diger_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Diger_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).diger_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Diger_Match_Threshold property not applicable to " + matchType));
				}

				if (lineItems[0] == "Blok_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Blok_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Blok_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Blok_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).blok_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Blok_Match_Threshold property not applicable to " + matchType));
				}

				if (lineItems[0] == "Bina_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Bina_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Bina_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Bina_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).bina_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Bina_Match_Threshold property not applicable to " + matchType));
				}

				if (lineItems[0] == "Site_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Site_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Site_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Site_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).site_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Site_Match_Threshold property not applicable to " + matchType));
				}

				if (lineItems[0] == "Yol_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Yol_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Yol_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Yol_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).yol_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Yol_Match_Threshold property not applicable to " + matchType));
				}

				if (lineItems[0] == "Belde_Koy_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Belde_Koy_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Belde_Koy_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Belde_Koy_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).belde_koy_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Belde_Koy_Match_Threshold property not applicable to " + matchType));
				}


				if (lineItems[0] == "Mahalle_Match_Threshold")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Mahalle_Match_Threshold property is not expected at this state"));

					int intVal = Integer.valueOf(prop.getProperty("Mahalle_Match_Threshold").trim());

					if (intVal<0 || intVal>100)
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Mahalle_Match_Threshold value must be between 0-100"));

					if (matchType.equals("Address"))
						addressMatchDefinitions.get(matchLevel).mahalle_match_threshold = intVal;
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Mahalle_Match_Threshold property not applicable to " + matchType));
				}


				if (lineItems[0] == "Additional_Rule_Sequence")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Additional_Rule_Sequence property is not expected at this state"));

					if (matchType.equals("Address"))
					{
						currentAddressRuleSequence = prop.getProperty("Additional_Rule_Sequence");
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Additional_Rule_Sequence property is not applicable to " + matchType));
				}

				if (lineItems[0] == "Rule")
				{
					if (!state.equals("GET_PROPERTIES"))
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Rule property is not expected at this state"));

					String rule = prop.getProperty("Rule").trim();
					rule = rule.replace(" +", " ");

					if (matchType.equals("Address"))
					{
						AddressMatchRule addressMatchRule = new AddressMatchRule();

						rule = rule.toUpperCase();

						addressMatchRule.description = rule;

						if (!rule.startsWith("IF "))
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Rule property must start with If keyword"));

						rule = rule.substring(2).trim();

						if (!rule.contains("THEN"))
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Rule property must contain Then keyword"));

						String conditions = rule.substring(0,rule.indexOf("THEN")).trim();

						if (conditions.length()==0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Rule property does not contain condition clause"));

						String actionString = rule.substring(rule.indexOf("THEN")+4).trim();

						if (actionString.length()==0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Rule property does not contain any action"));

						String[] actionTokens = actionString.trim().split(" +");

						if (actionTokens.length==0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Rule property does not contain any action"));

						String scoreAdjustment = actionTokens[0];

						if (scoreAdjustment.charAt(0)!='+' && scoreAdjustment.charAt(0)!='-' && (scoreAdjustment.charAt(0)<'0' ) || scoreAdjustment.charAt(0)>'9')
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Then keyword must be followed by a valid adjustment score"));

						int adjustmentType=0;
						int adjustmentScore=0;
						if (scoreAdjustment.charAt(0)=='+' || scoreAdjustment.charAt(0)=='-')
						{
							if (currentAddressRuleSequence.equals("Default"))
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Relative score adjustment is only allowed in Addtional Rule Sequence Definitions"));

							if (scoreAdjustment.length()<=1)
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Then keyword must be followed by a valid adjustment score"));
							if (scoreAdjustment.charAt(0)=='+')
								adjustmentType=1;
							else
								adjustmentType=-1;

							adjustmentScore = Integer.valueOf(scoreAdjustment.substring(1));

							if (adjustmentScore<0 || adjustmentScore>100)
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Adjustment score value must be between 0-100"));
						}
						else
						{
							adjustmentScore = Integer.valueOf(scoreAdjustment);

							if (adjustmentScore<0 || adjustmentScore>100)
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Adjustment score value must be between 0-100"));
						}

						if (actionTokens.length>1)
						{
							if (!actionString.contains("ADDITIONAL SEQUENCE"))
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid additional sequence definition"));

							String additionalSequenceList = actionString.substring(actionString.indexOf("ADDITIONAL SEQUENCE")+19).trim();

							if (additionalSequenceList.length()==0)
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Additional sequence list is empty"));

							addressMatchRule.additionalRules = additionalSequenceList.split(",");

						}
						addressMatchRule.scoreAdjustment = adjustmentScore;
						addressMatchRule.scoreAdjustmentType = adjustmentType;
						conditions = conditions.replaceAll("\\("," ( ");
						conditions = conditions.replaceAll("\\)"," ) ").trim();
						addressMatchRule.tokens = conditions.split(" +");

						String parseState="condition";
						int openParantheses = 0;

						for (String token:addressMatchRule.tokens)
						{
							if (!allValidAddressConditionTokens.contains(token))
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid address rule condition token:"+token));

							if (token.equals("&&"))
							{
								if (parseState.equals("condition"))
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid address rule condition"));

								parseState="condition";
							}
							else if (token.equals("||"))
							{
								if (parseState.equals("condition"))
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid address rule condition"));

								parseState="condition";
							}
							else if (token.equals("("))
							{
								if (!parseState.equals("condition") && !parseState.equals("negate"))
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid address rule condition"));

								openParantheses++;
							}
							else if (token.equals(")"))
							{
								if (!parseState.equals("operator") || openParantheses<=0)
									throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid address rule condition"));

								openParantheses--;
								parseState="operator";
							}
							else if (parseState.equals("condition") || parseState.equals("negate")) {
								parseState="operator";
							}
							else
								throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid address rule condition"));
						}
						if (openParantheses!=0)
							throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Invalid address rule condition"));

						if (currentAddressRuleSequence.equals("Default"))
							addressMatchDefinitions.get(matchLevel).rules.add(addressMatchRule);
						else
						{
							if (!addressMatchDefinitions.get(matchLevel).additionalRuleMap.containsKey(currentAddressRuleSequence))
								addressMatchDefinitions.get(matchLevel).additionalRuleMap.put(currentAddressRuleSequence, new ArrayList<AddressMatchRule>());
							addressMatchDefinitions.get(matchLevel).additionalRuleMap.get(currentAddressRuleSequence).add(addressMatchRule);
						}
					}
					else
						throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + "Rule property is not applicable to " + matchType));

				}
*/
			}
		}
	}
	catch (const runtime_error& error)
	{//Catch exception if any
		string message = std::string(error.what());
		if (!StringUtil::StartsWith(message, "Error on Line:"))
			throw std::runtime_error("Error on Line:" + std::to_string(lineNumber) + "-" + message);
		else
			throw (error);
	}

	if (personNameMatchDefinitions.find("Typical") == personNameMatchDefinitions.end())
		throw std::runtime_error("Missing definition: Typical match level for PersonName is not defined");

	if (personNameMatchDefinitions.find("Strict") == personNameMatchDefinitions.end())
		throw std::runtime_error("Missing definition: Strict match level for PersonName is not defined");

	if (personNameMatchDefinitions.find("Loose") == personNameMatchDefinitions.end())
		throw std::runtime_error("Missing definition: Loose match level for PersonName is not defined");

	if (companyNameMatchPatterns.find("Typical") == companyNameMatchPatterns.end())
		throw std::runtime_error("Missing definition: Typical match level for CompanyName is not defined");

	if (companyNameMatchPatterns.find("Strict") == companyNameMatchPatterns.end())
		throw std::runtime_error("Missing definition: Strict match level for CompanyName is not defined");

	if (companyNameMatchPatterns.find("Loose") == companyNameMatchPatterns.end())
		throw std::runtime_error("Missing definition: Loose match level for CompanyName is not defined");

	map<string,map<string,CompanyNameMatchPattern>>:: iterator it;

	for (it = companyNameMatchPatterns.begin(); it != companyNameMatchPatterns.end(); it++)
	{
		for (string pattern:allPatternSet)
           if (it->second.find(pattern) == it->second.end())
    			throw std::runtime_error("Missing definition: Pattern " + pattern + " not defined for CompanyName " + it->first + " match level");

	}
}


void removeCrossMatchingIndex(vector<int>& array, int index)
{
	for (int i = 0; i < array.size(); i++)
		if (array[i] != -1)
			if (array[i] > index)
				array[i] = array[i] - 1;
}


string MatchingAlgorithms::StandardizeWord(const string& word)
{
	size_t length = word.length();
	if (length == 0)
		return word;
	
	string returnValue = "";
	returnValue += word[0];
	char prevChar = word[0];
	for (size_t i = 1; i < word.length(); i++)
	{
		if (word[i] != prevChar) {
			switch (word[i])
			{
				case 'P': returnValue += 'B';break;
				case 'T': returnValue += 'D';break;
				case 'J': returnValue += 'C';break;
				default : returnValue += word[i];break;
			}

			prevChar = word[i];
		}		
	}

	return returnValue;
}

int MatchingAlgorithms::StdWordMatch(const string& name1, const string& name2, bool standardize)
{	
	int length1 = name1.length();
	int length2 = name2.length();

	if (length1 == 0 || length2 == 0)
		return 0;

	if (std::abs(length1 - length2) / std::min(length1,length2) > 0.3)
		return 0;

	bool preMatchTest = false;
	bool firstMatch = name1[0] == name2[0];

	if (firstMatch)
	{
		bool secondMatch = length1 > 1 && length2 > 1 ? name1[1] == name2[1] || (name1[1] == 'P' && name2[1] == 'B') || (name1[1] == 'B' && name2[1] == 'P') : false;
		bool thirdMatch = length1 > 2 && length2 > 2 ? name1[2] == name2[2] || (name1[2] == 'P' && name2[2] == 'B') || (name1[2] == 'B' && name2[2] == 'P') : false;
		bool thirdAndSecondMatch = length1 > 4 && length2 > 4 ? name1[2] == name2[1] || name1[1] == name2[2] : false;
		preMatchTest = secondMatch || thirdMatch || thirdAndSecondMatch;
	}

	if (!preMatchTest) 
		preMatchTest = length1 > 3 && length2 > 3 ? name1[1] == name2[1] && name1[2] == name2[2] && name1[3] && name2[3] : false;

	if (preMatchTest)
	{
		int distance = LevenshteinDistance(name1, name2);
		int score = 100 - distance * 100 / std::max(length1, length2);

		if (standardize && score >= 70)
		{
			int distance2 = LevenshteinDistance(StandardizeWord(name1), StandardizeWord(name2));
			int score2 = 100 - distance2 * 100 / std::max(name1.length(), name2.length());
			if (score2 > score)
				score = (score + 1.5*score2) / 2.5;
		}

		if (name1[0] != name2[0])
			score = std::max(score - 10, 0);
//		else if (name1.length()>1 && name2.length()>1 && name1[1] != name2[1] && !((name1[1] == 'P' && name2[1] == 'B') || (name1[1] == 'B' && name2[1] == 'P') || (name1[1] == 'D' && name2[1] == 'T') || (name1[1] == 'T' && name2[1] == 'D') || (name1[1] == 'C' && name2[1] == 'J') || (name1[1] == 'J' && name2[1] == 'C')))
//			score = std::max(score - 3, 0);
		else if (name1.length() > 5 && name2.length() > 5 && name1[2] != name2[2] && !((name1[2] == 'P' && name2[2] == 'B') || (name1[2] == 'B' && name2[2] == 'P') || (name1[2] == 'D' && name2[2] == 'T') || (name1[2] == 'T' && name2[2] == 'D') || (name1[2] == 'C' && name2[2] == 'J') || (name1[2] == 'J' && name2[2] == 'C')))
			score = std::max(score - 3, 0);

		return score;
	}
}

int MatchingAlgorithms::CompanyNameMatch(const string match_level, const string name1, const string name2, int standardizationLevel)
{
	CompanyMatchResult companyMatchResult = CompanyNameMatchAnalyze(match_level,name1,name2,standardizationLevel);
	return companyMatchResult.score;
}


MatchingAlgorithms::CompanyMatchResult MatchingAlgorithms::CompanyNameMatchAnalyze(const string match_level, const string name1, const string name2, int standardizationLevel)
{
	CompanyMatchResult companyMatchResult(0,"");

	int initialsMatchScore;

	if (match_level == "Loose")
		initialsMatchScore = 70;
	else if (match_level == "Typical")
		initialsMatchScore = 60;
	else
		initialsMatchScore = 50;

	if (name1.empty() || name2.empty())
		return companyMatchResult;

	Parser::CompanyNameParsed companyNameParsed1 = parser->ParseCompanyName(name1, standardizationLevel);
	Parser::CompanyNameParsed companyNameParsed2 = parser->ParseCompanyName(name2, standardizationLevel);

	if (companyNameParsed1.stdWords.size() == 0 || companyNameParsed2.stdWords.size() == 0)
		return companyMatchResult;


	if (this->companyNameMatchPatterns.empty())
		LoadScoringDefinition();

	auto it = companyNameMatchPatterns.find(match_level);

	if (it != companyNameMatchPatterns.end())
		companyMatchResult = CompanyStdWordsMatch(it->second,initialsMatchScore,
				companyNameParsed1.stdWords,companyNameParsed1.categories,
				companyNameParsed2.stdWords,companyNameParsed2.categories);
	else
		return companyMatchResult;

	if (companyNameParsed1.stdWords[0] == "TURKIYE")
		if (companyNameParsed2.stdWords[0] !="TURKIYE") {
			vector<string> categories = companyNameParsed1.categories;
			vector<string> stdWords = companyNameParsed1.stdWords;
			categories.erase(categories.begin());
			stdWords.erase(stdWords.begin());

		CompanyMatchResult	companyMatchResult2 = CompanyStdWordsMatch(it->second,initialsMatchScore,
				stdWords, categories, companyNameParsed2.stdWords,companyNameParsed2.categories);

			if (companyMatchResult2.score > companyMatchResult.score)
			{
				companyMatchResult.score = companyMatchResult2.score;
				companyMatchResult.pattern = companyMatchResult2.pattern;
			}
		}

	if (companyNameParsed2.stdWords[0] == "TURKIYE")
		if (companyNameParsed1.stdWords[0] != "TURKIYE") {
			vector<string> categories = companyNameParsed2.categories;
			vector<string> stdWords = companyNameParsed2.stdWords;
			categories.erase(categories.begin());
			stdWords.erase(stdWords.begin());

			CompanyMatchResult	companyMatchResult2  = CompanyStdWordsMatch(it->second,initialsMatchScore,
					companyNameParsed1.stdWords,
					companyNameParsed1.categories, stdWords, categories);

			if (companyMatchResult2.score > companyMatchResult.score)
			{
				companyMatchResult.score = companyMatchResult2.score;
				companyMatchResult.pattern = companyMatchResult2.pattern;
			}
		}

	if (companyNameParsed1.stdWords.size() > 1) {
		if (StringUtil::ReplaceTurkishChars(companyNameParsed1.stdWords[0] + companyNameParsed1.stdWords[1])
				== StringUtil::ReplaceTurkishChars(companyNameParsed2.stdWords[0]))
		{
			vector<string> words_t(companyNameParsed1.stdWords.size() - 1);
			words_t[0] = companyNameParsed1.stdWords[0] + companyNameParsed1.stdWords[1];
			for (int i = 2; i < companyNameParsed1.stdWords.size(); i++)
				words_t[i - 1] = companyNameParsed1.stdWords[i];
			vector<string> categories_t(companyNameParsed1.categories.size() - 1);
			categories_t[0] = "$$$$$";
			for (int i = 2; i < companyNameParsed1.categories.size(); i++)
				categories_t[i - 1] = companyNameParsed1.categories[i];

			CompanyMatchResult	companyMatchResult2 = CompanyStdWordsMatch(it->second, initialsMatchScore,
					words_t, categories_t,
					companyNameParsed2.stdWords,
					companyNameParsed2.categories);

			if (companyMatchResult2.score > companyMatchResult.score)
			{
				companyMatchResult.score = companyMatchResult2.score;
				companyMatchResult.pattern = companyMatchResult2.pattern;
			}
		}
	}

	if (companyNameParsed2.stdWords.size() > 1) {
		if (StringUtil::ReplaceTurkishChars(companyNameParsed2.stdWords[0] + companyNameParsed2.stdWords[1])
				== StringUtil::ReplaceTurkishChars(companyNameParsed1.stdWords[0])) {
			vector<string> words_t(companyNameParsed2.stdWords.size() - 1);
			words_t[0] = companyNameParsed2.stdWords[0] + companyNameParsed2.stdWords[1];
			for (int i = 2; i < companyNameParsed2.stdWords.size(); i++)
				words_t[i - 1] = companyNameParsed2.stdWords[i];
			vector<string> categories_t(companyNameParsed2.categories.size() - 1);
			categories_t[0] = "$$$$$";
			for (int i = 2; i < companyNameParsed2.categories.size(); i++)
				categories_t[i - 1] = companyNameParsed2.categories[i];

			CompanyMatchResult	companyMatchResult2 = CompanyStdWordsMatch(it->second, initialsMatchScore,
					words_t, categories_t,
					companyNameParsed1.stdWords,
					companyNameParsed1.categories);

			if (companyMatchResult2.score > companyMatchResult.score)
			{
				companyMatchResult.score = companyMatchResult2.score;
				companyMatchResult.pattern = companyMatchResult2.pattern;
			}
		}
	}

	if (companyMatchResult.score<89 && companyMatchResult.score>83)
	{
		string t1 = parser->DecideCustomerType(name1);
		string t2 = parser->DecideCustomerType(name2);

		if (t1 == "P" || t2 == "P")
			companyMatchResult.score = 83;

	}

	return companyMatchResult;
}


MatchingAlgorithms::CompanyMatchResult MatchingAlgorithms::CompanyStdWordsMatch(const map<string,CompanyNameMatchPattern>& companyNameMatchPatternMap, int initialsMatchScore,
		const vector<string>& words1, const vector<string>& categories1, const vector<string>& words2, const vector<string>& categories2)
{
	int wordsCount1 = words1.size();
	int wordsCount2 = words2.size();

	CompanyMatchResult companyMatchResult(0,"");

	if (wordsCount1 == 0 || wordsCount2 == 0)
		return companyMatchResult;

	vector<int> word_Matches1(wordsCount1,-1);
	vector<int> word_Matches2(wordsCount2,-1);
	vector<int> word_Match_Scores1(wordsCount1,0);
	vector<int> word_Match_Scores2(wordsCount2,0);

	vector<string> myWords1(wordsCount1);
	vector<string> myWords2(wordsCount2);
	vector<string> myCategories1(wordsCount1);
	vector<string> myCategories2(wordsCount2);

	std::copy(std::begin(words1), std::end(words1), std::begin(myWords1));
	std::copy(std::begin(words2), std::end(words2), std::begin(myWords2));
	std::copy(std::begin(categories1), std::end(categories1), std::begin(myCategories1));
	std::copy(std::begin(categories2), std::end(categories2), std::begin(myCategories2));


	int effective_word_count1 = 0;
	int effective_word_count2 = 0;

	for (int p = 0; p < wordsCount1; p++)
		if (myCategories1[p] != "KURUMSÝL" &&  myWords1[p].length() >= 2)
			effective_word_count1++;

	for (int p = 0; p < wordsCount2; p++)
		if (myCategories2[p] !="KURUMSÝL" &&  myWords2[p].length() >= 2)
			effective_word_count2++;

	int score = 0;

	for (int p = 0; p < wordsCount1; p++)
		if (myCategories1[p] !="KURUMSÝL")
			for (int k = 0; k < wordsCount2; k++) {
				if (myCategories2[k] != "KURUMSÝL") {
					if (p == 0 && k == 0
							&& myWords1[p].length() == 1
							&& myWords2[k].length() > 1
							&& myWords1[p].substr(0, 1) == myWords2[k].substr(0, 1))
						score = initialsMatchScore;
					else if (p == 0 && k == 0
							&& myWords1[p].length() > 1
							&& myWords2[k].length() == 1
							&& myWords1[p].substr(0, 1) == myWords2[k].substr(0, 1))
						score = initialsMatchScore;
					else if (std::min(myWords1[p].size(),myWords2[k].size()) == 0)
						score = 0;
					else {
						int t1 = std::min(myWords1[p].size(),myWords2[k].size());
						double t2 = std::floor(100 - 100 * DamerauLevenshteinDistance::Calculate(myWords1[p], myWords2[k]) / t1);
						score = std::max(static_cast<int>(t2), 0);

						if (abreviation_match(myWords1[p], myWords2[k])
								&& myWords2[k].length() >= 7 && score < 80)
							score = 80;

						if (abreviation_match(myWords2[k], myWords1[p])
								&& myWords1[p].length() >= 7 && score < 80)
							score = 80;

						if (score < 70)
							score = 0;

						if (p >= 1 && k >= 1)
							if (score < 70
									&& abreviation_match(myWords1[p], myWords2[k])
									&& myWords2[k].length() >= 3
									&& (myCategories2[k] != "SEKTÖR"
											|| myCategories1[p] == "SEKTÖR" || myCategories1[p] == "KURUM")) {
								if (myWords2[k].length() <= 6)
									score = 50;
								else
									score = 90;
							}

						if (p >= 1 && k >= 1)
							if (score < 70
									&& abreviation_match(myWords2[k], myWords1[p])
									&& myWords1[p].length() >= 3
									&& (myCategories1[p] != "SEKTÖR"
											|| myCategories2[k] == "SEKTÖR" ||  myCategories2[k] == "KURUM")) {
								if (myWords1[p].length() <= 6)
									score = 50;
								else
									score = 90;
							}

						if (score >= 25 && myWords1[p].substr(0, 1) != myWords2[k].substr(0, 1))
							score = score - 25;
					}

					if (score < 70)
						if (p > 1 && k > 1)
							if ((myWords1[p].length() == 3 || myWords1[p]
									.length() == 4)
									&& abreviation_match(myWords2[k],myWords1[p])
									&& (myCategories1[p] != "SEKTÖR"
											|| myCategories2[k] == "SEKTÖR" || myCategories2[k] == "KURUM"))
								score = 70;

					if (score < 70)
						if (p > 1 && k > 1)
							if ((myWords2[k].length() == 3 || myWords2[k].length() == 4)
									&& abreviation_match(myWords1[p],
											myWords2[k])
									&& (myCategories2[k] != "SEKTÖR"
											|| myCategories1[p] == "SEKTÖR" || myCategories1[p] == "KURUM"))
								score = 70;

					if (score >= 70 && myWords1[p].length() >= 2
							&& myWords2[k].length() >= 2)
						if (myWords1[p].substr(0, 1) != myWords2[k].substr(0, 1))
							if (myWords1[p].substr(1, 2) != myWords2[k].substr(1, 2))
								score = 0;

					if (score >= 50
							&& score > word_Match_Scores1[p]
							&& score > word_Match_Scores2[k]) {
						if (word_Matches1[p] != -1)
							word_Matches2[word_Matches1[p]] = -1;

						if (word_Matches2[k] != -1)
							word_Matches1[word_Matches2[k]] = -1;

						word_Match_Scores1[p] = score;
						word_Matches1[p] = k;
						word_Match_Scores2[k] = score;
						word_Matches2[k] = p;
					}
				}
			}

	int matching_major_word_count = 0;

	for (int p = 0; p < wordsCount1 && p <= 4; p++)
		if (word_Matches1[p] != -1)
			matching_major_word_count++;

	if (matching_major_word_count >= 2) {
		for (int p = 1; p < wordsCount1 && p <= 4; p++)
			if (myCategories1[p] == "SEKTÖR" && word_Matches1[p] == -1)
				for (int k = 1; k < wordsCount2 && k <= 4; k++)
					if (myWords2[k].length() == 2 && myCategories2[k] != "KURUMSÝL"
							&& word_Matches2[k] == -1
							&& StringUtil::StartsWith(myWords1[p], myWords2[k])) {
						word_Match_Scores1[p] = 59;
						word_Matches1[p] = k;
						word_Match_Scores2[k] = 59;
						word_Matches2[k] = p;
						break;
					}
	}

	if (matching_major_word_count >= 2) {
		for (int p = 1; p < wordsCount2 && p <= 4; p++)
			if (myCategories2[p] == "SEKTÖR" && word_Matches2[p] == -1)
				for (int k = 1; k < wordsCount1 && k <= 4; k++)
					if (myWords1[k].length() == 2 && myCategories1[k] != "KURUMSÝL"
							&& word_Matches1[k] == -1
							&& StringUtil::StartsWith(myWords2[p], myWords1[k])) {
						word_Match_Scores2[p] = 59;
						word_Matches2[p] = k;
						word_Match_Scores1[k] = 59;
						word_Matches1[k] = p;
						break;
					}
	}


	if (matching_major_word_count >= 2
			|| effective_word_count1 == 2
			|| effective_word_count2 == 2
			|| (matching_major_word_count == 1 && (effective_word_count1 == 3 || effective_word_count2 == 3)))
		for (int p = 1; p < myWords1.size(); p++) {
			if (word_Matches1[p] == -1 && myCategories1[p] != "KURUMSÝL") {
				if (word_Matches1[p - 1] != -1) {
					string word1 = myWords1[p - 1] + myWords1[p];
					string word2 = myWords2[word_Matches1[p - 1]];

					if (word_Match_Scores1[p - 1] < 95) {
						int score_t = (int) std::max(
								static_cast<int>(std::floor(100
										- 100
										* DamerauLevenshteinDistance::Calculate(word1, word2)
										/ std::min(word1.length(),
												word2.length()))), 0);

						if (score_t >= word_Match_Scores1[p - 1] && score_t>=93) {
							word_Match_Scores1[p - 1] = score_t;
							word_Match_Scores2[word_Matches1[p - 1]] = score_t;
							myWords1[p - 1] = myWords1[p - 1] + myWords1[p];
							myWords1.erase(myWords1.begin() + p);
							myCategories1.erase(myCategories1.begin() + p);
							word_Matches1.erase(word_Matches1.begin() + p);
							word_Match_Scores1.erase(word_Match_Scores1.begin() + p);
							removeCrossMatchingIndex(word_Matches2, p);
						}
					}
				} else {
					string word1 = myWords1[p - 1] + myWords1[p];

					vector<string> t = { "DUMMY", "DUMMY", word1 };

					vector<string> c = parser->CategorizeWords(t);

					string tCategory = c[2];

					if ((word1 == "SANAYÝ" || word1 == "SANAYI" || word1 == "TÝCARET" || word1== "TICARET") && p > 2)
						tCategory = "KURUMSÝL";

					bool matched = false;
					for (int k = 0; k < myWords2.size(); k++) {

						score = 0;

						if (word1.length() > 0 && myWords2[k].length() > 0)
							if (word_Matches2[k] == -1 && myCategories2[k] != "KURUMSÝL") {
								score = std::max(static_cast<int>(std::floor(100
										- 100
										* DamerauLevenshteinDistance::Calculate(word1, myWords2[k])
										/ std::min(word1.length(),
												myWords2[k].length()))), 0);

								if (score < 93)
									score = 0;

								if (p >= 2 && k >= 1)
									if (score < 70
											&& abreviation_match(word1,
													myWords2[k])
											&& myWords2[k].length() >= 3
											&& (myCategories2[k] != "SEKTÖR" || tCategory == "SEKTÖR" || tCategory == "KURUM"))
										if (myWords2[k].length() <= 6)
											score = 70;
										else
											score = 90;

								if (p >= 2 && k >= 1)
									if (score < 70
											&& abreviation_match(
													myWords2[k], word1)
											&& word1.length() >= 3
											&& (tCategory != "SEKTÖR" || myCategories2[k] == "SEKTÖR" || myCategories2[k] == "KURUM"))
										if (word1.length() <= 6)
											score = 70;
										else
											score = 90;

								if (score >= 25 && word1.substr(0, 1) != myWords2[k].substr(0, 1))
									score = score - 25;

							}

						if (score >= 70) {
							if (score==70)
								score = 60;

							matched = true;
							if (word_Matches1[p - 1] != -1)
								word_Matches2[word_Matches1[p - 1]] = -1;

							if (word_Matches2[k] != -1)
								word_Matches1[word_Matches2[k]] = -1;

							word_Match_Scores1[p - 1] = score;
							word_Matches1[p - 1] = k;
							word_Match_Scores2[k] = score;
							word_Matches2[k] = p - 1;
						}
					}

					if (matched
							|| ((tCategory =="SEKTÖR" || tCategory == "KURUM" || tCategory == "KURUMSÝL")
									&& (myWords1[p - 1] + myWords1[p]).length() >= 3)) {
						myWords1[p - 1] = myWords1[p - 1] + myWords1[p];
						myCategories1[p - 1] = tCategory;

						myWords1.erase(myWords1.begin() + p);
						myCategories1.erase(myCategories1.begin() + p);
						word_Matches1.erase(word_Matches1.begin() + p);
						word_Match_Scores1.erase(word_Match_Scores1.begin() + p);
						removeCrossMatchingIndex(word_Matches2, p);

					}
				}
			}
		}


	if (matching_major_word_count >= 2
			|| effective_word_count1 == 2
			|| effective_word_count2 == 2
			|| (matching_major_word_count == 1 && (effective_word_count1 == 3 || effective_word_count2 == 3)))
		for (int p = 1; p < myWords2.size(); p++) {
			if (word_Matches2[p] == -1 && myCategories2[p] != "KURUMSÝL") {
				if (word_Matches2[p - 1] != -1) {
					string word2 = myWords2[p - 1] + myWords2[p];
					string word1 = myWords1[word_Matches2[p - 1]];

					if (word1.length() > 0 && word2.length() > 0) {
						if (word_Match_Scores2[p - 1] < 95) {
							int score_t = std::max(
									static_cast<int>(std::floor(100
											- 100
											* DamerauLevenshteinDistance::Calculate(word1, word2)
											/ std::min(word1.length(),
													word2.length()))), 0);

							if (score_t >= word_Match_Scores2[p - 1] && score_t>=93) {
								word_Match_Scores2[p - 1] = score_t;
								word_Match_Scores1[word_Matches2[p - 1]] = score_t;
								myWords2[p - 1] = myWords2[p - 1]
										+ myWords2[p];

								myWords2.erase(myWords2.begin() + p);
								myCategories2.erase(myCategories2.begin() + p);
								word_Matches2.erase(word_Matches2.begin() + p);
								word_Match_Scores2.erase(word_Match_Scores2.begin() + p);
								removeCrossMatchingIndex(word_Matches1, p);
							}
						}
					}
				} else {
					string word2 = myWords2[p - 1] + myWords2[p];

					vector<string> t = { "DUMMY", "DUMMY", word2 };

					vector<string> c = parser->CategorizeWords(t);

					string tCategory = c[2];

					if ((word2 == "SANAYÝ" || word2 == "SANAYI" || word2 == "TÝCARET" || word2 == "TICARET") && p > 2)
						tCategory = "KURUMSÝL";

					bool matched = false;

					for (int k = 0; k < myWords1.size(); k++) {

						score = 0;

						if (word2.length() > 0 && myWords1[k].length() > 0)
							if (word_Matches1[k] == -1
									&& myCategories1[k] != "KURUMSÝL") {
								score = std::max(static_cast<int>(std::floor(100
										- 100
										* DamerauLevenshteinDistance::Calculate(word2, myWords1[k])
										/ std::min(word2.length(),
												myWords1[k].length()))), 0);

								if (score < 93)
									score = 0;

								if (p >= 2 && k >= 1)
									if (score < 70
											&& abreviation_match(word2,
													myWords1[k])
											&& myWords1[k].length() >= 3
											&& (myCategories1[k] != "SEKTÖR"
													|| tCategory == "SEKTÖR" || tCategory == "KURUM"))
										if (myWords1[k].length() <= 6)
											score = 70;
										else
											score = 90;

								if (p >= 2 && k >= 1)
									if (score < 70
											&& abreviation_match(
													myWords1[k], word2)
											&& word2.length() >= 3
											&& (tCategory != "SEKTÖR"
													|| myCategories1[k] == "SEKTÖR" || myCategories1[k] == "KURUM"))
										if (word2.length() <= 6)
											score = 70;
										else
											score = 90;

								if (score >= 25 && word2.substr(0, 1) != myWords1[k].substr(0, 1))
									score = score - 25;

							}

						if (score >= 70) {
							if (score==70)
								score = 60;

							matched = true;
							if (word_Matches2[p - 1] != -1)
								word_Matches1[word_Matches2[p - 1]] = -1;

							if (word_Matches1[k] != -1)
								word_Matches2[word_Matches1[k]] = -1;

							word_Match_Scores2[p - 1] = score;
							word_Matches2[p - 1] = k;
							word_Match_Scores1[k] = score;
							word_Matches1[k] = p - 1;
						}
					}

					if (matched
							|| ((tCategory == "SEKTÖR"|| tCategory == "KURUM" || tCategory == "KURUMSÝL")
									&& (myWords2[p - 1] + myWords2[p]).length() >= 3)) {
						myWords2[p - 1] = myWords2[p - 1] + myWords2[p];
						myCategories2[p - 1] = tCategory;

						myWords2.erase(myWords2.begin() + p);
						myCategories2.erase(myCategories2.begin() + p);
						word_Matches2.erase(word_Matches2.begin() + p);
						word_Match_Scores2.erase(word_Match_Scores2.begin() + p);
						removeCrossMatchingIndex(word_Matches1, p);
					}

				}
			}
		}

	for (int i = 0; i < myWords1.size() && i < 10; i++)
		if (word_Matches1[i]==-1 && myCategories1[i] == "KURUMSÝL")
			if (myWords1[i] == "TÝCARET" || myWords1[i] == "TICARET" || myWords1[i] == "TÝC" || myWords1[i] == "TIC")
			{
				for (int p = 0; p < myWords2.size() && p < 10; p++)
					if (word_Matches2[p]==-1 && myCategories2[p] == "KURUMSÝL")
						if (myWords2[p] == "TÝCARET" || myWords2[p] == "TICARET" || myWords2[p] == "TÝC" || myWords2[p] == "TIC")
						{
							word_Matches1[i]=p;
							word_Matches2[p]=i;
						}
			}
			else if (myWords1[i] == "SANAYÝ" || myWords1[i] == "SANAYI" || myWords1[i] == "SAN")
			{
				for (int p = 0; p < myWords2.size() && p < 10; p++)
					if (word_Matches2[p]==-1 && myCategories2[p] == "KURUMSÝL")
						if (myWords2[p] == "SANAYÝ" || myWords2[p] == "SANAYI" || myWords2[p] == "SAN")
						{
							word_Matches1[i]=p;
							word_Matches2[p]=i;
						}
			}

	bool sanTicMissMatch = true;
	int wTic=-1;
	int wSan=-1;
	int mTic=-1;
	int mSan=-1;

	for (int i = 0; i < myWords1.size() && i < 7; i++)
		if (myCategories1[i] == "KURUMSÝL")
				if (myWords1[i] == "TÝCARET" || myWords1[i] == "TICARET" || myWords1[i] == "TÝC" || myWords1[i] == "TIC")
					if (word_Matches1[i]==-1)
					{
						wTic = i;
						sanTicMissMatch = true;
						break;
					}
					else
					{
						sanTicMissMatch = false;
						wTic = i;
						mTic = word_Matches1[i];
						break;
					}

	if (wTic == -1 && !sanTicMissMatch)
		for (int i = 0; i < myWords2.size() && i < 7; i++)
			if (myCategories2[i] == "KURUMSÝL")
				if (myWords2[i] == "TÝCARET" || myWords2[i] == "TICARET" || myWords2[i] == "TÝC" || myWords2[i] == "TIC")
					{
						sanTicMissMatch = true;
						break;
					}


	for (int i = 0; i < myWords1.size() && i < 7; i++)
		if (myCategories1[i] == "KURUMSÝL")
				if (myWords1[i] == "SANAYÝ" || myWords1[i] == "SANAYI" || myWords1[i] == "SAN")
					if (word_Matches1[i]==-1)
					{
						wSan = i;
						sanTicMissMatch = true;
						break;
					}
					else
					{
						sanTicMissMatch = false;
						wSan = i;
						mSan = word_Matches1[i];
						break;
					}

	if (wSan == -1 && !sanTicMissMatch)
		for (int i = 0; i < myWords2.size() && i < 7; i++)
			if (myCategories2[i] == "KURUMSÝL")
					if (myWords2[i] == "SANAYÝ" || myWords2[i] == "SANAYI" || myWords2[i] == "SAN")
					{
						sanTicMissMatch = true;
						break;
					}

	if (!sanTicMissMatch)
		if (wSan<wTic && mSan>mTic)
			sanTicMissMatch = true;
		else if (wSan>wTic && mSan<mTic)
			sanTicMissMatch = true;


	bool anonim1=false;
	for (int i = 0; i < myWords1.size() && i < 7; i++)
		if (myCategories1[i] == "KURUMSÝL")
			if (myWords1[i] == "AS")
			{
				anonim1=true;
				break;
			}

	bool anonim2=false;
	for (int i = 0; i < myWords2.size() && i < 7; i++)
		if (myCategories2[i] == "KURUMSÝL")
			if (myWords2[i] == "AS")
			{
				anonim2=true;
				break;
			}

	if (anonim1 && anonim2)
		sanTicMissMatch = false;

	char word_Freq1[10];
	char word_Freq2[10];

	char name_Freq1[3];
	char name_Freq2[3];


	for (int i = 0; i < myWords1.size() && i < 10; i++)
		word_Freq1[i] = dictionary->GetCompanyNameFrequency(myWords1[i]);

	for (int i = 0; i < myWords1.size() && i < 3; i++)
		name_Freq1[i] = dictionary->GetCompanyNameFrequency(myWords1[i]);

	for (int i = 0; i < myWords2.size() && i < 10; i++)
		word_Freq2[i] = dictionary->GetCompanyNameFrequency(myWords2[i]);

	for (int i = 0; i < myWords2.size() && i < 3; i++)
		name_Freq2[i] = dictionary->GetCompanyNameFrequency(myWords2[i]);


	int non_matching_in_first[] ={0,0,0,0,0};

	int l1 = 0;
	int l2 = 0;

	for (int v=0;v<myCategories1.size() ;v++)
		if (myCategories1[v] != "KURUMSÝL")
			l1++;

	for (int v=0;v<myCategories2.size();v++)
		if (myCategories2[v] != "KURUMSÝL")
			l2++;

	vector<string>* myCategories;
	vector<string>* myCategories_Other;
	vector<string>* myWords;
	vector<string>* myOtherWords;
	vector<int>* word_Matches;
	vector<int>* word_Matches_Other;
	vector<int>* word_Match_Scores;
	char* nameFreq;
	char* wordFreq;
	char* nameFreq_Other;
	char* wordFreq_Other;


	if (l2<l1)
	{
		myCategories = &myCategories1;
		myCategories_Other = &myCategories2;
		myWords = &myWords1;
		myOtherWords = &myWords2;
		word_Matches = &word_Matches1;
		word_Matches_Other = &word_Matches2;
		word_Match_Scores = &word_Match_Scores1;
		nameFreq = name_Freq1;
		wordFreq = word_Freq1;
		nameFreq_Other = name_Freq2;
		wordFreq_Other = word_Freq2;
	}
	else
	{
		myCategories = &myCategories2;
		myCategories_Other = &myCategories1;
		myWords = &myWords2;
		myOtherWords = &myWords1;
		word_Matches = &word_Matches2;
		word_Matches_Other = &word_Matches1;
		word_Match_Scores = &word_Match_Scores2;
		nameFreq = name_Freq2;
		wordFreq = word_Freq2;
		nameFreq_Other = name_Freq1;
		wordFreq_Other = word_Freq1;
	}


	bool mandatoryWordCheck=true;

	for (int q=2;q<myWords1.size();q++)
		if (word_Matches1[q] == -1 && dictionary->GetMandatoryMatchingWords().find(myWords1[q]) != dictionary->GetMandatoryMatchingWords().end())
		{
			mandatoryWordCheck = false;
			break;
		}

	if (mandatoryWordCheck)
		for (int q=2;q<myWords2.size();q++)
			if (word_Matches2[q] == -1  && dictionary->GetMandatoryMatchingWords().find(myWords2[q]) != dictionary->GetMandatoryMatchingWords().end())
			{
				mandatoryWordCheck = false;
				break;
			}


	int i1=0;
	int i2=0;
	int a=0;
	string match_pattern="";


	while (i2<myCategories_Other->size() && a<5) {
		while ((*myCategories_Other)[i2] == "KURUMSÝL")
		{
			i2++;
			if (i2>=myCategories_Other->size())
				break;
		}

		if (i1<myCategories->size())
			while ((*myCategories)[i1] == "KURUMSÝL")
			{
				i1++;
				if (i1>=myCategories->size())
					break;
			}


		if (i2<myCategories_Other->size())
		{
			if ((*word_Matches_Other)[i2] != -1)
			{
				int d = 0;
				int w = 0;
				while (d<(*word_Matches_Other)[i2])
				{
					if ((*myCategories)[d] != "KURUMSÝL")
						w++;
					d++;
				}
				if (w<=4) {
					char c = 'A' + w;
					match_pattern = match_pattern + c;
				}
				else
					match_pattern = match_pattern + "N";
			}
			else
				match_pattern = match_pattern + "N";

			i1++;
			i2++;
			a++;
		}
	}

	while (i1<myCategories->size() && match_pattern.length()<5)
	{
		if ((*myCategories)[i1] != "KURUMSÝL")
			match_pattern = match_pattern + "X";
		i1++;
	}

	auto it = companyNameMatchPatternMap.find(match_pattern);

	if (it != companyNameMatchPatternMap.end())
	{
		MatchingAlgorithms::CompanyNameMatchPattern matchPattern = it->second;

		companyMatchResult.pattern = match_pattern;

		if (matchPattern.ignoreXXIfPossible && StringUtil::EndsWith(match_pattern,"xx"))
		{
			string lastCategory="KURUM";
			string secondLastCategory="KURUM";

			for (int w=0;w<myCategories->size();w++)
				if ((*myCategories)[w] != "KURUMSÝL")
				{
					secondLastCategory = lastCategory;
					lastCategory = (*myCategories)[w];
				}

			if (lastCategory != "KURUM" && secondLastCategory != "KURUM")
			{
				int i = match_pattern.rfind("xx");
				if (i != string::npos)
				{
					auto it2 = companyNameMatchPatternMap.find(match_pattern.substr(0, i));

					if (it2 != companyNameMatchPatternMap.end())
						matchPattern = it2->second;
				}
			}
		}

		vector<char> matchWordFreq(3, 'N');

		if (matchPattern.FirstWordCommonPenalty>0 || matchPattern.FirstWordNonCommonBonus>0 ||
		matchPattern.SecondWordCommonPenalty>0 || matchPattern.SecondWordNonCommonBonus>0)
		{
			int f=0;

			for (int w=0;w< myCategories->size();w++)
			{
				if ((*myCategories)[w] != "KURUMSÝL")
				{
					if ((*word_Matches)[w]!=-1 && w<3)
					{
						if ((*myWords)[w].length()>=3)
							matchWordFreq[f] = 'U';
						else
							matchWordFreq[f] = 'C';

						if (nameFreq[w]=='V')
							matchWordFreq[f]='V';
						else if (wordFreq[w]=='V')
							matchWordFreq[f]='V';

						if ((matchWordFreq[f]=='U' || matchWordFreq[f]=='C') && (*word_Matches)[w]<3)
						{
							if (wordFreq_Other[(*word_Matches)[w]]=='V')
								matchWordFreq[f]='V';
							else if (nameFreq_Other[(*word_Matches)[w]]=='V')
								matchWordFreq[f]='V';
						}

						if (matchWordFreq[f]=='U') {
							if (nameFreq[w]=='C')
								matchWordFreq[f]='C';
							else if (wordFreq[w]=='C')
								matchWordFreq[f]='C';
						}

						if (matchWordFreq[f]=='U' && (*word_Matches)[w]<3)
						{
							if (wordFreq_Other[(*word_Matches)[w]]=='C')
								matchWordFreq[f]='C';
							else if (nameFreq_Other[(*word_Matches)[w]]=='C')
								matchWordFreq[f]='C';
						}

						f++;
					}
				}
			}

		}

		int k=5;
		int sum=0;
		int wsum=0;
		int firstwordscore=0;
		int secondwordscore=100;
		bool secondMatchIsAbbrevation=false;

		int matchingwordcount=0;
		int patternWordIndex=0;

		for (int w=0;w<myCategories->size();w++)
		{
			if ((*myCategories)[w] != "KURUMSÝL")
			{
				if ((*word_Matches)[w] != -1)
				{
					sum = sum + (*word_Match_Scores)[w]*k;
					wsum = wsum+k;
					if (matchingwordcount==0)
						firstwordscore = (*word_Match_Scores)[w];
					else if (matchingwordcount==1) {
						if (abreviation_match((*myWords)[w], (*myOtherWords)[(*word_Matches)[w]]))
							secondMatchIsAbbrevation = true;
						else if (abreviation_match((*myOtherWords)[(*word_Matches)[w]],(*myWords)[w]))
							secondMatchIsAbbrevation = true;
						secondwordscore = (*word_Match_Scores)[w];
					}

					matchingwordcount++;
				}
				else if (std::find(matchPattern.nonMatchingOptions.begin(), matchPattern.nonMatchingOptions.end(), EnumScoreNonMatchingOptions::ALL) != matchPattern.nonMatchingOptions.end())
					wsum = wsum+k;
				else if (std::find(matchPattern.nonMatchingOptions.begin(), matchPattern.nonMatchingOptions.end(), EnumScoreNonMatchingOptions::FIRST_THREE) != matchPattern.nonMatchingOptions.end() && w<=2)
					wsum = wsum+k;
				else if (std::find(matchPattern.nonMatchingOptions.begin(), matchPattern.nonMatchingOptions.end(), EnumScoreNonMatchingOptions::LONGER_THAN_TWO) != matchPattern.nonMatchingOptions.end() && (*myWords)[w].length()>=3)
					wsum = wsum+k;
				k--;
				if (k==0)
					break;

				patternWordIndex++;

				if (patternWordIndex>=matchPattern.pattern.length())
					break;

				if (matchPattern.pattern[patternWordIndex]=='X')
					break;
			}
		}

		int scr=sum/wsum;
		bool positiveAdjustment=false;
		bool negativeAdjustment=false;

		if (!matchPattern.scoreAdjustmentList.empty())
			for (ScoreAdjustmentRange r:matchPattern.scoreAdjustmentList)
				if (scr>=r.minScore && scr<=r.maxScore) {
					if (scr<r.adjustmentScore)
						positiveAdjustment = true;
					else if (scr>r.adjustmentScore)
						negativeAdjustment = true;
					scr = r.adjustmentScore;
					break;
				}

		if (sanTicMissMatch && matchPattern.SanTicMismatchPenalty>0 && (!negativeAdjustment || matchPattern.enableMultiNegativeAdjustment))
		{
			scr = scr - matchPattern.SanTicMismatchPenalty;
			negativeAdjustment = true;
		}

		if (!positiveAdjustment || matchPattern.enableMultiPositiveAdjustment)
		{
			if (matchPattern.FirstWordNonCommonBonus>0 && matchWordFreq[0]=='U')
			{
				scr = scr + matchPattern.FirstWordNonCommonBonus;
				positiveAdjustment = true;
			}
		}

		if (!positiveAdjustment || matchPattern.enableMultiPositiveAdjustment)
		{
			if (matchPattern.SecondWordNonCommonBonus>0 && matchWordFreq[1]=='U')
			{
				scr = scr + matchPattern.SecondWordNonCommonBonus;
				positiveAdjustment = true;
			}
		}

		if (!negativeAdjustment || matchPattern.enableMultiNegativeAdjustment)
		{
			if (matchPattern.FirstWordCommonPenalty>0 && matchWordFreq[0]=='V')
			{
				scr = scr - matchPattern.FirstWordCommonPenalty;
				negativeAdjustment = true;
			}
		}

		if (!negativeAdjustment || matchPattern.enableMultiNegativeAdjustment)
		{

			if (matchPattern.SecondWordCommonPenalty>0 && matchWordFreq[1]=='V')
			{
				scr = scr - matchPattern.SecondWordCommonPenalty;
				negativeAdjustment = true;
			}
		}

		if (scr>100)
			scr=100;
		else if (scr<0)
			scr=0;

		if (matchPattern.MandatoryMatchWordThreshold<scr)
			if (!mandatoryWordCheck)
				scr = matchPattern.MandatoryMatchWordThreshold;

		if (!matchPattern.firstWordScoreLimitList.empty())
			for (FirstWordScoreLimit f:matchPattern.firstWordScoreLimitList)
				if (firstwordscore<f.limitScore && scr>f.adjustmentScore)
					scr = f.adjustmentScore;

		if (secondwordscore<100 && !secondMatchIsAbbrevation)
			if (!matchPattern.secondWordScoreLimitList.empty())
				for (FirstWordScoreLimit f:matchPattern.secondWordScoreLimitList)
					if (secondwordscore<f.limitScore && scr>f.adjustmentScore)
						scr = f.adjustmentScore;

		companyMatchResult.score = scr;

		return companyMatchResult;
	}

	companyMatchResult.score = 0;

	return companyMatchResult;

}