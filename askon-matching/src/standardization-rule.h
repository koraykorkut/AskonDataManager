/*
 * standardization-rule.h
 *
 *  Created on: 7 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_STANDARDIZATION_RULE_H_
#define ALGORITHM_STANDARDIZATION_RULE_H_

#include <string>

using namespace std;


class StandardizationRule {
public:
	StandardizationRule();
	StandardizationRule(string textToFind,string replaceWith,bool matchTurkishCharacters,bool beforeDelimited,bool afterDelimited);
	bool operator== (const StandardizationRule& r2);
	bool IsAfterDelimited() const;
	void SetAfterDelimited(const bool afterDelimited);
	bool IsBeforeDelimited() const;
	void SetBeforeDelimited(const bool beforeDelimited);
	const string& GetKey() const;
	void SetKey(const string &key);
	bool IsMatchTurkishCharacters() const;
	void SetMatchTurkishCharacters(const bool matchTurkishCharacters);
	const string& GetReplaceWith() const;
	void SetReplaceWith(const string &replaceWith);
	int GetSeq() const;
	void SetSeq(int seq);
	const string& GetTextToFind() const;
	void SetTextToFind(const string &textToFind);
private:
	int seq;
	string textToFind;
	string replaceWith;
	bool matchTurkishCharacters;
	bool beforeDelimited;
	bool afterDelimited;
	string key="";
};

#endif /* ALGORITHM_STANDARDIZATION_RULE_H_ */
