/*
 * dictionary.h
 *
 *  Created on: 6 Kas 2021
 *      Author: koray.korkut
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <string>
#include <map>
#include <set>
#include <list>
#include "phrase.h"
#include "word.h"
#include "standardization-rule.h"
#include "word-frequency-definition.h"

using namespace std;

class Dictionary {
//	static Logger logger = Logger.getLogger(Dictionary.class);
public:
	Dictionary();
	~Dictionary();
	virtual void Init() = 0;
	bool PersonNameContains(const string &name);
	bool SurnameContains(const string &name);
	bool CompanyNameContains(const string &name);
	char GetCompanyNameFrequency(const string &word);
	char GetCompanyNameWordFrequency(const string &word);
	char GetPersonNameFrequency(const string &word);
	char GetSurnameFrequency(const string &word);
	bool IsFrequentCompanyKey(const string &key);
	virtual string GetDescription() = 0;
	vector<WordFrequencyDefinition> GetCompanyNameFrequencyList();
	vector<WordFrequencyDefinition> GetPersonNameFrequencyList();
	vector<WordFrequencyDefinition> GetSurnameFrequencyList();
	vector<Word> GetMandatoryMatchingWordList();
	vector<string> GetFrequentCompanyKeyList();
	const vector<StandardizationRule>& GetStandardizationRuleList();
	vector<Word> GetMandatoryMatchingWordList(const string &filter);
	const map<string,Word>& GetMandatoryMatchingWords();
	const map<string,string>& GetCompanyNameFrequencyMap();
	const map<string,string>& GetPersonNameFrequencyMap();
	const map<string,string>& GetSurnameFrequencyMap();


	const vector<StandardizationRule>& GetStandardizationRuleSimpleList();
	vector<StandardizationRule> GetStandardizationRuleList(const string &filter);
	vector<StandardizationRule> GetStandardizationRuleSimpleList(const string &filter);
	vector<Phrase> GetPhraseList(const string &key);
	vector<Phrase> GetPhraseList(const string &category,const string &filter);
	string GetScoringDefinitions();
	void SetScoringDefinitions(const string &scoringDefinitions);
	string GetLabel();
	void SetLabel(const string &label);
	vector<tuple<string, char, int>> GenerateKeys(const string& name);

/*
	virtual void AddStandardizationRule(string username,StandardizationRule standardizationRule) = 0;
	virtual void DeleteStandardizationRule(string username,StandardizationRule standardizationRule) = 0;
	virtual void UpdateStandardizationRule(string username,StandardizationRule standardizationRule) = 0;
	virtual void AddPhrase(string username,Phrase phrase) = 0;
	virtual void DeletePhrase(string username,Phrase phrase) = 0;
	virtual void UpdatePhrase(string username,Phrase phrase) = 0;
	virtual void AddMandatoryMatchWord(string username,Word word) = 0;
	virtual void DeleteMandatoryMatchWord(string username,Word word) = 0;
	virtual void UpdateMandatoryMatchWord(string username,Word word) = 0;
	virtual void AddPersonName(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void DeletePersonName(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void UpdatePersonName(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void AddCompanyName(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void DeleteCompanyName(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void UpdateCompanyName(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void AddSurname(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void DeleteSurname(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void UpdateSurname(string username, WordFrequencyDefinition wordFrequencyDefinition) = 0;
	virtual void UpdateScoringDefinition(string username) = 0;
*/
protected:
	map<string,string> companyNameFrequencyMap;
	map<string,string> personNameFrequencyMap;
//	map<string, WordFrequencyDefinition> personNameFrequencyDefinitionMap;
//	map<string, vector<WordFrequencyDefinition>> personNameFrequencyDefinitionGroupMap[5];
	map<string,string> surnameFrequencyMap;
	set<string> frequentCompanyKeySet;
	map<string,vector<Phrase>> phraseMap;
	vector<StandardizationRule> standardizationRules;
	vector<StandardizationRule> standardizationRulesSimple ;
	string scoringDefinitions;
	map<string,Word> mandatoryMatchWords;
	string label;
	void Read();
//	void Write();
//	void Copy(Dictionary &d);
	virtual void ReadStandardizationRules() = 0;
	virtual void ReadStandardizationRulesSimple() = 0;
	virtual void ReadPersonnames() = 0;
	virtual void ReadSurnames() = 0;
	virtual void ReadCompanynames() = 0;
	virtual void ReadPhrases() = 0;
	virtual void ReadScoringDefinitions() = 0;
	virtual void ReadMandatoryMatchWords() = 0;
/*
	virtual void WriteStandardizationRules() = 0;
	virtual void WriteStandardizationRulesSimple() = 0;
	virtual void WritePersonnames() = 0;
	virtual void WriteSurnames() = 0;
	virtual void WriteCompanynames() = 0;
	virtual void WritePhrases() = 0;
	virtual void WriteScoringDefinitions() = 0;
	virtual void WriteMandatoryMatchWords() = 0;
*/
};
#endif /* DICTIONARY_H_ */
