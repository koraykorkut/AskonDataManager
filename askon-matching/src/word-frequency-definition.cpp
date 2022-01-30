/*
 * word-frequency-definition.cpp
 *
 *  Created on: 7 Kas 2021
 *      Author: koray.korkut
 */

#include <string>
#include <string-util.h>
#include "word-frequency-definition.h"

WordFrequencyDefinition::WordFrequencyDefinition() {
}

WordFrequencyDefinition::WordFrequencyDefinition(const string &word,const char frequency)
{
	this->word = word;
	this->frequency = frequency;
	this->key = word;
}


int WordFrequencyDefinition::GetSeq() const
{
	return this->seq;
}

void WordFrequencyDefinition::SetSeq(const int seq)
{
	this->seq = seq;
}

string WordFrequencyDefinition::GetWord()
{
	if (!this->word.empty())
		return StringUtil::ReplaceTurkishChars(this->word);
	else
		return this->word;
}

void WordFrequencyDefinition::SetWord(const string &word)
{
	if (!word.empty())
		this->word = StringUtil::ReplaceTurkishChars(word);
	else
		this->word = word;
}

char WordFrequencyDefinition::GetFrequency() const
{
	return this->frequency;
}

void WordFrequencyDefinition::SetFrequency(const char frequency)
{
	this->frequency = frequency;
}

string WordFrequencyDefinition::GetKey()
{
	return this->key;
}

void WordFrequencyDefinition::SetKey(const string &key)
{
	this->key = key;
}

vector<vector<pair<string,char>>>& WordFrequencyDefinition::GetNeighbours()
{
	return this->neighbours;
}

void WordFrequencyDefinition::SetNeighbours(const vector<vector<pair<string,char>>>& neighbours)
{
	this->neighbours = neighbours;
}

vector<vector<string>>& WordFrequencyDefinition::GetDbKeys()
{
	return this->dbKeys;
}

vector<vector<string>>& WordFrequencyDefinition::GetSearchKeys()
{
	return this->searchKeys;
}

void WordFrequencyDefinition::SetDbKeys(const vector<vector<string>>& keys)
{
	this->dbKeys = keys;
}

void WordFrequencyDefinition::SetSearchKeys(const vector<vector<string>>& keys)
{
	this->searchKeys = keys;
}
