/*
 * util.h
 *
 *  Created on: 5 Kas 2021
 *      Author: koray.korkut
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "delimiter.h"

class Util {
public:
	Util();
	static char ConvertDelimiterToChar(Delimiter delimiter);
};

#endif /* UTIL_H_ */
