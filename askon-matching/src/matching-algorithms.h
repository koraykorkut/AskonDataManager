/*
 * matching-algorithms.h
 *
 *  Created on: 16 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_MATCHING_ALGORITHMS_H_
#define ALGORITHM_MATCHING_ALGORITHMS_H_

#include <string>
#include <vector>
#include <map>
#include "dictionary.h"
#include "parser.h"

using namespace std;

class MatchingAlgorithms {
public:
	MatchingAlgorithms();
	~MatchingAlgorithms();

	enum class EnumScoreNonMatchingOptions {ALL,FIRST_THREE,LONGER_THAN_TWO,NONE};

	struct ScoreAdjustmentRange {
		int minScore;
		int maxScore;
		int adjustmentScore;
		ScoreAdjustmentRange(int minScore, int maxScore, int adjustmentScore)
		{
			this->minScore = minScore;
			this->maxScore = maxScore;
			this->adjustmentScore = adjustmentScore;
		}
	};

	struct FirstWordScoreLimit {
		int limitScore;
		int adjustmentScore;
		FirstWordScoreLimit(int limitScore, int adjustmentScore)
		{
			this->limitScore = limitScore;
			this->adjustmentScore = adjustmentScore;
		}
	};

	struct CompanyNameMatchPattern {
		string pattern;
		int FirstWordNonCommonBonus=0;
		int SecondWordNonCommonBonus=0;
		int FirstWordCommonPenalty=0;
		int SecondWordCommonPenalty=0;
		int SanTicMismatchPenalty=0;
		int MandatoryMatchWordThreshold=100;

		bool enableMultiPositiveAdjustment=true;
		bool enableMultiNegativeAdjustment=true;

		bool ignoreXXIfPossible=false;

		vector<ScoreAdjustmentRange> scoreAdjustmentList;
		vector<FirstWordScoreLimit> firstWordScoreLimitList;
		vector<FirstWordScoreLimit> secondWordScoreLimitList;
		vector<EnumScoreNonMatchingOptions> nonMatchingOptions;

		CompanyNameMatchPattern()
		{

		};

		CompanyNameMatchPattern(const string& pattern)
		{
				this->pattern=pattern;
				this->nonMatchingOptions.push_back(EnumScoreNonMatchingOptions::ALL);
		}
	};

	struct CompanyMatchResult
	{
		int score;
		string pattern;

		CompanyMatchResult(int score, const string& pattern) {
			this->score = score;
			this->pattern = pattern;
		}
	};

	static size_t LevenshteinDistance(const string& s1, const string& s2);

	static string StandardizeWord(const string& word);

	static int StdWordMatch(const string& name1, const string& name2, bool standardize);

	int WordsMatch(const string& name1, const string& name2, bool refMin,
			int orderPenalty, int missingWordPenalty, int initialsMatchScore,
			int abbrevation_match_score, bool rawMatch);

	CompanyMatchResult CompanyStdWordsMatch(const map<string,CompanyNameMatchPattern>& companyNameMatchPatternMap, int initialsMatchScore,
			const vector<string>& words1, const vector<string>& categories1, const vector<string>& words2, const vector<string>& categories2);

	int PersonNameMatch(const string& match_level, const string& name1, const string& name2);

	int CompanyNameMatch(const string match_level, const string name1, const string name2, int standardizationLevel);

	CompanyMatchResult CompanyNameMatchAnalyze(const string match_level, const string name1, const string name2, int standardizationLevel);

	void LoadScoringDefinition();
private:
	Dictionary* dictionary = NULL;
	Parser* parser = NULL;

	enum class MatchPrecision {UNCOMMON, COMMON, VERY_COMMON, PARTIAL, NOT_MATCHED};

	enum class MatchPosition {NAME1, NAME2, NAME3, NAME, SECTOR1, SECTOR2, SECTOR3, SECTOR, UNKNOWN1, UNKNOWN2, UNKNOWN, NOT_MATCHED, NOISE_WORD};

	struct PersonNameMatchDefinition
	{
		bool refmin;
		int missingWordPenalty;
		int orderPenalty;
		int initialsMatchScore;
		int abbrevationMatchScore;
	};

	map<string, map<string,CompanyNameMatchPattern>> companyNameMatchPatterns;
	map<string,PersonNameMatchDefinition> personNameMatchDefinitions;

	void LoadScoringDefinition(string definition,
			map<string,map<string,CompanyNameMatchPattern>>& companyNameMatchPatterns,
			map<string,PersonNameMatchDefinition>& personNameMatchDefinitions);
};

#endif /* ALGORITHM_MATCHING_ALGORITHMS_H_ */
