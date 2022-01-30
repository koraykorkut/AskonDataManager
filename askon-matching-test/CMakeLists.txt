cmake_minimum_required (VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 17)

project(askon-matching-test)

add_executable(matching-test "matching-test.cpp" 
"${PROJECT_SOURCE_DIR}/../askon-util/src/string-util.cpp" 
"${PROJECT_SOURCE_DIR}/../askon-matching/src/dictionary.cpp" 
"${PROJECT_SOURCE_DIR}/../askon-matching/src/dictionary-txt.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/phrase.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/damerau-levenshtein-distance.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/key-generator.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/matching-algorithms.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/parser.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/standardization-rule.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/word.cpp"
"${PROJECT_SOURCE_DIR}/../askon-matching/src/word-frequency-definition.cpp"
)

#add_library(askon-util STATIC IMPORTED)
#set_property(TARGET askon-util PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../askon-util/Release/askon-util.lib")

#add_library(askon-matching STATIC IMPORTED)
#set_property(TARGET askon-matching PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../askon-matching/Release/askon-matching.lib")

include_directories("${PROJECT_SOURCE_DIR}/../askon-util/src" "${PROJECT_SOURCE_DIR}/../askon-matching/src" )

#target_link_libraries (matching-test LINK_PUBLIC askon-util askon-matching)