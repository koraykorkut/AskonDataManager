/*
 * key-generator.cpp
 *
 *  Created on: 13 Kas 2021
 *      Author: koray.korkut
 */

#include "key-generator.h"
#include "parser.h"
#include "phrase.h"
#include "matching-algorithms.h"
#include "dictionary-txt.h"
#include <string-util.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <set>

KeyGenerator::KeyGenerator() {
	// TODO Auto-generated constructor stub
	dictionary = new DictionaryTxt();
	dictionary->Init();
	parser = new Parser();
	GeneratePersonNameFrequencyData();
	GenerateSurnameFrequencyData();
}


KeyGenerator::~KeyGenerator()
{
	delete dictionary;
	delete parser;
}

string KeyGenerator::WordKey1(const string& word)
{
	string v_Str = StringUtil::ToUpper(StringUtil::ReplaceTurkishChars(StringUtil::Trim(word)));
	int l = v_Str.length();
	if (l==5 || l==4)
		return(v_Str.substr(0,4))+"$";
	else if (l>5)
		return(v_Str.substr(0,5))+"*";
	else if (l==3)
		return v_Str+"$$";
	else if (l==2)
		return v_Str+"$$$";
	else if (l==1)
		return v_Str+"$$$$";
	else
		return "$$$$$";
}

string KeyGenerator::WordKey2(const string& word)
{
	string v_Str = StringUtil::ToUpper(StringUtil::ReplaceTurkishChars(StringUtil::Trim(word)));
	int l = v_Str.length();
	if (l>5)
		return(v_Str.substr(0,1))+"*"+v_Str.substr(l-4);
	else if (l==5)
		return(v_Str.substr(0,1))+"$"+v_Str.substr(2);
	else
		return "$$$$$";
}

void AddKey1(set<string>& set, const string& key)
{
	if (!StringUtil::StartsWith(key,"$$$$$") && !StringUtil::StartsWith(key.substr(5),"$$$$$") && !StringUtil::StartsWith(key.substr(6),"$$$$$"))
		set.insert(key);
}

void AddKey2(set<string>& set, const string& key)
{
	if (!StringUtil::StartsWith(key,"$$$$$") && !StringUtil::StartsWith(key.substr(5),"$$$$$") && !StringUtil::StartsWith(key.substr(6),"$$$$$") && !StringUtil::EndsWith(key,"$$$$$"))
	{
		set.insert(key);
		set.insert(key.substr(0,10)+"*****");
	}
}

void AddSearchKey1(set<string>& set, const string& key)
{
	if ((!StringUtil::StartsWith(key,"$$$$$") && !StringUtil::StartsWith(key.substr(5),"$$$$$")) && StringUtil::EndsWith(key,"$$$$$"))
	{
		set.insert(key);
		set.insert(key.substr(0,10)+"*****");
	}
}

void AddSearchKey2(set<string>& set,const string& key)
{
	if (!StringUtil::StartsWith(key,"$$$$$") && !StringUtil::StartsWith(key.substr(5),"$$$$$"))
		set.insert(key);
}

void AddSearchKey3(set<string>& set,const string& key)
{
	if (!StringUtil::Contains(key, "$$$$$"))
		set.insert(key);
}

