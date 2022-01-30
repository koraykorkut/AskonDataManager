/*
 * parser.h
 *
 *  Created on: 10 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_PARSER_H_
#define ALGORITHM_PARSER_H_

#include <string>
#include <vector>
#include "dictionary.h"

class Parser {
public:
	struct CompanyNameParsed {
		string fullName="";
		vector<string> words;
		vector<string> categories;
		string majorName="";
		string alt_MajorName="";
		vector<string> stdWords;
		vector<string> sectorWords;
	};
	Parser();
	~Parser();
	vector<string> Parse(const string& text, bool stdComplex);
	CompanyNameParsed ParseCompanyName(const string& companyName,int standardizationLevel);
	string CompanyMajorName(const string& name);
	vector<string> CategorizeWords(const vector<string>& words);
	string StandardizeCompanyName(const string& text);
	string DecidePersonNameFrequency(const string& name);
	string DecideCustomerType(const string& name);
	bool IsNumber(const string& str);
private:
	Dictionary *dictionary;
	string Standardize(const string& text, const vector<StandardizationRule> &ruleList);
};

#endif /* ALGORITHM_PARSER_H_ */
