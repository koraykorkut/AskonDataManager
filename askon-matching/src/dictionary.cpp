/*
 * dictionary.cpp
 *
 *  Created on: 6 Kas 2021
 *      Author: koray.korkut
 */

#include <chrono>
#include <list>
#include <iostream>
#include "dictionary.h"
#include "phrase.h"
#include "word.h"
#include "word-frequency-definition.h"
#include <string-util.h>
#include "damerau-levenshtein-distance.h"
#include "matching-algorithms.h"


	Dictionary::Dictionary() {
	// TODO Auto-generated constructor stub
	}

	Dictionary::~Dictionary()
	{
		companyNameFrequencyMap.clear();
		personNameFrequencyMap.clear();
		surnameFrequencyMap.clear();
		frequentCompanyKeySet.clear();
		phraseMap.clear();
		standardizationRules.clear();
		standardizationRulesSimple.clear();
		scoringDefinitions.clear();
		mandatoryMatchWords.clear();
	}

	void Dictionary::Read()
	{
		ReadPersonnames();
		ReadSurnames();
		ReadCompanynames();
		ReadStandardizationRules();
		ReadStandardizationRulesSimple();
		ReadPhrases();
		ReadScoringDefinitions();
		ReadMandatoryMatchWords();
	}
/*
	void Dictionary::Write()
	{
		WritePersonnames();
		WriteSurnames();
		WriteCompanynames();
		WriteStandardizationRules();
		WriteStandardizationRulesSimple();
		WritePhrases();
		WriteScoringDefinitions();
		WriteMandatoryMatchWords();
	}
*/
/*
	void Dictionary::Copy(Dictionary &d)
	{
		d.Init();
		this->companyNameFrequencyMap = d.companyNameFrequencyMap;
		this->personNameFrequencyMap = d.personNameFrequencyMap;
		this->surnameFrequencyMap = d.surnameFrequencyMap;
		this->frequentCompanyKeySet = d.frequentCompanyKeySet;
		this->phraseMap = d.phraseMap;
		this->standardizationRules = d.standardizationRules;
		this->standardizationRulesSimple = d.standardizationRulesSimple;
		this->scoringDefinitions = d.scoringDefinitions;
		this->mandatoryMatchWords = d.mandatoryMatchWords;
		this->Write();
	}
*/
	bool Dictionary::PersonNameContains(const string &name)
	{
		if (this->personNameFrequencyMap.find(StringUtil::ReplaceTurkishChars(name)) == this->personNameFrequencyMap.end())
			return false;
		else
			return true;
	}

	bool Dictionary::SurnameContains(const string &name)
	{
		if (this->surnameFrequencyMap.find(StringUtil::ReplaceTurkishChars(name)) == this->surnameFrequencyMap.end())
			return false;
		else
			return true;
	}

	bool Dictionary::CompanyNameContains(const string &name)
	{
		if (this->companyNameFrequencyMap.find(StringUtil::ReplaceTurkishChars(name)) == this->companyNameFrequencyMap.end())
			return false;
		else
			return true;
	}

	char Dictionary::GetCompanyNameFrequency(const string &word)
	{
		string word_t = StringUtil::ReplaceTurkishChars(word);

		auto  it = companyNameFrequencyMap.find(word_t);
		if (it != companyNameFrequencyMap.end())
		        return it->second[0];

		return 'U';
	}

	char Dictionary::GetCompanyNameWordFrequency(const string &word)
	{
		string word_t = StringUtil::Trim(StringUtil::Replace(word, ".", " "));

		vector<Phrase> phraseList;

		auto  it = phraseMap.find(word_t);

		if (it != phraseMap.end())
		{
			phraseList = it->second;

			for (Phrase phrase : phraseList)
			{
				if (phrase.GetText() == word )
					return phrase.GetFrequency();
				else if (phrase.IsTurkishCharacterTolerance() && StringUtil::ReplaceTurkishChars(phrase.GetText()) == word_t)
					return phrase.GetFrequency();
			}
		}

		return 'U';
	}

	char Dictionary::GetPersonNameFrequency(const string &word)
	{
		string word_t = StringUtil::ReplaceTurkishChars(word);

		auto  it = personNameFrequencyMap.find(word_t);
		if (it != personNameFrequencyMap.end())
		        return it->second[0];

		return 'U';
	}

	char Dictionary::GetSurnameFrequency(const string &word)
	{
		string word_t = StringUtil::ReplaceTurkishChars(word);

		auto  it = surnameFrequencyMap.find(word_t);
		if (it != surnameFrequencyMap.end())
		        return it->second[0];

		return 'U';

	}

	bool Dictionary::IsFrequentCompanyKey(const string &key)
	{
		auto  it = frequentCompanyKeySet.find(key);
		if (it != frequentCompanyKeySet.end())
		        return true;

		return false;
	}

	vector<WordFrequencyDefinition> Dictionary::GetCompanyNameFrequencyList()
	{
		vector<WordFrequencyDefinition> companyNameFrequencyList;

		for (auto const& [key, val] : companyNameFrequencyMap)
			companyNameFrequencyList.push_back(WordFrequencyDefinition(key, val[0]));

		return companyNameFrequencyList;
	}


	vector<WordFrequencyDefinition> Dictionary::GetPersonNameFrequencyList()
	{
		vector<WordFrequencyDefinition> personNameFrequencyList;

		for (auto const& [key, val] : personNameFrequencyMap)
			personNameFrequencyList.push_back(WordFrequencyDefinition(key, val[0]));

		return personNameFrequencyList;
	}

	vector<WordFrequencyDefinition> Dictionary::GetSurnameFrequencyList()
	{
		vector<WordFrequencyDefinition> surnameFrequencyList;

		for (auto const& [key, val] : surnameFrequencyMap)
			surnameFrequencyList.push_back(WordFrequencyDefinition(key, val[0]));

		return surnameFrequencyList;
	}

	vector<Word> Dictionary::GetMandatoryMatchingWordList()
	{
		vector<Word> wordList;

		for (auto const& [key, val] : mandatoryMatchWords)
			wordList.push_back(val);

		return wordList;
	}

	vector<string> Dictionary::GetFrequentCompanyKeyList()
	{
		vector<string> wordList;

		for (string key : frequentCompanyKeySet)
			wordList.push_back(key);

		return wordList;
	}

	const vector<StandardizationRule>& Dictionary::GetStandardizationRuleList()
	{

		return this->standardizationRules;
	}

	vector<Word> Dictionary::GetMandatoryMatchingWordList(const string &filter)
	{
		string filter_t = StringUtil::Trim(filter);

		vector<Word> wordList;

		for (auto &[key, val] : mandatoryMatchWords)
		{
			if (filter_t == "")
			{
				wordList.push_back(Word(val.GetText()));
			}
			else if (StringUtil::StartsWith(filter_t, "%") && StringUtil::EndsWith(filter_t, "%"))
			{
				if (StringUtil::Contains(val.GetText(), StringUtil::Replace(filter_t, "%", "")))
						wordList.push_back(Word(val.GetText()));
			}
			else if (StringUtil::StartsWith(filter_t, "%"))
			{
				if (StringUtil::StartsWith(val.GetText(), StringUtil::Replace(filter_t, "%", "")))
					wordList.push_back(Word(val.GetText()));
			}
			else if (StringUtil::EndsWith(filter_t, "%"))
			{
				if (StringUtil::EndsWith(val.GetText(), StringUtil::Replace(filter_t, "%", "")))
					wordList.push_back(Word(val.GetText()));
			}
			else if(val.GetText() == filter_t)
				wordList.push_back(Word(val.GetText()));
		}

		return wordList;
	}

	const vector<StandardizationRule>& Dictionary::GetStandardizationRuleSimpleList()
	{
		return this->standardizationRulesSimple;
	}

	vector<StandardizationRule> Dictionary::GetStandardizationRuleList(const string &filter)
	{
		string filter_t = StringUtil::Trim(filter);
		vector<StandardizationRule> standardizationRulelist;

		for (StandardizationRule s : this->standardizationRules)
		{
			StandardizationRule standardizationRule(s.GetTextToFind(),s.GetReplaceWith(),s.IsMatchTurkishCharacters(),
					s.IsBeforeDelimited(),s.IsAfterDelimited());

			if (filter_t == "")
				standardizationRulelist.push_back(standardizationRule);
			else if (StringUtil::StartsWith(filter_t, "%") && StringUtil::EndsWith(filter_t, "%"))
			{
				if (StringUtil::Contains(standardizationRule.GetKey(), StringUtil::Replace(filter_t, "%", "")))
					standardizationRulelist.push_back(standardizationRule);
			}
			else if (StringUtil::StartsWith(filter_t, "%"))
			{
				if (StringUtil::StartsWith(standardizationRule.GetKey(), StringUtil::Replace(filter_t, "%", "")))
					standardizationRulelist.push_back(standardizationRule);
			}
			else if (StringUtil::EndsWith(filter_t, "%"))
			{
				if (StringUtil::EndsWith(standardizationRule.GetKey(), StringUtil::Replace(filter_t, "%", "")))
					standardizationRulelist.push_back(standardizationRule);
			}
			else if (standardizationRule.GetKey() == filter_t)
				standardizationRulelist.push_back(standardizationRule);
		}

		return standardizationRulelist;
	}

	vector<StandardizationRule> Dictionary::GetStandardizationRuleSimpleList(const string &filter)
	{
		string filter_t = StringUtil::Trim(filter);
		vector<StandardizationRule> standardizationRulelist;

		for (StandardizationRule s : this->standardizationRules)
		{
			StandardizationRule standardizationRule(s.GetTextToFind(),s.GetReplaceWith(),s.IsMatchTurkishCharacters(),
					s.IsBeforeDelimited(),s.IsAfterDelimited());

			if (filter_t == "")
				standardizationRulelist.push_back(standardizationRule);
			else if (StringUtil::StartsWith(filter_t, "%") && StringUtil::EndsWith(filter_t, "%"))
			{
				if (StringUtil::Contains(standardizationRule.GetKey(), StringUtil::Replace(filter_t, "%", "")))
					standardizationRulelist.push_back(standardizationRule);
			}
			else if (StringUtil::StartsWith(filter_t, "%"))
			{
				if (StringUtil::StartsWith(standardizationRule.GetKey(), StringUtil::Replace(filter_t, "%", "")))
					standardizationRulelist.push_back(standardizationRule);
			}
			else if (StringUtil::EndsWith(filter_t, "%"))
			{
				if (StringUtil::EndsWith(standardizationRule.GetKey(), StringUtil::Replace(filter_t, "%", "")))
					standardizationRulelist.push_back(standardizationRule);
			}
			else if (standardizationRule.GetKey() == filter_t)
				standardizationRulelist.push_back(standardizationRule);
		}

		return standardizationRulelist;
	}

	vector<Phrase> Dictionary::GetPhraseList(const string &key)
	{
		vector<Phrase> phraseList;

		auto  it = phraseMap.find(key);

		if (it != phraseMap.end())
			phraseList = it->second;

		return phraseList;
	}

	vector<Phrase> Dictionary::GetPhraseList(const string &category,const string &filter)
	{
		vector<Phrase> phraseList;

		for(auto it = phraseMap.begin(); it != phraseMap.end(); it++)
		{
			for (Phrase p: it->second)
			{
				Phrase phrase(p.GetText(),p.GetCategory(),p.IsTurkishCharacterTolerance(),p.GetFrequency());

				if (phrase.GetCategory()==category || category =="")
				{
					if (filter == "") {
						phraseList.push_back(phrase);
					}
					else if (StringUtil::StartsWith(filter, "%") && StringUtil::EndsWith(filter, "%"))
					{
						if (StringUtil::Contains(phrase.GetKey(), StringUtil::Replace(filter, "%", "")))
							phraseList.push_back(phrase);
					}
					else if (StringUtil::StartsWith(filter, "%"))
					{
						if (StringUtil::StartsWith(phrase.GetKey(), StringUtil::Replace(filter, "%", "")))
							phraseList.push_back(phrase);
					}
					else if (StringUtil::EndsWith(filter, "%"))
					{
						if (StringUtil::EndsWith(phrase.GetKey(), StringUtil::Replace(filter, "%", "")))
							phraseList.push_back(phrase);
					}
					else if(phrase.GetKey() == filter)
						phraseList.push_back(phrase);
				}
			}

		}

		return phraseList;
	}

	string Dictionary::GetScoringDefinitions()
	{
		return scoringDefinitions;
	}

	void Dictionary::SetScoringDefinitions(const string &scoringDefinitions)
	{
		this->scoringDefinitions = scoringDefinitions;
	}

	string Dictionary::GetLabel()
	{
		return label;
	}

	void Dictionary::SetLabel(const string &label)
	{
		this->label = label;
	}

	const map<string,Word>& Dictionary::GetMandatoryMatchingWords()
	{
		return this->mandatoryMatchWords;
	}

	vector<tuple<string, char, int>> Dictionary::GenerateKeys(const string& name)
	{
		vector<tuple<string, char, int>> result;
		for (auto const& [key, val] : personNameFrequencyMap)
		{
			string nameTemp = name.substr(0, 5);
			int score = MatchingAlgorithms::StdWordMatch(nameTemp, key.substr(0,5), true);
			if (score >= 82) {
				cout << key << " " << val << " " << score << endl;
				result.push_back(tuple<string,char,int>(key,val[0],score));
			}
		}

		return result;
	}

	const map<string, string>& Dictionary::GetCompanyNameFrequencyMap()
	{
		return this->companyNameFrequencyMap;
	}

	const map<string, string>& Dictionary::GetPersonNameFrequencyMap()
	{
		return this->personNameFrequencyMap;
	}

	const map<string, string>& Dictionary::GetSurnameFrequencyMap()
	{
		return this->surnameFrequencyMap;
	}