vector<set<string>> KeyGenerator::GenerateCompanyNameKeys(const string& name)
{
	vector<set<string>> keys;

	if (name=="")
		return keys;

	set<string> keys1;
	set<string> keys2;

	bool test2=true;

	Parser::CompanyNameParsed companyNameParsed = parser->ParseCompanyName(name,2);
	Parser::CompanyNameParsed companyNameParsed2 = parser->ParseCompanyName(name,1);

	for (int a=1;a<=2;a++)
	{
		if (a==2)
		{
			test2=false;

			if(companyNameParsed2.stdWords.size() != companyNameParsed.stdWords.size())
				test2=true;

			if (!test2)
				for (int b=0;b<companyNameParsed.stdWords.size();b++)
					if (companyNameParsed.stdWords[b] == companyNameParsed2.stdWords[b])
						test2=true;

			if (test2)
				companyNameParsed = companyNameParsed2;
		}

		if (!test2)
			break;

		int keyWordCount = 0;

		for (int i=0;i<companyNameParsed.stdWords.size();i++)
			if (i==0 || companyNameParsed.categories[i] !="KURUMSÝL")
				keyWordCount++;

		if (keyWordCount==0)
			return keys;

		vector<vector<string>> wordKeys(companyNameParsed.stdWords.size(), vector<string>(2));
		for (int i=0;i<companyNameParsed.stdWords.size();i++)
		{
			wordKeys[i][0]=WordKey1(companyNameParsed.stdWords[i]);
			wordKeys[i][1]=WordKey2(companyNameParsed.stdWords[i]);
		}


		int q=0;

		if (keyWordCount>1)
			for (int i=0;q<1 && i<companyNameParsed.stdWords.size();i++)
			{
				if (companyNameParsed.categories[i] != "KURUMSÝL")
				{
					q++;
					int p=0;
					for(int k=i+1;k<keyWordCount && p<2;k++)
					{
						if (companyNameParsed.categories[k] != "KURUMSÝL")
							{
								set<string> keys_t;
								if (k-i<=2)
									keys_t=keys1;
								else
									keys_t=keys2;
								keys_t.insert(wordKeys[i][0]+wordKeys[k][0]);
								if (!StringUtil::StartsWith(wordKeys[k][1],"$"))
									keys_t.insert(wordKeys[i][0]+wordKeys[k][1]);

								if (!StringUtil::StartsWith(wordKeys[i][1],"$"))
								{
									keys_t.insert(wordKeys[i][1]+wordKeys[k][0]);
									if (!StringUtil::StartsWith(wordKeys[k][1],"$"))
										keys_t.insert(wordKeys[i][1]+wordKeys[k][1]);
								}
								p++;
							}
					}
				}
			}
		else if (keyWordCount==1)
		{
			keys1.insert(wordKeys[0][0]+"$$$$$");
			if (!StringUtil::StartsWith(wordKeys[0][1],"$"))
				keys1.insert(wordKeys[0][1]+"$$$$$");
		}
		else if (keyWordCount==0)
			return keys;


		if (keyWordCount>1)
		{
			keys1.insert(wordKeys[0][0]+"*****");
			if (!StringUtil::StartsWith(wordKeys[0][1],"$"))
				keys1.insert(wordKeys[0][1]+"*****");

			if (companyNameParsed.stdWords[0].length()>2)
			{
				int i=1;
				int p=0;
				while (p<2 && i<companyNameParsed.stdWords.size())
				{
					if (companyNameParsed.categories[i] != "KURUMSÝL")
					{
						keys1.insert(companyNameParsed.stdWords[0].substr(0,std::min(5, static_cast<int>(companyNameParsed.stdWords[0].length())))+
									companyNameParsed.stdWords[i].substr(0,std::min(2, static_cast<int>(companyNameParsed.stdWords[i].length()))));
						p++;
					}
					i++;
				}

			}

			if (keyWordCount>2)
				keys1.insert((companyNameParsed.stdWords[0]+companyNameParsed.stdWords[1]).substr(0,std::min(5, static_cast<int>((companyNameParsed.stdWords[0]+companyNameParsed.stdWords[1]).length())))+
						companyNameParsed.stdWords[2].substr(0,std::min(2, static_cast<int>(companyNameParsed.stdWords[2].length()))));
			else
				keys1.insert(WordKey1(companyNameParsed.stdWords[0]+companyNameParsed.stdWords[1])+"$$$$$");

		}
	}

	if (companyNameParsed.stdWords[0] == "TURKIYE" && companyNameParsed.stdWords.size() > 1)
	{
		string tempName="";

		for (int i=1;i < companyNameParsed.stdWords.size();i++)
			tempName = tempName + " " + companyNameParsed.stdWords[i];

		vector<set<string>> keys_t =  GenerateCompanyNameSearchKeys(tempName);

		for (string key: keys_t[0])
			keys1.insert(key);

		for (string key: keys_t[1])
			keys2.insert(key);
	}

	keys.push_back(keys1);
	keys.push_back(keys2);

	return keys;
}


