#ifndef SRC_FIELD_MAPPING_H_
#define SRC_FIELD_MAPPING_H_

#include <string>

using namespace std;

struct FieldMapping {
    string inputField;
    string outputField;
    string transformation;
};

#endif /* SRC_FIELD_MAPPING_H_ */