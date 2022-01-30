/*
 * dictionary-txt.h
 *
 *  Created on: 9 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_DICTIONARY_TXT_H_
#define ALGORITHM_DICTIONARY_TXT_H_

#include "dictionary.h"

class DictionaryTxt : public Dictionary{
public:
	DictionaryTxt();
	string GetDescription();
protected:
	void Init();
	void ReadStandardizationRules();
	void ReadStandardizationRulesSimple();
	void ReadPersonnames();
	void ReadSurnames();
	void ReadCompanynames();
	void ReadPhrases();
	void ReadScoringDefinitions();
	void ReadMandatoryMatchWords();
	map<string,string> ReadWordFrequencyMap(string filename);
private:
	bool initialized=false;
	string dictionaryDirectory = "dictionary";
};

#endif /* ALGORITHM_DICTIONARY_TXT_H_ */
