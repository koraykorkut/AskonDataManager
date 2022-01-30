/*
 * matching-field.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_MATCHING_FIELD_H_
#define SRC_MATCHING_FIELD_H_

#include <string>
#include "match-type.h"

using namespace std;


struct MatchingField {
	string name;
	MatchType type;
	float weight;
	int noLowScore=0;
	int minSignificantScore=0;
};

#endif /* SRC_MATCHING_FIELD_H_ */
