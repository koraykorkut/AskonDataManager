cmake_minimum_required (VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 17)

project(askon-matching)

add_library(askon-matching 
damerau-levenshtein-distance.h damerau-levenshtein-distance.cpp 
dictionary.h dictionary.cpp
dictionary-txt.h dictionary-txt.cpp
key-generator.h key-generator.cpp
matching-algorithms.h matching-algorithms.cpp
parser.h parser.cpp
phrase.h phrase.cpp
standardization-rule.h standardization-rule.cpp
word.h word.cpp
word-frequency-definition.h word-frequency-definition.cpp)

add_library(askon-util STATIC IMPORTED)
set_property(TARGET askon-util PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../../askon-util/Release/askon-util.lib")

include_directories(${PROJECT_SOURCE_DIR}/../../askon-util/src)

target_link_libraries (askon-matching LINK_PUBLIC askon-util)
