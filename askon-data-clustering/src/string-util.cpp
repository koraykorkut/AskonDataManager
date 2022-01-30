/*
 * string-util.cpp
 *
 *  Created on: 1 Kas 2021
 *      Author: koray.korkut
 */

#include "string-util.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>


void StringUtil::Test()
{

	ifstream inputFileStream("test.txt");
	string line;

	if(std::getline(inputFileStream, line))
	{
//		line = RemoveUTF8BOM(line);
		line = ToUpper(line);
		line = ReplaceTurkishChars(line);
		cout << line;
		cout << "\n";
		cout.flush();
	}
}

vector<string> StringUtil::Split(const string &str, char delim)
{
	  vector<string> result;
	  const char *sourceString = str.c_str();
	  string item;
	  int start = 0;
	  int end = 0;
	  while (sourceString[end] != 0)
	  {
		  int strLength=0;
		  end = start;
		  while (sourceString[end] != delim && sourceString[end] != 0) {
			  strLength++;
			  end++;
		  }

		  item = string(&sourceString[start], strLength);
		  result.push_back (item);
		  start = end + 1;
	  }

	  return result;
}

string StringUtil::Join(const vector<string>& v, char c) {

   string s;

   for (vector<string>::const_iterator p = v.begin();
        p != v.end(); ++p) {
      s += *p;
      if (p != v.end() - 1)
        s += c;
   }

   return s;
}

string StringUtil::ReplaceTurkishChars(const string &text)
{
	int n;
	char c;
	string returnText = "";

	for (n=0; n < text.size(); n++)
	{
		c=text[n];
		switch(c)
		{
			case 'Ü': c='U';break;
			case 'Ý': c='I';break;
			case 'Þ': c='S';break;
			case 'Ð': c='G';break;
			case 'Ö': c='O';break;
			case 'Ç': c='C';break;
		}
		returnText += c;
	}

	return returnText;
}


char StringUtil::ReplaceTurkishChars(const char &c)
{
	switch(c)
	{
		case 'Ü': return 'U';break;
		case 'Ý': return 'I';break;
		case 'Þ': return 'S';break;
		case 'Ð': return 'G';break;
		case 'Ö': return 'O';break;
		case 'Ç': return 'C';break;
		default : return c;
	}
}


string StringUtil::ToUpper(const string &text)
{
	int n;
	char c;
	string returnText = "";

	for (n=0; n < text.size(); n++)
	{
		c=text[n];
		switch(c)
		{
			case 'ü': c='Ü';break;
			case 'i': c='Ý';break;
			case 'ð': c='Ð';break;
			case 'ö': c='Ö';break;
			case 'þ': c='Þ';break;
			case 'ý': c='I';break;
			case 'ç': c='Ç';break;
			default: c = std::toupper(c);break;
		}
		returnText += c;
	}

	return returnText;
}


string StringUtil::Trim(const string &str ) {
	auto start = str.begin();
	while (start != str.end() && std::isspace(*start)) {
		start++;
	}

	auto end = str.end();
	do {
		end--;
	} while (std::distance(start, end) > 0 && std::isspace(*end));

	return std::string(start, end + 1);
}

string StringUtil::Replace(const string& subject, const string& search, const string& replace) {
    string returnStr = subject;
	size_t pos = 0;
    while ((pos = returnStr.find(search, pos)) != std::string::npos) {
    	returnStr.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return returnStr;
}

bool StringUtil::StartsWith(const string &mainStr, const string &toMatch)
{
	 if(mainStr.find(toMatch) == 0)
	        return true;
	    else
	        return false;
}

bool StringUtil::EndsWith(const string &mainStr, const string &toMatch)
{
	return mainStr.size() >= toMatch.size() && 0 == mainStr.compare(mainStr.size()-toMatch.size(), toMatch.size(), toMatch);
}

bool StringUtil::Contains(const string &mainStr, const string &toMatch)
{
	if (mainStr.find(mainStr) != string::npos)
		return true;
	else
		return false;
}

string StringUtil::RemoveUTF8BOM(const string &str)
{
	if (StartsWith(str, "\xEF\xBB\xBF"))
		return str.substr(3,string::npos);
	else
		return str;
}

