/*
 * Column.h
 *
 *  Created on: 29 Eki 2021
 *      Author: koray.korkut
 */
#ifndef SRC_FIELD_H_
#define SRC_FIELD_H_

#include <string>
#include "data-type.h"

using namespace std;

struct Field {
		string name;
		DataType type;
		int length;
		int scale;
};

#endif /* SRC_FIELD_H_ */
