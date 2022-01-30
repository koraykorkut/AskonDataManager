/*
 * indexkeyfield.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_KEY_FIELD_H_
#define SRC_KEY_FIELD_H_

#include <string>
#include "sort-order.h"

using namespace std;


struct KeyField {
    string fieldName;
    SortOrder sortOrder;
};

#endif /* SRC_KEY_FIELD_H_ */