/*
	void Dictionary::GeneratePersonNameFrequencyData()
	{
		int counter = 1;
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (auto const& [key, val] : personNameFrequencyMap)
		{
			WordFrequencyDefinition wordFrequencyDefinition;
			wordFrequencyDefinition.SetKey(key);
			wordFrequencyDefinition.SetFrequency(val[0]);
			int keyLength = key.length();
			vector<pair<string, char>> strictMatches;
			vector<pair<string, char>> typicalMatches;
			vector<pair<string, char>> looseMatches;

			for (auto const& [key1, val2] : personNameFrequencyMap)
			{
				if (key == key1)
					continue;

				int score = MatchingAlgorithms::StdWordMatch(key, key1, true);

				if (score >= 90)
				{
					strictMatches.push_back(pair<string, char>(key1, val2[0]));
				}
				else if (score >= 82)
				{
					typicalMatches.push_back(pair<string, char>(key1, val2[0]));
				}
				else if (score >= 80)
				{
					looseMatches.push_back(pair<string, char>(key1, val2[0]));
				}
			}

			vector<vector<pair<string, char>>> neighbours(3);
			neighbours[0] = strictMatches;
			neighbours[1] = typicalMatches;
			neighbours[2] = strictMatches;

			wordFrequencyDefinition.SetNeighbours(neighbours);

			personNameFrequencyDefinitionMap.insert(pair<string, WordFrequencyDefinition>(key, wordFrequencyDefinition));

			std::cout << counter++ << "\n";
		}

		int clusterIdSequence = 1;
		map<string, int> keyClusterMapping;
		map<int, vector<string>*> keyClusters;

		for (auto [key, val] : personNameFrequencyDefinitionMap)
		{
			int clusterId;

			if (val.GetFrequency() != 'V' && val.GetFrequency() != 'C')
				continue;

			auto it = keyClusterMapping.find(key);

			vector<string>* clusterItems;

			if (it == keyClusterMapping.end())
			{
				clusterId = clusterIdSequence++;
				clusterItems = new vector<string>;
				clusterItems->push_back(key);
				keyClusters.insert(pair<int, vector<string>*>(clusterId, clusterItems));
				keyClusterMapping.insert(pair<string, int>(key, clusterId));
			}
			else {
				clusterId = it->second;
				clusterItems = keyClusters.find(clusterId)->second;
			}

			for (pair<string, char> item : val.GetNeighbours()[0])
			{
				if (item.second != 'V' && item.second != 'C')
					continue;

				auto it2 = keyClusterMapping.find(item.first);
				if (it2 == keyClusterMapping.end())
				{
					keyClusterMapping.insert(pair<string, int>(item.first, clusterId));
					clusterItems->push_back(item.first);
				}
				else if (it2->second != clusterId)
				{
					int temp = it2->second;
					vector<string>* itemsToMerge = keyClusters.find(it2->second)->second;
					for (string s : *itemsToMerge)
					{
						keyClusterMapping.find(s)->second = clusterId;
					}
					keyClusters.erase(temp);
					clusterItems->insert(clusterItems->end(), (*itemsToMerge).begin(), (*itemsToMerge).end());
				}
			}
		}

		for (pair<int, vector<string>*> item : keyClusters)
		{
			if (item.second->size() > 1)
			{
				vector<vector<string>> v = vector<vector<string>>(3, vector<string>(1, (*(item.second))[0]));

				for (string s : *(item.second)) {
					personNameFrequencyDefinitionMap.find(s)->second.SetDbKeys(v);
				}
			}
		}

		map<string, WordFrequencyDefinition>::iterator it;
		for (it = personNameFrequencyDefinitionMap.begin(); it != personNameFrequencyDefinitionMap.end(); it++)
		{
			char itemFrequency = it->second.GetFrequency();
			vector<vector<string>>& v = it->second.GetDbKeys();
			if (v.empty())
				v = vector<vector<string>>(3);

			vector<vector<pair<string, char>>>& neighbours = it->second.GetNeighbours();

			for (int i = 0; i <= 2; i++)
				if (v[i].empty())
					v[i].push_back(it->second.GetKey());

			for (pair<string, char> item2 : neighbours[0])
			{
				if (item2.second == 'C' || item2.second == 'V')
				{
					vector<vector<string>>& dbKeys = personNameFrequencyDefinitionMap.find(item2.first)->second.GetDbKeys();
					if (!dbKeys.empty())
					{
						if (std::find(v[1].begin(), v[1].end(), dbKeys[0][0]) == v[1].end())
							v[1].push_back(dbKeys[0][0]);
					}
					else if (std::find(v[1].begin(), v[1].end(), item2.first) == v[1].end())
						v[1].push_back(item2.first);
				}
			}


		}

		for (pair <string, WordFrequencyDefinition> item2 : personNameFrequencyDefinitionMap)
		{
			cout << item2.first << " : ";
			vector<vector<string>>& dbKeys = item2.second.GetDbKeys();
			if (dbKeys[0].size() > 0)
				for (string s : dbKeys[0])
					cout << s << " ";
			cout << " - ";
			if (dbKeys[1].size() > 0)
				for (string s : dbKeys[1])
					cout << s << " ";
			cout << endl;
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

	}
	*/