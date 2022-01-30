/*
 * word-frequency-definition.h
 *
 *  Created on: 7 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_WORD_FREQUENCY_DEFINITION_H_
#define ALGORITHM_WORD_FREQUENCY_DEFINITION_H_

#include <string>

using namespace std;

class WordFrequencyDefinition {
	public:
		WordFrequencyDefinition();
		WordFrequencyDefinition(const string &word,const char frequency);
		int GetSeq() const;
		void SetSeq(const int seq);
		string GetWord();
		void SetWord(const string &word);
		char GetFrequency() const;
		void SetFrequency(const char frequency);
		string GetKey();
		void SetKey(const string &key);
		void SetNeighbours(const vector<vector<pair<string,char>>>& neighbours);
		vector<vector<pair<string,char>>>& GetNeighbours();
		vector<vector<string>>& GetDbKeys();
		vector<vector<string>>& GetSearchKeys();
		void SetDbKeys(const vector<vector<string>>& keys);
		void SetSearchKeys(const vector<vector<string>>& keys);

		bool operator== (const WordFrequencyDefinition& w2)
		{
			return this->word == w2.word;
		}

	private:
		int seq;
		string word;
		char frequency;
		string key="";
		vector<vector<pair<string,char>>> neighbours;
		vector<vector<string>> dbKeys;
		vector<vector<string>> searchKeys;
};

#endif /* ALGORITHM_WORD_FREQUENCY_DEFINITION_H_ */
