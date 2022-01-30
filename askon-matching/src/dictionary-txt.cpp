/*
 * dictionary-txt.cpp
 *
 *  Created on: 9 Kas 2021
 *      Author: koray.korkut
 */
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include "phrase.h"
#include "dictionary-txt.h"
#include <string-util.h>


DictionaryTxt::DictionaryTxt() {
	Init();
}

map<string,string> DictionaryTxt::ReadWordFrequencyMap(string filename)
{
	map<string,string> map;
	ifstream fstream(dictionaryDirectory + StringUtil::FileSeparator + filename);
	string strLine;

	while (getline(fstream, strLine))
	{
		vector<string> fields = StringUtil::Split(strLine,'|');
		if (fields.size() == 2) {
			map.insert(std::pair<string,string>(StringUtil::ReplaceTurkishChars(StringUtil::Trim(fields[0])), StringUtil::Trim(fields[1])));
		}
	}

	fstream.close();

	return map;
}


void DictionaryTxt::Init()
{
	if (initialized)
		return;

	dictionaryDirectory = std::getenv("ASKON_MATCHING_HOME") + std::string("/dictionary");

	if (!filesystem::exists(std::filesystem::path(dictionaryDirectory)))
		throw("Dizin :" +  dictionaryDirectory + "mevcut de�il.");

	Read();
	initialized = true;
}

string  DictionaryTxt::GetDescription()
{
	return "TEXT DICTIONARY:"+this->dictionaryDirectory;
}

void DictionaryTxt::ReadPersonnames()
{
	personNameFrequencyMap = ReadWordFrequencyMap("person_names.txt");
}

void DictionaryTxt::ReadSurnames()
{
	surnameFrequencyMap = ReadWordFrequencyMap("surnames.txt");
}

void DictionaryTxt::ReadCompanynames()
{
	companyNameFrequencyMap = ReadWordFrequencyMap("company_names.txt");
}

void DictionaryTxt::ReadStandardizationRules()
{
	ifstream fstream(dictionaryDirectory + StringUtil::FileSeparator + "standardization_rules.txt");
	string strLine;

	while (getline(fstream, strLine))
	{
		vector<string> fields = StringUtil::Split(strLine,'|');
		if (fields.size() >= 5)
		{
			StandardizationRule standardizationRule;
			standardizationRule.SetTextToFind(fields[0]);
			standardizationRule.SetReplaceWith(fields[1]);
			standardizationRule.SetMatchTurkishCharacters(StringUtil::Trim(fields[2]) == "TRUE");
			standardizationRule.SetBeforeDelimited(StringUtil::Trim(fields[3]) == "TRUE");
			standardizationRule.SetAfterDelimited(StringUtil::Trim(fields[4]) == "TRUE");
			standardizationRule.SetKey(StringUtil::Trim(fields[0]));
			standardizationRules.push_back(standardizationRule);
		}
	}

	fstream.close();
}

void DictionaryTxt::ReadStandardizationRulesSimple()
{
	ifstream fstream(dictionaryDirectory + StringUtil::FileSeparator + "standardization_rules_simple.txt");
	string strLine;

	while (getline(fstream, strLine))
	{
		vector<string> fields = StringUtil::Split(strLine,'|');
		if (fields.size() >= 5)
		{
			StandardizationRule standardizationRule;
			standardizationRule.SetTextToFind(fields[0]);
			standardizationRule.SetReplaceWith(fields[1]);
			standardizationRule.SetMatchTurkishCharacters(StringUtil::Trim(fields[2]) == "TRUE");
			standardizationRule.SetBeforeDelimited(StringUtil::Trim(fields[3]) == "TRUE");
			standardizationRule.SetAfterDelimited(StringUtil::Trim(fields[4]) == "TRUE");
			standardizationRule.SetKey(StringUtil::Trim(fields[0]));
			standardizationRulesSimple.push_back(standardizationRule);
		}
	}

	fstream.close();
}

void DictionaryTxt::ReadPhrases()
{
	ifstream fstream(dictionaryDirectory + StringUtil::FileSeparator + "phrases.txt");
	string strLine;

	while (getline(fstream, strLine))
	{
		vector<string> fields = StringUtil::Split(strLine,'|');
		if (fields.size() >= 4)
		{

			string baseKey = StringUtil::ReplaceTurkishChars(StringUtil::Split(fields[0],' ')[0]);
			auto  it = phraseMap.find(baseKey);

			if (it == phraseMap.end())
			{
				vector<Phrase> *vectorPtr = new vector<Phrase>;
				phraseMap.insert(std::pair<string, vector<Phrase>>(baseKey, *vectorPtr));
			}

			Phrase phrase(fields[0], fields[1],fields[2] == "TRUE", fields[3].at(0));
			phrase.SetBaseKey(baseKey);
			phraseMap[baseKey].push_back(phrase);
		}
	}

	fstream.close();
}

void DictionaryTxt::ReadScoringDefinitions()
{
	scoringDefinitions = "";
	ifstream fstream(dictionaryDirectory + StringUtil::FileSeparator + "scoring_definitions.txt");
	string strLine;

	while (getline(fstream, strLine))
	{
		scoringDefinitions += strLine + "\r\n";
	}

	fstream.close();
}

void DictionaryTxt::ReadMandatoryMatchWords()
{
	ifstream fstream(dictionaryDirectory + StringUtil::FileSeparator + "mandatory_match_words.txt");
	string strLine;

	while (getline(fstream, strLine))
	{
		mandatoryMatchWords.insert(std::pair<string, Word>(StringUtil::Trim(strLine), Word(StringUtil::Trim(strLine))));
	}

	fstream.close();
}

