cmake_minimum_required (VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 17)

project(askon-data-clustering)

add_executable(askon-data-clustering 
data-manager.cpp job-definition.h job-definition.cpp process.h process.cpp
data-type.h delimiter.h file-format.h job-type.h match-type.h sort-order.h
key-level.h match-level.h search-level.h
data-definition.h
field.h
field-mapping.h
file-definition.h
index-definition.h index-definition.cpp
key-field.h
matching-definition.h
matching-field.h
merge-heap-node.h merge-heap-node.cpp
transformation-definition.h
"${PROJECT_SOURCE_DIR}/../../askon-util/src/string-util.cpp" 
"${PROJECT_SOURCE_DIR}/../../askon-util/src/util.cpp" 
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/dictionary.cpp" 
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/dictionary-txt.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/phrase.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/damerau-levenshtein-distance.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/key-generator.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/matching-algorithms.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/parser.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/standardization-rule.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/word.cpp"
"${PROJECT_SOURCE_DIR}/../../askon-matching/src/word-frequency-definition.cpp"
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/api.c" 
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/dumper.c" 
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/emitter.c" 
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/loader.c"
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/parser.c" 
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/reader.c" 
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/scanner.c" 
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/writer.c" 
"${PROJECT_SOURCE_DIR}/../../libyamlc/src/yaml_private.h"
)

include_directories("${PROJECT_SOURCE_DIR}/../../askon-util/src" "${PROJECT_SOURCE_DIR}/../../askon-matching/src" "${PROJECT_SOURCE_DIR}/../../libyamlc/src" )

