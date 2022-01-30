/*
 * parser.cpp
 *
 *  Created on: 10 Kas 2021
 *      Author: koray.korkut
 */

#include "parser.h"
#include "phrase.h"
#include "dictionary-txt.h"
#include <string-util.h>
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <regex>
#include <stdlib.h>

Parser::Parser() {
	// TODO Auto-generated constructor stub
	dictionary = new DictionaryTxt();
	dictionary->Init();
}

Parser::~Parser()
{
	delete dictionary;
}


bool Parser::IsNumber(const string& str)
{
	std::string::const_iterator it = str.begin();
    while (it != str.end() && std::isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}


vector<string> Parser::Parse(const string& text, bool stdComplex)
{
	string text_t="";

	if (stdComplex)
		text_t = Standardize(text, dictionary->GetStandardizationRuleList());
	else
		text_t = Standardize(text, dictionary->GetStandardizationRuleSimpleList());

	text_t = StringUtil::Replace(text_t, ".", " ");
	text_t = StringUtil::Trim(text_t);
	text_t = StringUtil::Replace(text_t, "  ", " ");
	text_t = StringUtil::Replace(text_t, "  ", " ");
	text_t = StringUtil::Replace(text_t, "  ", " ");

	return StringUtil::Split(text_t, ' ');
}

vector<string> Parser::CategorizeWords(const vector<string>& words)
{
	int n;
	int m;
	string temp;
	string temp_t;
	string temp_i;
	vector<Phrase> phraseList;
	Phrase *phraseMatched=NULL;

	vector<string> keywords;
	bool match;

	vector<string> categories;

	for (n = 0; n < words.size(); n++)
		categories.push_back("");

	for (n = 0; n < words.size(); n++)
	{
		temp = words[n];
		temp = StringUtil::Replace(temp,"."," ");
		temp = StringUtil::Trim(temp);

		temp_t = StringUtil::ReplaceTurkishChars(temp);
		phraseList = dictionary->GetPhraseList(temp_t);

		if (!phraseList.empty())
		{
			for (Phrase phrase : phraseList) {
				match = true;
				keywords = phrase.GetKeyWordSequence();
				for (m = 0; m < keywords.size() && match==true && n+m<words.size() ; m++)
				{
					temp_i = words[n+m];
					temp_i = StringUtil::Replace(temp_i, "."," ");
					temp_i = StringUtil::Trim(temp_i);

					temp_t = StringUtil::ReplaceTurkishChars(temp_i);

					if (phrase.IsTurkishCharacterTolerance()) {
						if (temp_t != StringUtil::ReplaceTurkishChars(keywords[m]))
							match = false;
					}
					else if (temp_i != keywords[m])
						match = false;
				}
				if (match & m == keywords.size()) {
					if (phraseMatched == NULL)
						phraseMatched = new Phrase(phrase.GetText(), phrase.GetCategory(), phrase.IsTurkishCharacterTolerance(), phrase.GetFrequency());
					else if (phraseMatched->GetKeyWordSequence().size() < phrase.GetKeyWordSequence().size())
						phraseMatched = new Phrase(phrase.GetText(), phrase.GetCategory(), phrase.IsTurkishCharacterTolerance(), phrase.GetFrequency());
				}
			}

			if (phraseMatched != NULL) {
				for (m = 0; m < phraseMatched->GetKeyWordSequence().size(); m++)
					categories[n++] = phraseMatched->GetCategory();
				n--;
				delete phraseMatched;
				phraseMatched = NULL;
			}

			if ((words[n] == "SANAYÝ" || words[n] == "SANAYI" || words[n] == "TÝCARET" || words[n] == "TICARET") && n > 1)
				categories[n]="KURUMSÝL";
		}


		temp_i = words[n];
		temp_i = StringUtil::Replace(temp_i, "."," ");
		temp_i = StringUtil::Trim(temp_i);
		temp_i = StringUtil::ReplaceTurkishChars(temp_i);

		if (temp_i == "AS" && n > 0)
			categories[n]="KURUMSÝL";

		if (n==0 && (categories[n] == "SEKTÖR" || categories[n] == "KURUMSÝL"))
			categories[n]="KURUM";

		if (temp_i == "TC" && n==0)
			categories[n]="KURUMSÝL";

		if (categories[n] == "")
		{
			temp = "";
			for (m=0; m < words[n].length() && m<5; m++)
			{
				if (isdigit(words[n][m]))
					temp = temp + '#';
				else if (isalpha(words[n][m]))
					temp = temp + '$';
				//						else if (words[n].charAt(m)!='.')
				else
					temp = temp + words[n][m];
			}
			if (temp.find("$") >=0 && temp.length()>=2)
			{
				if (temp.length()>5)
					temp = "$$$$$";
				else if (temp.length()==5)
				{
					if (temp != "$####" && temp !="####$")
						temp="$$$$$";
				}
				else if (temp.length()==4)
				{
					if (temp !="$###" && temp!="###$" && temp !="$##." && temp!="##$.")
						temp="$$$$$";
				}
				else if (temp.length()==3)
				{
					if (temp !="$##" && temp!="##$" && temp!="$#." && temp !="#$.")
						temp="$$$$$";
				}
				else if (temp.length()==2)
				{
					if (temp!="$#" && temp!="#$" && temp !="$.")
						temp="$$$$$";
				}
			}

			categories[n] = temp;
		}
	}

	if (words.size() >= 3)
	{
		if (words[0] == "SS")
			categories[0]="KURUMSÝL";
	}

	return categories;
}



string Parser::Standardize(const string& text, const vector<StandardizationRule>& ruleList)
{
	if (text.empty())
		return "";

	StandardizationRule standardizationRule;
	string textToFind;
	string textReturn = text;
	string replaceWith;
	string replaceSequence="";

	textReturn = StringUtil::Replace(textReturn,"*"," ");
	textReturn = StringUtil::Replace(textReturn,","," ");
	textReturn = StringUtil::Replace(textReturn,":"," ");
	textReturn = StringUtil::Replace(textReturn,"%"," ");
	textReturn = StringUtil::Replace(textReturn,"#"," ");
	textReturn = StringUtil::Replace(textReturn,"?"," ");
	textReturn = StringUtil::Replace(textReturn,"'"," ");
	textReturn = StringUtil::Replace(textReturn,"|"," ");
	textReturn = StringUtil::Replace(textReturn,"\""," ");
	textReturn = StringUtil::Replace(textReturn,"+"," ");
	textReturn = StringUtil::Replace(textReturn,"-"," ");
	textReturn = StringUtil::Replace(textReturn,"/"," ");
	textReturn = StringUtil::Replace(textReturn,"&"," ");
	textReturn = StringUtil::Replace(textReturn,"_"," ");
	textReturn = StringUtil::Replace(textReturn,"\\","/");
	textReturn = StringUtil::Replace(textReturn,"\n"," ");
	textReturn = StringUtil::Replace(textReturn,"\t"," ");
//	text = StringUtil::Replace(text,"\u00A0"," ");
	textReturn = StringUtil::Replace(textReturn,"="," ");
	textReturn = StringUtil::Replace(textReturn,"("," ");
	textReturn = StringUtil::Replace(textReturn,")"," ");
	textReturn = StringUtil::Replace(textReturn,"{"," ");
	textReturn = StringUtil::Replace(textReturn,"}"," ");
	textReturn = StringUtil::Replace(textReturn,"  "," ");
	textReturn = StringUtil::Replace(textReturn,"  "," ");
	textReturn = StringUtil::Replace(textReturn,"  "," ");
	textReturn = StringUtil::Replace(textReturn,"  "," ");
	textReturn = StringUtil::Replace(textReturn,"  "," ");
	textReturn = StringUtil::Replace(textReturn,"  "," ");

	textReturn = " " + StringUtil::ToUpper(textReturn) + " ";


	for (StandardizationRule standardizationRule : ruleList)
	{
		textToFind = standardizationRule.GetTextToFind();

		if (standardizationRule.IsBeforeDelimited())
			textToFind = " " + textToFind;

		if (standardizationRule.IsAfterDelimited())
			textToFind = textToFind + " ";

		replaceWith = standardizationRule.GetReplaceWith();
		int n = 0;
		int m = 0;
		replaceSequence="";
		do {
			char c = textReturn[n+m];
			char c2 = textToFind[m];
			if (standardizationRule.IsBeforeDelimited() && m==0)
			{
				if (isalnum(c))
					n++;
				else
					m++;
			}
			else if (standardizationRule.IsAfterDelimited() && m==textToFind.length()-1)
			{
				if (textReturn.length()>n+m)
				{
					c = textReturn[n+m];
					if (isalnum(c))
					{
						m = 0;
						replaceSequence="";
						n++;
					}
					else
						m++;
				}
				else
					m++;
			}
			else if (c2=='#')
			{
				if (!isdigit(c))
				{
					m=0;
					replaceSequence="";
					n++;
				}
				else {
					replaceSequence=replaceSequence+c;
					m++;
				}
			}
			else if (c2=='$')
			{
				if (!isalpha(c))
				{
					m=0;
					replaceSequence="";
					n++;
				}
				else {
					replaceSequence=replaceSequence+c;
					m++;
				}
			}
			else if (standardizationRule.IsMatchTurkishCharacters())
			{
				if (!(StringUtil::ReplaceTurkishChars(c)==StringUtil::ReplaceTurkishChars(c2)))
				{
					m=0;
					replaceSequence="";
					n++;
				}
				else
					m++;
			}
			else if (c!=c2)
			{
				m=0;
				replaceSequence="";
				n++;
			} else
				m++;


			if (m==textToFind.length())
			{
				string temp="";
				string replaceWithFinal="";

				int i=0;
				for (int k=0;k<replaceWith.length();k++)
				{
					char p = replaceWith[k];
					if (p=='$' || p=='#')
						replaceWithFinal = replaceWithFinal + replaceSequence[i++];
					else
						replaceWithFinal = replaceWithFinal + p;
				}


				if (n>0)
					if (standardizationRule.IsBeforeDelimited())
						temp = textReturn.substr(0,n+1);
					else
						temp = textReturn.substr(0,n);

				temp = temp + replaceWithFinal;
				if (n+m<textReturn.length())
					if (standardizationRule.IsAfterDelimited())
						textReturn = temp + textReturn.substr(n+m-1, string::npos);
					else
						textReturn = temp + textReturn.substr(n+m, string::npos);
				else
					textReturn = temp;

				if (textReturn=="")
					return "";

				n = temp.length();
				m = 0;
				replaceSequence="";
			}
		} while (n+m<textReturn.length());
	}
	textReturn = StringUtil::Replace(textReturn, "  "," ");
	textReturn = StringUtil::Replace(textReturn, "  "," ");
	textReturn = StringUtil::Replace(textReturn, "  "," ");

	return StringUtil::Trim(textReturn);
}

string Parser::CompanyMajorName(const string& name)
{
	vector<string> words = Parse(name,true);
	bool prevConcanated=false;
	for (int i=0; i <words.size(); i++)
	{
		if (i>0)
		{
			string w_1 = StringUtil::Replace(words[i-1], ".", "");
			string w = StringUtil::Replace(words[i], ".", "");

			if ((w_1.length()==1 || prevConcanated) && w.length()==1)
			{
				vector<string> words_t;
				for (int k=0; k < i-1; k++)
					words_t.push_back(words[k]);
				words_t.push_back(w_1+w);
				for (int k=i+1;k < words.size();k++)
					words_t.push_back(words[k]);
				words = words_t;
				i--;
				prevConcanated=true;
			}
			else
				prevConcanated=false;
		}
	}

	vector<string> categories = CategorizeWords(words);
	string returnName="";
	if (words.size() > 3)
	{
		if (words[0] =="SS")
			categories[0]="KURUMSÝL";
	}
	for (int i=0;i<words.size();i++)
	{
		if ((words[i] == "SANAYÝ" || words[i] == "SANAYI" || words[i] == "TÝCARET" || words[i]=="TÝCARET") && i>1)
			categories[i]="KURUMSÝL";
		if (categories[i] !="KURUMSÝL" || (i==0 && words[0] !="SS"))
			returnName=StringUtil::Trim(returnName+" "+ words[i]);
	}
	return returnName;
}

string Parser::StandardizeCompanyName(const string& text)
{
	return Standardize(text, dictionary->GetStandardizationRuleList());
}

string Parser::DecidePersonNameFrequency(const string& name)
{
	if (name=="")
		return "C";

	string name_t = StringUtil::ToUpper(name);
	name_t = StringUtil::ReplaceTurkishChars(name_t);
	name_t = StringUtil::Replace(name_t, "."," ");
	name_t = StringUtil::Replace(name_t, "*"," ");
	name_t = StringUtil::Replace(name_t, ","," ");
	name_t = StringUtil::Replace(name_t, ":"," ");
	name_t = StringUtil::Replace(name_t, "%"," ");
	name_t = StringUtil::Replace(name_t, "#"," ");
	name_t = StringUtil::Replace(name_t, "?"," ");
	name_t = StringUtil::Replace(name_t, "'"," ");
	name_t = StringUtil::Replace(name_t, "|"," ");
	name_t = StringUtil::Replace(name_t, "\""," ");
	name_t = StringUtil::Replace(name_t, "+"," ");
	name_t = StringUtil::Replace(name_t, "-"," ");
	name_t = StringUtil::Replace(name_t, "/"," ");
	name_t = StringUtil::Replace(name_t, "&"," ");
	name_t = StringUtil::Replace(name_t, "_"," ");
	name_t = StringUtil::Replace(name_t, "\\","/");
	name_t = StringUtil::Replace(name_t, "\n"," ");
	name_t = StringUtil::Replace(name_t, "\t"," ");
//	name_t = StringUtil::Replace(name_t, "\u00A0"," ");
	name_t = StringUtil::Replace(name_t, "="," ");
	name_t = StringUtil::Replace(name_t, "("," ");
	name_t = StringUtil::Replace(name_t, ")"," ");
	name_t = StringUtil::Replace(name_t, "{"," ");
	name_t = StringUtil::Replace(name_t, "}"," ");
	name_t = StringUtil::Replace(name_t, "  "," ");
	name_t = StringUtil::Replace(name_t, "  "," ");
	name_t = StringUtil::Replace(name_t, "  "," ");

	string firstName="";
	string middleName="";
	string surName="";

	char fNameFrequency='U';
	char mNameFrequency='U';
	char sNameFrequency='U';

	vector<string> words = StringUtil::Split(StringUtil::Trim(name_t),' ');

	if (words.size() > 0)
		firstName = words[0];
	else
		return "C";

	if (words.size() > 1)
		surName = words[words.size()-1];

	if (words.size() > 2)
		middleName = words[1];

	fNameFrequency = dictionary->GetPersonNameFrequency(firstName);

	if (words.size() > 1)
		sNameFrequency = dictionary->GetSurnameFrequency(surName);

	if (words.size() > 2)
		mNameFrequency = dictionary->GetPersonNameFrequency(middleName);

	if (words.size() > 2)
		if (fNameFrequency!='V' ||  mNameFrequency!='V' || sNameFrequency!='V')
			return "U";
	    else
	        return "C";

	if (words.size() == 2)
		if (fNameFrequency=='U' || sNameFrequency=='U')
			return "U";
		else if (fNameFrequency=='V' && sNameFrequency=='V')
			return "V";
		else
			return "C";
	else
		return string(1, fNameFrequency);
}

Parser::CompanyNameParsed Parser::ParseCompanyName(const string& companyName,int standardizationLevel)
{

	CompanyNameParsed companyNameParsed;
	companyNameParsed.fullName = companyName;
	if (standardizationLevel==1)
		companyNameParsed.words = Parse(companyName,false);
	else if (standardizationLevel==2)
		companyNameParsed.words = Parse(companyName,true);
	else {
		string text = companyName;
		text = StringUtil::Replace(text, "*"," ");
		text = StringUtil::Replace(text, ","," ");
		text = StringUtil::Replace(text, ":"," ");
		text = StringUtil::Replace(text, "%"," ");
		text = StringUtil::Replace(text, "#"," ");
		text = StringUtil::Replace(text, "?"," ");
		text = StringUtil::Replace(text, "'"," ");
		text = StringUtil::Replace(text, "|"," ");
		text = StringUtil::Replace(text, "\""," ");
		text = StringUtil::Replace(text, "+"," ");
		text = StringUtil::Replace(text, "-"," ");
		text = StringUtil::Replace(text, "/"," ");
		text = StringUtil::Replace(text, "&"," ");
		text = StringUtil::Replace(text, "_"," ");
		text = StringUtil::Replace(text, "\\","/");
		text = StringUtil::Replace(text, "\n"," ");
		text = StringUtil::Replace(text, "\t"," ");
//		text = StringUtil::Replace(text, "\u00A0"," ");
		text = StringUtil::Replace(text, "="," ");
		text = StringUtil::Replace(text, "("," ");
		text = StringUtil::Replace(text, ")"," ");
		text = StringUtil::Replace(text, "{"," ");
		text = StringUtil::Replace(text, "}"," ");
		text = StringUtil::Replace(text, "  "," ");
		text = StringUtil::Replace(text, "  "," ");
		text = StringUtil::Replace(text, "  "," ");

		text = " " + StringUtil::ToUpper(text) + " ";

		text = StringUtil::Replace(text, "."," ");
		text = StringUtil::Trim(text);
		text = StringUtil::Replace(text, "  "," ");
		text = StringUtil::Replace(text, "  "," ");
		text = StringUtil::Replace(text, "  "," ");

		companyNameParsed.words = StringUtil::Split(text, ' ');
	}

	if (companyNameParsed.words.size() == 0 )
		return companyNameParsed;

	bool prevConcanated=false;
	for (int i=0;i < companyNameParsed.words.size(); i++)
	{
		if (i>0)
		{
			string w_1 = StringUtil::Replace(companyNameParsed.words[i-1], ".", "");
			string w = StringUtil::Replace(companyNameParsed.words[i], ".", "");

			if ((w_1.length()==1 || prevConcanated) && w.length()==1)
			{
				vector<string> words_t;
				for (int k=0; k < i-1; k++)
					words_t.push_back(companyNameParsed.words[k]);
				words_t.push_back(w_1+w);
				for (int k=i+1;k<companyNameParsed.words.size();k++)
					words_t.push_back(companyNameParsed.words[k]);
				companyNameParsed.words = words_t;
				i--;
				prevConcanated=true;
			}
			else
				prevConcanated=false;
		}
	}
	vector<string> categories = CategorizeWords(companyNameParsed.words);

	bool majorNameEnd=false;
	bool start=false;

	if (companyNameParsed.words.size() > 2)
	{
		if (!StringUtil::StartsWith(companyName, "TAÞ")
				&& (StringUtil::StartsWith(companyNameParsed.words[0], "TASF") || StringUtil::StartsWith(companyNameParsed.words[0], "TAS") || StringUtil::StartsWith(companyNameParsed.words[0], "ÝFL") || StringUtil::StartsWith(companyNameParsed.words[0], "IFL")))
		{
			if (companyNameParsed.words[1] == "HAL" || StringUtil::StartsWith(companyNameParsed.words[1], "HALI") || companyNameParsed.words[1] == "HL" || companyNameParsed.words[1] == "HA" || companyNameParsed.words[1] =="H")
			{
				categories[0]="KURUMSÝL";
				categories[1]="KURUMSÝL";
			}
		}
	}

	for (int i=0;i<companyNameParsed.words.size();i++)
	{
		if (companyNameParsed.words[i] =="SAN")
		{
			if (i>=2)
				companyNameParsed.words[i] = "SANAYI";
			else if (i==1)
			{
				bool replace = true;
				for (int k=i+1;k < companyNameParsed.words.size() ;k++)
					if (categories[k] !="KURUMSÝL")
					{
						replace = false;
						break;
					}
				if (replace)
					companyNameParsed.words[i] = "SANAYI";
			}
		}
	}

	for (int i=0;i<companyNameParsed.words.size();i++)
	{
		if (companyNameParsed.words[i] == "TUR")
		{
			if (i>=2) {
				companyNameParsed.words[i] = "TURIZM";
				categories[i]="SEKTÖR";
			}
			else if (i==1)
			{
				bool replace = true;
				for (int k=i+1;k < companyNameParsed.words.size() ;k++)
					if (categories[k] != "KURUMSÝL")
					{
						replace = false;
						break;
					}
				if (replace) {
					companyNameParsed.words[i] = "TURIZM";
					categories[i]="SEKTÖR";
				}
			}
		}
	}

	for (int i=0;i < companyNameParsed.words.size();i++)
	{
		if (companyNameParsed.words[i] == "TÝCARET" || companyNameParsed.words[i] == "TICARET" ||
				companyNameParsed.words[i] =="SANAYÝ" || companyNameParsed.words[i] == "SANAYI")
		{
			if (i==companyNameParsed.words.size()-1)
				categories[i]="KURUMSÝL";
			else if (i<companyNameParsed.words.size()-1)
				if (categories[i+1] != "KURUM" && categories[i+1]!="MEVKÝ")
					categories[i]="KURUMSÝL";
		}

		if (i==1 && companyNameParsed.words.size() > 2)
			if (companyNameParsed.words[i] == "SANAYÝ" || companyNameParsed.words[i] =="SANAYI")
				if (categories[i+1] != "KURUMSÝL")
					categories[i]="$$$$$";

		if (categories[i] !="KURUMSÝL")
			start=true;

		if (start)
		{
			companyNameParsed.stdWords.push_back(StringUtil::ReplaceTurkishChars(companyNameParsed.words[i]));
			companyNameParsed.categories.push_back(categories[i]);

			if (categories[i] == "SEKTÖR")
			{
				companyNameParsed.sectorWords.push_back(companyNameParsed.words[i]);
				if (i>0)
					if (i<companyNameParsed.words.size()-1)
					{
						if (categories[i+1] != "KURUM")
							majorNameEnd=true;
					}
					else
						majorNameEnd=true;
			}

			if (!majorNameEnd) {
				companyNameParsed.majorName = companyNameParsed.majorName + " " + companyNameParsed.words[i];
			}
		}
	}

	companyNameParsed.majorName = StringUtil::Trim(companyNameParsed.majorName);

	return companyNameParsed;
}


string Parser::DecideCustomerType(const string& name)
{
	if (name=="")
		return "U";

	string name_t = StringUtil::ToUpper(name);
	name_t = StringUtil::ReplaceTurkishChars(name_t);
	name_t = StringUtil::Replace(name_t, ".",". ");
	name_t = StringUtil::Replace(name_t, "  "," ");
	name_t = StringUtil::Replace(name_t, "  "," ");
	name_t = StringUtil::Replace(name_t, "  "," ");

	vector<string> tokens = StringUtil::Split(name_t,' ');

	for (int i=0; i < tokens.size(); i++)
	{
		if (StringUtil::EndsWith(tokens[i],"."))
			if (tokens[i].size() > 2 || i == tokens.size() - 1)
				return "C";
	}

	string name_t1 = name_t;
	name_t = StringUtil::Replace(name_t, "*"," ");
	name_t = StringUtil::Replace(name_t, ","," ");
	name_t = StringUtil::Replace(name_t, ":"," ");
	name_t = StringUtil::Replace(name_t, "%"," ");
	name_t = StringUtil::Replace(name_t, "#"," ");
	name_t = StringUtil::Replace(name_t, "?"," ");
	name_t = StringUtil::Replace(name_t, "'"," ");
	name_t = StringUtil::Replace(name_t, "|"," ");
	name_t = StringUtil::Replace(name_t, "\""," ");
	name_t = StringUtil::Replace(name_t, "+"," ");
	name_t = StringUtil::Replace(name_t, "-"," ");
	name_t = StringUtil::Replace(name_t, "/"," ");
	name_t = StringUtil::Replace(name_t, "&"," ");
	name_t = StringUtil::Replace(name_t, "_"," ");
	name_t = StringUtil::Replace(name_t, "\\","/");
	name_t = StringUtil::Replace(name_t, "\n"," ");
	name_t = StringUtil::Replace(name_t, "\t"," ");
//	name_t = StringUtil::Replace(name_t, "\u00A0"," ");
	name_t = StringUtil::Replace(name_t, "="," ");
	name_t = StringUtil::Replace(name_t, "("," ");
	name_t = StringUtil::Replace(name_t, ")"," ");
	name_t = StringUtil::Replace(name_t, "{"," ");
	name_t = StringUtil::Replace(name_t, "}"," ");

	if (name_t1 != name_t)
		return "C";

	name_t = StringUtil::Replace(name_t, "."," ");
	name_t = StringUtil::Replace(name_t, "  "," ");
	name_t = StringUtil::Replace(name_t, "  "," ");
	name_t = StringUtil::Replace(name_t, "  "," ");
	name_t = StringUtil::Replace(name_t, "  "," ");

	vector<string> words = StringUtil::Split(StringUtil::Trim(name_t), ' ');

	if (words.size() == 0)
		return "U";
	else if (words.size() == 1)
		return "C";
	else if (words.size() > 4)
		return "C";

	if (!regex_match(words[words.size() - 1], regex("^[A-Za-z|Ö|Þ|Ü|Ð|Ý|Ç|ö|þ|ü|ð|i|ç]{2,30}$")))
		return "C";

	bool includesWholeWord=false;

	for(int i=0;i < words.size()-1; i++)
	{
		if (!regex_match(words[i], regex("^[A-Za-z|Ö|Þ|Ü|Ð|Ý|Ç|ö|þ|ü|ð|i|ç]{2,30}$")) && !regex_match(words[i], regex("^[A-Za-z|Ö|Þ|Ü|Ð|Ý|Ç|ö|þ|ü|ð|i|ç].?$")))
			return "C";

		if (regex_match(words[i], regex("^[A-Za-z|Ö|Þ|Ü|Ð|Ý|Ç|ö|þ|ü|ð|i|ç]{2,30}$")))
			includesWholeWord=true;
	}

	if (!includesWholeWord)
		return "C";


	vector<string> categories = CategorizeWords(words);

	int nonPersonNameCount=0;

	for (int i=0; i < words.size(); i++)
	{
		if (!dictionary->PersonNameContains(words[i]) && !dictionary->SurnameContains(words[i]))
		{
			if (i > 0 && (categories[i] == "SEKTÖR" || categories[i] == "KURUM" || categories[i] == "KURUMSÝL"))
				return "C";

			nonPersonNameCount++;
		}
	}

	if (words.size() == 2)
	{
		if (nonPersonNameCount==2)
			return "C";
	}
	else if (words.size() >=3)
	{
		if (nonPersonNameCount>=2)
			return "C";
	}

	return "P";
}
