/*
 * matching-definition.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_MATCHING_DEFINITION_H_
#define SRC_MATCHING_DEFINITION_H_

#include <string>
#include <vector>
#include "matching-field.h"

using namespace std;

struct MatchingDefinition {
	string name;
    MatchType matchType;
	vector<MatchingField> matchingFields;
};

#endif /* SRC_MATCHING_DEFINITION_H_ */
