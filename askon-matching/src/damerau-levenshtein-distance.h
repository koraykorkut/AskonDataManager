/*
 * damerau-levenshtein-distance.h
 *
 *  Created on: 14 Kas 2021
 *      Author: koray.korkut
 */

#ifndef ALGORITHM_DAMERAU_LEVENSHTEIN_DISTANCE_H_
#define ALGORITHM_DAMERAU_LEVENSHTEIN_DISTANCE_H_

#include <string>
#include <vector>

using namespace std;

class DamerauLevenshteinDistance {
	public:
		static int Calculate(const string p_string1, const string p_string2);
};
#endif /* ALGORITHM_DAMERAU_LEVENSHTEIN_DISTANCE_H_ */
