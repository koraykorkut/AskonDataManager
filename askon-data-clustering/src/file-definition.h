/*
 * FileDefinition.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_FILE_DEFINITION_H_
#define SRC_FILE_DEFINITION_H_

#include <string>

#include "data-definition.h"
#include "delimiter.h""
#include "file-format.h"

struct FileDefinition {
		string name;
		string path;
		FileFormat format;
		Delimiter delimiter;
		DataDefinition dataDefinition;
};

#endif /* SRC_FILE_DEFINITION_H_ */
