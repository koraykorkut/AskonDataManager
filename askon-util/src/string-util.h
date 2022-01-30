/*
 * string-util.h
 *
 *  Created on: 1 Kas 2021
 *      Author: koray.korkut
 */

#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_

#include <string>
#include <vector>

using namespace std;

class StringUtil {
public:
	static const char FileSeparator =
	#ifdef _WIN32
	                            '\\';
	#else
	                            '/';
	#endif
	static vector<string> Split(const string &str, char delim);
	static string Join(const vector<string>& v, char c);
	static string ReplaceTurkishChars(const string &text);
	static char ReplaceTurkishChars(const char &c);
	static string ToUpper(const string &text);
	static string Trim(const string &str);
	static string Replace(const string& subject, const string& search, const string& replace);
	static bool StartsWith(const string &mainStr, const string &toMatch);
	static bool EndsWith(const string &mainStr, const string &toMatch);
	static bool Contains(const string &mainStr, const string &toMatch);
	static string RemoveUTF8BOM(const string &str);
	static void Test();
};

#endif /* STRING_UTIL_H_ */