vector<set<string>> KeyGenerator::GenerateCompanyNameSearchKeys(const string& name)
{
	vector<set<string>> keys;

	if (name=="")
		return keys;

	set<string> keys1;
	set<string> keys2;

	bool test2=true;

	Parser::CompanyNameParsed companyNameParsed = parser->ParseCompanyName(name,2);
	Parser::CompanyNameParsed companyNameParsed2 = parser->ParseCompanyName(name,1);

	for (int a=1;a<=2;a++)
	{
		if (a==2)
		{
			test2=false;

			if(companyNameParsed2.stdWords.size() !=companyNameParsed.stdWords.size())
				test2=true;

			if (!test2)
				for (int b=0;b<companyNameParsed.stdWords.size();b++)
					if (companyNameParsed.stdWords[b] == companyNameParsed2.stdWords[b])
						test2=true;

			if (test2)
				companyNameParsed = companyNameParsed2;
		}

		if (!test2)
			break;

		int keyWordCount = 0;

		for (int i=0;i<companyNameParsed.stdWords.size();i++)
			if (i==0 || companyNameParsed.categories[i] != "KURUMSÝL")
				keyWordCount++;

		if (keyWordCount==0)
			return keys;

		vector<vector<string>> wordKeys(companyNameParsed.stdWords.size(), vector<string>(2));
		for (int i=0;i<companyNameParsed.stdWords.size();i++)
		{
			wordKeys[i][0]=WordKey1(companyNameParsed.stdWords[i]);
			wordKeys[i][1]=WordKey2(companyNameParsed.stdWords[i]);
		}

		int q=0;

		if (keyWordCount>1)
			for (int i=0; q < 1 && i < companyNameParsed.stdWords.size();i++)
			{
				if (companyNameParsed.categories[i] != "KURUMSÝL")
				{
					q++;

					int p=0;
					for(int k=i+1;k<keyWordCount && p<2;k++)
					{
						if (companyNameParsed.categories[k] != "KURUMSÝL")
							{
								set<string> keys_t;
								if (k-i<=2)
									keys_t=keys1;
								else
									keys_t=keys2;
								keys_t.insert(wordKeys[i][0]+wordKeys[k][0]);
								if (!StringUtil::StartsWith(wordKeys[k][1],"$"))
									keys_t.insert(wordKeys[i][0]+wordKeys[k][1]);

								if (!StringUtil::StartsWith(wordKeys[i][1],"$"))
								{
									keys_t.insert(wordKeys[i][1]+wordKeys[k][0]);
									if (!StringUtil::StartsWith(wordKeys[k][1],"$"))
										keys_t.insert(wordKeys[i][1]+wordKeys[k][1]);
								}
								p++;
							}
					}
				}
			}
		else if (keyWordCount==1)
		{
			keys1.insert(wordKeys[0][0]+"*****");
			if (!StringUtil::StartsWith(wordKeys[0][1],"$"))
				keys1.insert(wordKeys[0][1]+"*****");
		}
		else if (keyWordCount==0)
			return keys;

		keys1.insert(wordKeys[0][0]+"$$$$$");
		if (!StringUtil::StartsWith(wordKeys[0][1],"$"))
			keys1.insert(wordKeys[0][1]+"$$$$$");

		if (keyWordCount>1)
		{
			if (companyNameParsed.stdWords[0].length()>2)
			{
				int i=1;
				int p=0;
				while (p < 2 && i < companyNameParsed.stdWords.size())
				{
					if (companyNameParsed.categories[i] != "KURUMSÝL")
					{
						keys1.insert(companyNameParsed.stdWords[0].substr(0,std::min(5, static_cast<int>(companyNameParsed.stdWords[0].length())))+
									companyNameParsed.stdWords[i].substr(0,std::min(2, static_cast<int>(companyNameParsed.stdWords[i].length()))));
						p++;
					}
					i++;
				}
			}

			if (keyWordCount>2)
				keys1.insert((companyNameParsed.stdWords[0]+companyNameParsed.stdWords[1]).substr(0,std::min(5,static_cast<int>((companyNameParsed.stdWords[0]+companyNameParsed.stdWords[1]).length())))+
						companyNameParsed.stdWords[2].substr(0,std::min(2, static_cast<int>(companyNameParsed.stdWords[2].length()))));
			else
				keys1.insert(WordKey1(companyNameParsed.stdWords[0]+companyNameParsed.stdWords[1])+"*****");

			keys1.insert(WordKey1(companyNameParsed.stdWords[0]+companyNameParsed.stdWords[1])+"$$$$$");
		}

	}

//		List<HashSet<String>> keys = new ArrayList<HashSet<String>>();

	if (companyNameParsed.stdWords[0] == "TURKIYE" && companyNameParsed.stdWords.size() > 1)
	{
		string tempName="";

		for (int i=1;i < companyNameParsed.stdWords.size(); i++)
			tempName = tempName + " " + companyNameParsed.stdWords[i];

		vector<set<string>> keys_t =  GenerateCompanyNameSearchKeys(tempName);

		for (string key: keys_t[0])
			keys1.insert(key);

		for (string key: keys_t[1])
			keys2.insert(key);
	}

	keys.push_back(keys1);
	keys.push_back(keys2);

	return keys;
}


