/*
 * process-definition.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_PROCESS_H_
#define SRC_PROCESS_H_

#include <vector>
#include <string>
#include <map>
#include <yaml.h>
#include "job-definition.h"
#include "transformation-definition.h"
#include <dictionary-txt.h>

using namespace std;

class Process {
	public:
		Process();
		~Process();
		Process(string filename);
		void Run();
		void Resume();
		void Load(string filename);
	    Dictionary *dictionary;
	private:
	    map<string, DataDefinition> dataDefinitions;
	    map<string, FileDefinition> fileDefinitions;
	    map<string, IndexDefinition> indexDefinitions;
	    map<string, MatchingDefinition> matchingDefinitions;
		map<string, TransformationDefinition> transformationDefinitions;
	    map<string, JobDefinition> jobDefinitions;
	    vector<JobDefinition> jobs;
	    void LoadDataDefinitions(yaml_parser_t *parser);
	    void LoadFileDefinitions(yaml_parser_t *parser);
	    void LoadIndexDefinitions(yaml_parser_t *parser);
	    void LoadMatchingDefinitions(yaml_parser_t *parser);
		void LoadTransformationDefinitions(yaml_parser_t* parser);
	    void LoadJobDefinitions(yaml_parser_t *parser);
	    void LoadProcessDefinition(yaml_parser_t *parser);
};

#endif /* SRC_PROCESS_H_ */
