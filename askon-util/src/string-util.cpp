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
			case '?': c='U';break;
			case '?': c='I';break;
			case '?': c='S';break;
			case '?': c='G';break;
			case '?': c='O';break;
			case '?': c='C';break;
		}
		returnText += c;
	}

	return returnText;
}


char StringUtil::ReplaceTurkishChars(const char &c)
{
	switch(c)
	{
		case '?': return 'U';break;
		case '?': return 'I';break;
		case '?': return 'S';break;
		case '?': return 'G';break;
		case '?': return 'O';break;
		case '?': return 'C';break;
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
			case '?': c='?';break;
			case 'i': c='?';break;
			case '?': c='?';break;
			case '?': c='?';break;
			case '?': c='?';break;
			case '?': c='I';break;
			case '?': c='?';break;
			default: c = std::toupper(c);break;
		}
		returnText += c;
	}

	return returnText;
}


string StringUtil::Trim(const string &str ) {
	auto start = str.begin();
	while (start != str.end() && std::isspace(static_cast<unsigned char>(*start))) {
		start++;
	}

	auto end = str.end();
	do {
		end--;
	} while (std::distance(start, end) > 0 && std::isspace(static_cast<unsigned char>(*end)));

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

