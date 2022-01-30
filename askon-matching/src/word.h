/*
 * word.h
 *
 *  Created on: 7 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_WORD_H_
#define ALGORITHM_WORD_H_

#include <string>

using namespace std;

class Word {
public:
	Word(string text);
	string GetText();
	void SetText(const string &text);
	string GetKey();
	void SetKey(const string &key);

	bool operator== (const Word& w2)
	{
		return this->text == w2.text;
	}

    bool operator!= (const Word& w2)
	{
		return this->text != w2.text;
	}

    bool operator< (const Word& w2)
    {
		return this->text < w2.text;
    }

    bool operator> (const Word& w2)
    {
		return this->text > w2.text;
    }


    bool operator<= (const Word& w2)
	{
		return this->text <= w2.text;
	}

    bool operator>= (const Word& w2)
	{
		return this->text >= w2.text;
	}

private:
	string text;
	string key="";
};

#endif /* ALGORITHM_WORD_H_ */
