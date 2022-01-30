/*
 * key-generator.h
 *
 *  Created on: 13 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_KEY_GENERATOR_H_
#define ALGORITHM_KEY_GENERATOR_H_

#include <string>
#include <vector>
#include <unordered_set>
#include "dictionary.h"
#include "parser.h"

using namespace std;

class KeyGenerator {
public:
	KeyGenerator();
	~KeyGenerator();
	string WordKey1(const string& word);
	string WordKey2(const string& word);
	vector<string> GenerateCompanyNameKeys(const string& key_level, const string& name);
	vector<string> GenerateCompanyNameSearchKeys(const string& search_level, const string& name);
	vector<string> GeneratePersonNameKeys(const string& key_level, const string& name);
	vector<string> GeneratePersonNameSearchKeys(const string& search_level, const string& name);

	vector<set<string>> GenerateCompanyNameKeys(const string& name);
	vector<set<string>> GenerateCompanyNameSearchKeys(const string& name);
	vector<set<string>> GeneratePersonNameKeys(const string& name);
	vector<set<string>> GeneratePersonNameSearchKeys(const string& name);
	string GeneratePhoneKey(const string& phoneNumber);
	vector<string> GetPersonNameWordKeys(const string& word);
	vector<string> GetSurnameWordKeys(const string& word);
private:
	Dictionary *dictionary;
	Parser *parser;
	unordered_set<string> personNameWordKeys;
	unordered_set<string> surnameWordKeys;
	void GeneratePersonNameFrequencyData();
	void GenerateSurnameFrequencyData();
};

#endif /* ALGORITHM_KEY_GENERATOR_H_ */
