/*
 * word.cpp
 *
 *  Created on: 7 Kas 2021
 *      Author: koray.korkut
 */

#include "word.h"

Word::Word(string text)
{
	this->text = text;
	this->key = text;
}

string Word::GetText()
{
	return this->text;
}

inline void Word::SetText(const string &text)
{
	this->text = text;
}

inline string Word::GetKey()
{
	return this->key;
}

inline void Word::SetKey(const string &key)
{
		this->key = key;
}