vector<string>  KeyGenerator::GetPersonNameWordKeys(const string& word)
{
	string wordStd = MatchingAlgorithms::StandardizeWord(word);

	for (size_t i = 0;i <= 3; i++)
	{
		string prefx = wordStd.substr(0, 3 + i) + to_string(wordStd.length());
		auto it = this->personNameWordKeys.find(prefx);
		if (it != personNameWordKeys.end()) {
			return vector<string>(1,*it);
		}
	}
	
	return vector<string>(1,wordStd.substr(0, 3) + to_string(wordStd.length()));
}

vector<string>  KeyGenerator::GetSurnameWordKeys(const string& word)
{
	string wordStd = MatchingAlgorithms::StandardizeWord(word);

	for (size_t i = 0;i <= 3; i++)
	{
		string prefx = wordStd.substr(0, 3 + i) + to_string(wordStd.length());
		auto it = this->surnameWordKeys.find(prefx);
		if (it != surnameWordKeys.end()) {
			return vector<string>(1, *it);
		}
	}

	return vector<string>(1, wordStd.substr(0, 3) + to_string(wordStd.length()));
}


vector<set<string>> KeyGenerator::GeneratePersonNameKeys(const string& name)
{
	vector<set<string>> keys = vector<set<string>>(3);

	if (name == "")
		return keys;

	string v_Str = StringUtil::ToUpper(StringUtil::ReplaceTurkishChars(StringUtil::Trim(name)));
	v_Str = StringUtil::Replace(v_Str, ",", " ");
	v_Str = StringUtil::Replace(v_Str, ".", " ");
	v_Str = StringUtil::Replace(v_Str, "-", " ");
	v_Str = StringUtil::Replace(v_Str, "/", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	vector<string> tempWords = StringUtil::Split(v_Str, ' ');
	int wordCount = tempWords.size();
	int i = 0;
	string firstName = "";
	string middleName = "";
	string surname = "";
	i = wordCount;
	while (i > 0)
	{
		i--;
		if (tempWords[i].length() > 1) {
			surname = tempWords[i];
			break;
		}
	}
	int k = 0;
	while (k < i)
	{
		if (tempWords[k].length() > 1)
		{
			if (firstName == "")
				firstName = tempWords[k];
			else {
				middleName = tempWords[k];
				break;
			}
		}
		k++;
	}

	vector<string> firstNameKeys = GetPersonNameWordKeys(firstName);
	vector<string> surnameKeys = GetSurnameWordKeys(surname);

	for (string firstNameKey : firstNameKeys)
		for (string surnameKey : surnameKeys)
			keys[0].insert(surnameKey + firstNameKey);

	if (middleName.length() > 0) {
		vector<string> middleNameKeys = GetPersonNameWordKeys(middleName);
		for (string firstNameKey : middleNameKeys)
			for (string surnameKey : surnameKeys)
				keys[0].insert(surnameKey + firstNameKey);
	}

	return keys;
}

pair<string, int> SplitKey(const string& key)
{
	string textPart;
	string numericPart;

	for (int i = 0; i < key.length(); i++)
	{
		switch (key[i])
		{
			case '0':
			case '1':
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9': numericPart += key[i];break;
		default:
			textPart += key[i];
		}
	}

	int n = numericPart.length() > 0 ? atoi(numericPart.c_str()) : 0;

	return pair<string, int>(textPart, n);

}

vector<set<string>> KeyGenerator::GeneratePersonNameSearchKeys(const string& name)
{
	vector<set<string>> keys = vector<set<string>>(3);

	if (name == "")
		return keys;

	string v_Str = StringUtil::ToUpper(StringUtil::ReplaceTurkishChars(StringUtil::Trim(name)));
	v_Str = StringUtil::Replace(v_Str, ",", " ");
	v_Str = StringUtil::Replace(v_Str, ".", " ");
	v_Str = StringUtil::Replace(v_Str, "-", " ");
	v_Str = StringUtil::Replace(v_Str, "/", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	vector<string> tempWords = StringUtil::Split(v_Str, ' ');
	int wordCount = tempWords.size();
	int i = 0;
	string firstName = "";
	string middleName = "";
	string surname = "";
	i = wordCount;
	while (i > 0)
	{
		i--;
		if (tempWords[i].length() > 1) {
			surname = tempWords[i];
			break;
		}
	}
	int k = 0;
	while (k < i)
	{
		if (tempWords[k].length() > 1)
		{
			if (firstName == "")
				firstName = tempWords[k];
			else {
				middleName = tempWords[k];
				break;
			}
		}
		k++;
	}

	vector<string> firstNameKeys = GetPersonNameWordKeys(firstName);
	vector<string> surnameKeys = GetSurnameWordKeys(surname);

	for (string firstNameKey : firstNameKeys) {
		pair<string, int> firstNameKeySplit = SplitKey(firstNameKey);
		for (string surnameKey : surnameKeys) {
			keys[0].insert(surnameKey + firstNameKey);
			keys[2].insert(firstNameKey + surnameKey);
			pair<string, int> surnameKeySplit = SplitKey(surnameKey);
			if (firstNameKeySplit.second > 6) {
				keys[1].insert(surnameKey + firstNameKeySplit.first + to_string(firstNameKeySplit.second - 1));
				keys[1].insert(surnameKey + firstNameKeySplit.first + to_string(firstNameKeySplit.second + 1));
			}
			else if (firstNameKeySplit.second > 5) {
				keys[1].insert(surnameKey + firstNameKeySplit.first + to_string(firstNameKeySplit.second + 1));
			}

			if (surnameKeySplit.second > 6) {
				keys[1].insert(surnameKeySplit.first + to_string(surnameKeySplit.second - 1) + firstNameKey);
				keys[1].insert(surnameKeySplit.first + to_string(surnameKeySplit.second + 1) + firstNameKey);
			}
			else if (surnameKeySplit.second > 5) {
				keys[1].insert(surnameKeySplit.first + to_string(surnameKeySplit.second + 1) + firstNameKey);
			}
		}
	}

	if (middleName.length() > 0) {
		vector<string> middleNameKeys = GetPersonNameWordKeys(middleName);
		for (string firstNameKey : middleNameKeys)
			for (string surnameKey : surnameKeys)
				keys[0].insert(surnameKey + firstNameKey);
	}

	return keys;
}


/*
vector<set<string>> KeyGenerator::GeneratePersonNameKeys(const string& name)
{
	vector<set<string>> keys;

	if (name=="")
		return keys;

	string v_Str = StringUtil::ToUpper(StringUtil::ReplaceTurkishChars(StringUtil::Trim(name)));
	v_Str = StringUtil::Replace(v_Str, ",", " ");
	v_Str = StringUtil::Replace(v_Str, ".", " ");
	v_Str = StringUtil::Replace(v_Str, "-", " ");
	v_Str = StringUtil::Replace(v_Str, "/", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	vector<string> tempWords = StringUtil::Split(v_Str, ' ');
	int wordCount = tempWords.size();
	int i=0;
	string firstName="";
	string middleName="";
	string surName="";
	i = wordCount;
	while (i>0)
	{
		i--;
		if (tempWords[i].length()>1){
			surName=tempWords[i];
			break;
		}
	}
	int k=0;
	while (k<i)
	{
		if (tempWords[k].length()>1)
		{
			if(firstName == "")
				firstName=tempWords[k];
			else {
				middleName=tempWords[k];
				break;
			}
		}
		k++;
	}

	string firstNameKey1 = WordKey1(firstName);
	string firstNameKey2 = WordKey2(firstName);
	string middleNameKey1 = WordKey1(middleName);
	string middleNameKey2 = WordKey2(middleName);
	string surNameKey1 = WordKey1(surName);
	string surNameKey2 = WordKey2(surName);

	set<string> keys0;
	set<string> keys1;

	AddKey1(keys0,firstNameKey1+surNameKey1+middleNameKey1);
	AddKey1(keys0,firstNameKey1+surNameKey2+middleNameKey1);
	AddKey1(keys0,firstNameKey2+surNameKey1+middleNameKey1);
	AddKey1(keys0,firstNameKey2+surNameKey2+middleNameKey1);

	AddKey1(keys1,middleNameKey1+surNameKey1+firstNameKey1);
	AddKey1(keys1,middleNameKey1+surNameKey2+firstNameKey1);
	AddKey1(keys1,middleNameKey2+surNameKey1+firstNameKey1);
	AddKey1(keys1,middleNameKey2+surNameKey2+firstNameKey1);

	AddKey1(keys1,firstNameKey1+middleNameKey1+surNameKey1);
	AddKey1(keys1,firstNameKey1+middleNameKey2+surNameKey1);
	AddKey1(keys1,firstNameKey2+middleNameKey1+surNameKey1);
	AddKey1(keys1,firstNameKey2+middleNameKey2+surNameKey1);

	AddKey2(keys0,firstNameKey1+surNameKey1+middleNameKey2);
	AddKey2(keys0,firstNameKey1+surNameKey2+middleNameKey2);
	AddKey2(keys0,firstNameKey2+surNameKey1+middleNameKey2);
	AddKey2(keys0,firstNameKey2+surNameKey2+middleNameKey2);

	AddKey2(keys1,middleNameKey1+surNameKey1+firstNameKey2);
	AddKey2(keys1,middleNameKey1+surNameKey2+firstNameKey2);
	AddKey2(keys1,middleNameKey2+surNameKey1+firstNameKey2);
	AddKey2(keys1,middleNameKey2+surNameKey2+firstNameKey2);

	AddKey2(keys1,firstNameKey1+middleNameKey1+surNameKey2);
	AddKey2(keys1,firstNameKey1+middleNameKey2+surNameKey2);
	AddKey2(keys1,firstNameKey2+middleNameKey1+surNameKey2);
	AddKey2(keys1,firstNameKey2+middleNameKey2+surNameKey2);

	char nameFrequency=dictionary->GetPersonNameFrequency(firstName);
	char surnameFrequency=dictionary->GetSurnameFrequency(surName);

	if (nameFrequency=='U' && surnameFrequency=='U') {
		keys1.insert(firstName.substr(0,std::min(static_cast<int>(firstName.length()),7))+"+"+surName.substr(0,std::min(static_cast<int>(surName.length()),2)));
		keys1.insert(firstName.substr(0,std::min(static_cast<int>(firstName.length()),2))+"+"+surName.substr(0,std::min(static_cast<int>(surName.length()),7)));
	}

	keys.push_back(keys0);
	keys.push_back(keys1);

	return keys;
}
*/

/*
vector<set<string>> KeyGenerator::GeneratePersonNameSearchKeys(const string& name)
{
	vector<set<string>> keys;

	if (name=="")
		return keys;

	string v_Str = StringUtil::ToUpper(StringUtil::ReplaceTurkishChars(StringUtil::Trim(name)));
	v_Str = StringUtil::Replace(v_Str, ",", " ");
	v_Str = StringUtil::Replace(v_Str, ".", " ");
	v_Str = StringUtil::Replace(v_Str, "-", " ");
	v_Str = StringUtil::Replace(v_Str, "/", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	v_Str = StringUtil::Replace(v_Str, "  ", " ");
	vector<string> tempWords = StringUtil::Split(v_Str, ' ');
	int wordCount = tempWords.size();
	int i=0;
	string firstName="";
	string middleName="";
	string surName="";
	i = wordCount;
	while (i>0)
	{
		i--;
		if (tempWords[i].length()>1){
			surName=tempWords[i];
			break;
		}
	}
	int k=0;
	while (k<i)
	{
		if (tempWords[k].length()>1)
		{
			if(firstName =="")
				firstName=tempWords[k];
			else {
				middleName=tempWords[k];
				break;
			}
		}
		k++;
	}

	string firstNameKey1 = WordKey1(firstName);
	string firstNameKey2 = WordKey2(firstName);
	string middleNameKey1 = WordKey1(middleName);
	string middleNameKey2 = WordKey2(middleName);
	string surNameKey1 = WordKey1(surName);
	string surNameKey2 = WordKey2(surName);

	set<string> keys0;
	set<string> keys1;

	AddSearchKey1(keys0, firstNameKey1+surNameKey1+middleNameKey1);
	AddSearchKey1(keys0, firstNameKey1+surNameKey2+middleNameKey1);
	AddSearchKey1(keys0, firstNameKey2+surNameKey1+middleNameKey1);
	AddSearchKey1(keys0, firstNameKey2+surNameKey2+middleNameKey1);

	AddSearchKey2(keys0,firstNameKey1+surNameKey1+"$$$$$");
	AddSearchKey2(keys0,firstNameKey1+surNameKey1+"$$$$$");
	AddSearchKey2(keys0,firstNameKey2+surNameKey1+"$$$$$");
	AddSearchKey2(keys0,firstNameKey2+surNameKey2+"$$$$$");

	AddSearchKey2(keys1,middleNameKey1+surNameKey1+"$$$$$");
	AddSearchKey2(keys1,middleNameKey1+surNameKey1+"$$$$$");
	AddSearchKey2(keys1,middleNameKey2+surNameKey1+"$$$$$");
	AddSearchKey2(keys1,middleNameKey2+surNameKey2+"$$$$$");

	AddSearchKey2(keys1,firstNameKey1+middleNameKey1+"$$$$$");
	AddSearchKey2(keys1,firstNameKey1+middleNameKey1+"$$$$$");
	AddSearchKey2(keys1,firstNameKey2+middleNameKey1+"$$$$$");
	AddSearchKey2(keys1,firstNameKey2+middleNameKey2+"$$$$$");

	AddSearchKey3(keys1,firstNameKey1+middleNameKey1+surNameKey1);
	AddSearchKey3(keys1,firstNameKey1+middleNameKey1+surNameKey2);
	AddSearchKey3(keys1,firstNameKey1+middleNameKey2+surNameKey1);
	AddSearchKey3(keys1,firstNameKey1+middleNameKey2+surNameKey2);

	AddSearchKey3(keys1,firstNameKey2+middleNameKey1+surNameKey1);
	AddSearchKey3(keys1,firstNameKey2+middleNameKey1+surNameKey2);
	AddSearchKey3(keys1,firstNameKey2+middleNameKey2+surNameKey1);
	AddSearchKey3(keys1,firstNameKey2+middleNameKey2+surNameKey2);

	AddSearchKey3(keys1,middleNameKey1+firstNameKey1+surNameKey1);
	AddSearchKey3(keys1,middleNameKey1+firstNameKey1+surNameKey2);
	AddSearchKey3(keys1,middleNameKey1+firstNameKey2+surNameKey1);
	AddSearchKey3(keys1,middleNameKey1+firstNameKey2+surNameKey2);

	AddSearchKey3(keys1,middleNameKey2+firstNameKey1+surNameKey1);
	AddSearchKey3(keys1,middleNameKey2+firstNameKey1+surNameKey2);
	AddSearchKey3(keys1,middleNameKey2+firstNameKey2+surNameKey1);
	AddSearchKey3(keys1,middleNameKey2+firstNameKey2+surNameKey2);

	keys.push_back(keys0);
	keys.push_back(keys1);

	return keys;
}
*/

string KeyGenerator::GeneratePhoneKey(const string& phoneNumber)
{
	string phone_t = StringUtil::Replace(phoneNumber, "(", " ");
	phone_t = StringUtil::Replace(phone_t,"/", "-");
	phone_t = StringUtil::Replace(phone_t,")", " ");
	phone_t = StringUtil::Replace(phone_t,"]", " ");
	phone_t = StringUtil::Replace(phone_t,"[", " ");
	phone_t = StringUtil::Replace(phone_t,".", " ");
	phone_t = StringUtil::Replace(phone_t,",", " ");
	phone_t = StringUtil::Replace(phone_t,"&", " ");
	phone_t = StringUtil::Replace(phone_t,"+", " ");
	phone_t = StringUtil::Trim(phone_t);
	phone_t = StringUtil::Replace(phone_t,"  ", " ");
	phone_t = StringUtil::Replace(phone_t,"  ", " ");
	phone_t = StringUtil::Trim(phone_t);

	while (phone_t[0] == '0')
		phone_t = phone_t.substr(1);

	size_t pos = phone_t.find("-");

	if (pos != string::npos)
		if (pos < 7)
			phone_t = StringUtil::Replace(phone_t, "-", " ");

	size_t k = phone_t.find_last_of("-");

	if (k != string::npos)
		if (pos >= k || (k >= phone_t.length() - 5 && k < phone_t.length() - 3))
			phone_t = StringUtil::Trim(phone_t.substr(0, k));

	phone_t = StringUtil::Replace(phone_t, "-", "");
	phone_t = StringUtil::Replace(phone_t, " ", "");

	if (phone_t.length() > 7)
		return (phone_t.substr(phone_t.length() - 7));
	else
		return(phone_t);
}

void KeyGenerator::GeneratePersonNameFrequencyData()
{
	map<string, int> freqScores[4];

	for (auto const& [key, val] : dictionary->GetPersonNameFrequencyMap())
	{
		int score;
		switch (val[0])
		{
		case 'V':score = 10;break;
		case 'C':score = 10;break;
		default:score = 3;break;
		}

		for (size_t i = 0;i <= 3; i++)
		{
			string name = MatchingAlgorithms::StandardizeWord(key);
			string prefx = name.substr(0, 3 + i) + to_string(name.length());
			auto it = freqScores[i].find(prefx);
			if (it == freqScores[i].end()) {
				freqScores[i].insert(pair<string, int>(prefx, score));
			}
			else
			{
				it->second += score;
			}
		}
	}

	for (auto const& [key, val] : dictionary->GetPersonNameFrequencyMap())
	{
		string wordKey;

		for (size_t i = 0;i <= 3; i++)
		{
			string name = key;
			string stdName = MatchingAlgorithms::StandardizeWord(name);
			wordKey = stdName.substr(0, 3 + i) + to_string(stdName.length());

			auto it = freqScores[i].find(wordKey);

			if (it->second < 20 || i == 3)
				break;
		}

		personNameWordKeys.insert(wordKey);
	}
}

void KeyGenerator::GenerateSurnameFrequencyData()
{
	map<string, int> freqScores[4];

	for (auto const& [key, val] : dictionary->GetSurnameFrequencyMap())
	{
		int score;
		switch (val[0])
		{
		case 'V':score = 10;break;
		case 'C':score = 10;break;
		default:score = 3;break;
		}

		for (size_t i = 0;i <= 3; i++)
		{
			string name = MatchingAlgorithms::StandardizeWord(key);
			string prefx = name.substr(0, 3 + i) + to_string(name.length());
			auto it = freqScores[i].find(prefx);
			if (it == freqScores[i].end()) {
				freqScores[i].insert(pair<string, int>(prefx, score));
			}
			else
			{
				it->second += score;
			}
		}
	}

	for (auto const& [key, val] : dictionary->GetSurnameFrequencyMap())
	{
		string wordKey;

		for (size_t i = 0;i <= 3; i++)
		{
			string name = key;
			string stdName = MatchingAlgorithms::StandardizeWord(name);
			wordKey = stdName.substr(0, 3 + i) + to_string(stdName.length());

			auto it = freqScores[i].find(wordKey);

			if (it->second < 20 || i == 3)
				break;
		}

		surnameWordKeys.insert(wordKey);
	}
}

vector<string> KeyGenerator::GenerateCompanyNameKeys(const string& key_level, const string& name)
{
	vector<string> result;
	vector<set<string>> keys = GenerateCompanyNameKeys(name);

	for (string key : keys[0])
		result.push_back(key);

	if (key_level == "Typical" || key_level == "Loose")
	{
		for (string key : keys[1])
			result.push_back(key);
	}

	if (key_level == "Loose")
	{
		for (string key : keys[2])
			result.push_back(key);
	}

	return result;
}

vector<string> KeyGenerator::GenerateCompanyNameSearchKeys(const string& search_level, const string& name)
{
	vector<string> result;
	vector<set<string>> keys = GenerateCompanyNameSearchKeys(name);

	for (string key : keys[0])
		result.push_back(key);

	if (search_level == "Typical" || search_level == "Loose")
	{
		for (string key : keys[1])
			result.push_back(key);
	}

	if (search_level == "Loose")
	{
		for (string key : keys[2])
			result.push_back(key);
	}

	return result;
}

vector<string> KeyGenerator::GeneratePersonNameKeys(const string& key_level, const string& name)
{
	vector<string> result;
	vector<set<string>> keys = GeneratePersonNameKeys(name);

	for (string key : keys[0])
		result.push_back(key);

	if (key_level == "Typical" || key_level == "Loose")
	{
		for (string key : keys[1])
			result.push_back(key);
	}

	if (key_level == "Loose")
	{
		for (string key : keys[2])
			result.push_back(key);
	}

	return result;
}

vector<string> KeyGenerator::GeneratePersonNameSearchKeys(const string& search_level, const string& name)
{
	vector<string> result;
	vector<set<string>> keys = GeneratePersonNameSearchKeys(name);

	for (string key : keys[0])
		result.push_back(key);

	if (search_level == "Typical" || search_level == "Loose")
	{
		for (string key : keys[1])
			result.push_back(key);
	}

	if (search_level == "Loose")
	{
		for (string key : keys[2])
			result.push_back(key);
	}

	return result;
}
