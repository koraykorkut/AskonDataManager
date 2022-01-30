/*
 * DataDefinition.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_DATA_DEFINITION_H_
#define SRC_DATA_DEFINITION_H_

#include <vector>
#include <string>
#include "field.h"

using namespace std;

struct DataDefinition {
		string name;
		vector<Field> fields;
};

#endif /* SRC_DATA_DEFINITION_H_ */
