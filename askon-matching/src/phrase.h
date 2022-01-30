/*
 * phrase.h
 *
 *  Created on: 6 Kas 2021
 *      Author: koray.korkut
 */

#ifndef PHRASE_H_
#define PHRASE_H_

#include <string>
#include <vector>

using namespace std;

class Phrase {
	public:
		Phrase();
		Phrase(const string text, const string category,const bool turkishCharacterTolerance,const char frequency) :
			text(text), category(category), turkishCharacterTolerance(turkishCharacterTolerance),
			frequency(frequency){};
		vector<string> GetKeyWordSequence();
		const string& GetBaseKey() const;
		void SetBaseKey(const string &baseKey);
		const string& GetCategory() const;
		void SetCategory(const string &category);
		char GetFrequency() const;
		void SetFrequency(char frequency);
		const string& GetKey() const;
		void SetKey(const string &key);
		int GetSeq() const;
		void SetSeq(int seq);
		const string& GetText() const;
		void SetText(const string &text);
		bool IsTurkishCharacterTolerance() const;
		void SetTurkishCharacterTolerance(bool turkishCharacterTolerance);
private:
		int seq;
		string text;
		string category;
		bool turkishCharacterTolerance;
		char frequency;
		string baseKey="";
		string key="";
};

#endif /* PHRASE_H_ */
