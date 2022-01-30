#ifndef SRC_TRANSFORMATION_DEFINITION_H_
#define SRC_TRANSFORMATION_DEFINITION_H_

#include <string>
#include <vector>
#include "field-mapping.h"

using namespace std;

struct TransformationDefinition
{
	string name;
	vector<FieldMapping> fieldMappings;
};

#endif /* SRC_TRANSFORMATION_DEFINITION_H_ */
