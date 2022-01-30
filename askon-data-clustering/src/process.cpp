#include "process.h"

/*
 * process-definition.cpp
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#include <iostream>
#include <utility>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <yaml.h>
#include <map>
#include <string-util.h>
#include <parser.h>
#include <matching-algorithms.h>
#include <damerau-levenshtein-distance.h>
#include "process.h"
#include "transformation-definition.h"
#include "match-level.h"

using namespace std;

Process::Process() {
}

Process::~Process()
{
	delete dictionary;
}


Process::Process(string filename) {
	// TODO Auto-generated constructor stub
	Load(filename);
}

void Process::Run()
{
	for(JobDefinition jobDefinition : jobs )
		jobDefinition.Run();
}

void Process::LoadFileDefinitions(yaml_parser_t *parser)
{
	yaml_event_t event;
    int state = 0;
	string currentKey;
	FileDefinition *newFileDefinition;

    do {
        if (!yaml_parser_parse(parser, &event)) {
    	    fputs("Parse error!\n", stderr);
    	    exit(EXIT_FAILURE);
        }
        switch(event.type) {
    		case YAML_SEQUENCE_START_EVENT:
    			if (state == 0)
    				state = 1;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;
    		case YAML_SEQUENCE_END_EVENT:
    			if (state == 1)
    				return;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;
        	case YAML_MAPPING_START_EVENT:
        		if (state == 1) {
        			state = 2;
        			newFileDefinition = new FileDefinition;
        		}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_MAPPING_END_EVENT:
        		if (state == 2) {
        			state = 1;
        			fileDefinitions.insert(pair<string,FileDefinition>(newFileDefinition->name, *newFileDefinition));
        		}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_SCALAR_EVENT:
        		if (state == 2)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 3;
        		}
        		else if (state == 3)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 2;
        			if (currentKey == "name")
        				newFileDefinition->name = value;
        			else if (currentKey == "path")
        				newFileDefinition->path = value;
        			else if (currentKey == "format")
        			{
        				if (value == "Fixed")
        					newFileDefinition->format = FileFormat::Fixed;
        				else if (value == "Delimited")
        					newFileDefinition->format = FileFormat::Delimited;
        				else
        				{
						   fprintf(stderr, "Invalid document error!: file format '%s' not defined.\n", value.c_str());
						   exit(EXIT_FAILURE);
        				}

        			}
        			else if (currentKey == "delimiter")
        			{
        				if (value == "Comma")
        					newFileDefinition->delimiter = Delimiter::Comma;
        				else if (value == "Tab")
        					newFileDefinition->delimiter = Delimiter::Tab;
        				else if (value == "Space")
        					newFileDefinition->delimiter = Delimiter::Space;
        				else if (value == "Pipe")
        					newFileDefinition->delimiter = Delimiter::Pipe;
        				else
        				{
						   fprintf(stderr, "Invalid document error!: delimiter '%s' not defined.\n", value.c_str());
						   exit(EXIT_FAILURE);
        				}
        			}
        			else if (currentKey == "data-definition")
        			{
        				map<string,DataDefinition>::iterator it = dataDefinitions.find(value);
        				if (it != end(dataDefinitions))
        				{
        					newFileDefinition->dataDefinition = it->second;
        				}
        				else
        				{
						   fprintf(stderr, "Invalid document error!: data-definition '%s' not found.\n", value.c_str());
						   exit(EXIT_FAILURE);
        				}
        			}
        			else
        			{
					   fprintf(stderr, "Invalid document error!: field '%s' is not valid for file-definitions section.\n", currentKey.c_str());
               	       exit(EXIT_FAILURE);
        			}
        		}
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
            default:
            	break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
           yaml_event_delete(&event);
   } while(event.type != YAML_STREAM_END_EVENT);

   yaml_event_delete(&event);
}

void Process::LoadDataDefinitions(yaml_parser_t *parser)
{
	yaml_event_t event;
    int state = 0;
	string currentKey;
	DataDefinition *newDataDefinition;
	Field *newField;

    do {
        if (!yaml_parser_parse(parser, &event)) {
    	    fputs("Parse error!\n", stderr);
    	    exit(EXIT_FAILURE);
        }
        switch(event.type) {
    		case YAML_SEQUENCE_START_EVENT:
    			if (state == 0)
    				state = 1;
    			else if (state == 3) {
    				if (currentKey == "fields")
    					state = 4;
    				else
    				{
 					    fprintf(stderr, "Invalid document error!: field '%s' is not valid for data-definitions section.\n", currentKey.c_str());
    					exit(EXIT_FAILURE);
    				}
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;
    		case YAML_SEQUENCE_END_EVENT:
    			if (state == 4)
    				state = 2;
    			else if (state == 1)
    				return;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;

        	case YAML_MAPPING_START_EVENT:
        		if (state == 1) {
        			state = 2;
        			newDataDefinition = new DataDefinition;
        		}
    			else if (state == 4) {
    				state = 5;
    				newField = new Field();
					newField->scale = 0;
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_MAPPING_END_EVENT:
        		if (state == 2) {
        			state = 1;
        			dataDefinitions.insert(pair<string,DataDefinition>(newDataDefinition->name, *newDataDefinition));
        		}
    			else if (state == 5) {
    				state = 4;
    				if (newField->name == "ClusterId")
    				{
    					newField->type = DataType::Number;
    					newField->length = 12;
    					newField->scale = 0;
    				}
    				newDataDefinition->fields.push_back(*newField);
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_SCALAR_EVENT:
        		if (state == 2)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 3;
        		}
        		else if (state == 3)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 2;
        			if (currentKey == "name")
        				newDataDefinition->name = value;
        		}
        		else if (state == 5)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 6;
        		}
        		else if (state == 6)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 5;
        			if (currentKey == "name")
        				newField->name = value;
        			else if (currentKey == "type")
        			{
        				if (value == "Number")
        					newField->type = DataType::Number;
        				else if (value == "Text")
        					newField->type = DataType::Text;
        				else if (value == "Date")
        					newField->type = DataType::Date;
            			else
            			{
     					   fprintf(stderr, "Invalid document error!: '%s' is not a valid data type.\n", value.c_str());
                   	       exit(EXIT_FAILURE);
            			}
        			}
        			else if (currentKey == "length")
        			{
        				int length = atoi(value.c_str());
        				if (length > 0)
        					newField->length = length;
            			else
            			{
     					   fprintf(stderr, "Invalid document error!: Invalid number %s for field length.\n", value.c_str());
                   	       exit(EXIT_FAILURE);
            			}
        			}
        			else if (currentKey == "scale")
        			{
        				int scale = atoi(value.c_str());
        				if (scale > 0)
        					newField->scale = scale;
        				else if (value == "0")
        					newField->scale = 0;
            			else
            			{
      					   fprintf(stderr, "Invalid document error!: Invalid number %s for field scale.\n", value.c_str());
                   	       exit(EXIT_FAILURE);
            			}
        			}
        			else
        			{
 					   fprintf(stderr, "Invalid document error!: field '%s' is not valid for data-definitions->fields section.\n", currentKey.c_str());
               	       exit(EXIT_FAILURE);
        			}
        		}
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
            default:
            	break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
           yaml_event_delete(&event);
   } while(event.type != YAML_STREAM_END_EVENT);

   yaml_event_delete(&event);
}

void Process::LoadMatchingDefinitions(yaml_parser_t *parser)
{
	yaml_event_t event;
    int state = 0;
	string currentKey;
	MatchingDefinition *newMatchingDefinition;
	MatchingField *newMatchingField;

    do {
        if (!yaml_parser_parse(parser, &event)) {
    	    fputs("Parse error!\n", stderr);
    	    exit(EXIT_FAILURE);
        }
        switch(event.type) {
    		case YAML_SEQUENCE_START_EVENT:
    			if (state == 0)
    				state = 1;
    			else if (state == 3) {
    				if (currentKey == "matching-fields")
    					state = 4;
    				else
    				{
 					    fprintf(stderr, "Invalid document error!: field '%s' is not valid for matching-definitions section.\n", currentKey.c_str());
    					exit(EXIT_FAILURE);
    				}
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;
    		case YAML_SEQUENCE_END_EVENT:
    			if (state == 4)
    				state = 2;
    			else if (state == 1)
    				return;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;

        	case YAML_MAPPING_START_EVENT:
        		if (state == 1) {
        			state = 2;
        			newMatchingDefinition = new MatchingDefinition;
        		}
    			else if (state == 4) {
    				state = 5;
    				newMatchingField = new MatchingField();
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_MAPPING_END_EVENT:
        		if (state == 2) {
        			state = 1;
        			matchingDefinitions.insert(pair<string,MatchingDefinition>(newMatchingDefinition->name, *newMatchingDefinition));
        		}
    			else if (state == 5) {
    				state = 4;
    				newMatchingDefinition->matchingFields.push_back(*newMatchingField);
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_SCALAR_EVENT:
        		if (state == 2)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 3;
        		}
        		else if (state == 3)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 2;
        			if (currentKey == "name")
        				newMatchingDefinition->name = value;
        		}
        		else if (state == 5)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 6;
        		}
        		else if (state == 6)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 5;
        			if (currentKey == "name")
        				newMatchingField->name = value;
        			else if (currentKey == "match-type")
        			{
        				if (value == "Exact")
        					newMatchingField->type = MatchType::Exact;
        				else if (value == "Text")
        					newMatchingField->type = MatchType::Text;
        				else if (value == "PersonName")
        					newMatchingField->type = MatchType::PersonName;
        				else if (value == "OrganizationName")
        					newMatchingField->type = MatchType::OrganizationName;
        				else if (value == "Address")
        					newMatchingField->type = MatchType::Address;
        				else if (value == "Date")
        					newMatchingField->type = MatchType::Date;
        				else
            			{
     					   fprintf(stderr, "Invalid document error!: '%s' is not a valid match type.\n", value.c_str());
                   	       exit(EXIT_FAILURE);
            			}
        			}
        			else if (currentKey == "weight")
        			{
        				float weight = atof(value.c_str());
        				if (weight > 0)
        					newMatchingField->weight = weight;
        				else if (value == "0")
        					newMatchingField->weight = 0;
            			else
            			{
     					   fprintf(stderr, "Invalid document error!: Invalid number %s for field weight.\n", value.c_str());
                   	       exit(EXIT_FAILURE);
            			}
        			}
        			else
        			{
 					   fprintf(stderr, "Invalid document error!: field '%s' is not valid for matching-definitions->matching-fields section.\n", currentKey.c_str());
               	       exit(EXIT_FAILURE);
        			}
        		}
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
            default:
            	break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
           yaml_event_delete(&event);
   } while(event.type != YAML_STREAM_END_EVENT);

   yaml_event_delete(&event);
}

void Process::LoadTransformationDefinitions(yaml_parser_t* parser)
{
    yaml_event_t event;
    int state = 0;
    string currentKey;
    TransformationDefinition* newTransformationDefinition;
    FieldMapping* newFieldMapping;

    do {
        if (!yaml_parser_parse(parser, &event)) {
            fputs("Parse error!\n", stderr);
            exit(EXIT_FAILURE);
        }
        switch (event.type) {
        case YAML_SEQUENCE_START_EVENT:
            if (state == 0)
                state = 1;
            else if (state == 3) {
                if (currentKey == "field-mappings")
                    state = 4;
                else
                {
                    fprintf(stderr, "Invalid document error!: field '%s' is not valid for transformation-definitions section.\n", currentKey.c_str());
                    exit(EXIT_FAILURE);
                }
            }
            else {
                fputs("Invalid document error!\n", stderr);
                exit(EXIT_FAILURE);
            }
            break;
        case YAML_SEQUENCE_END_EVENT:
            if (state == 4)
                state = 2;
            else if (state == 1)
                return;
            else {
                fputs("Invalid document error!\n", stderr);
                exit(EXIT_FAILURE);
            }
            break;

        case YAML_MAPPING_START_EVENT:
            if (state == 1) {
                state = 2;
                newTransformationDefinition = new TransformationDefinition;
            }
            else if (state == 4) {
                state = 5;
                newFieldMapping = new FieldMapping;
            }
            else {
                fputs("Invalid document error!\n", stderr);
                exit(EXIT_FAILURE);
            }
            break;
        case YAML_MAPPING_END_EVENT:
            if (state == 2) {
                state = 1;
                transformationDefinitions.insert(pair<string, TransformationDefinition>(newTransformationDefinition->name, *newTransformationDefinition));
            }
            else if (state == 5) {
                state = 4;
                newTransformationDefinition->fieldMappings.push_back(*newFieldMapping);
            }
            else {
                fputs("Invalid document error!\n", stderr);
                exit(EXIT_FAILURE);
            }
            break;
        case YAML_SCALAR_EVENT:
            if (state == 2)
            {
                currentKey = (char*)event.data.scalar.value;
                state = 3;
            }
            else if (state == 3)
            {
                string value = (char*)event.data.scalar.value;
                state = 2;
                if (currentKey == "name")
                    newTransformationDefinition->name = value;
            }
            else if (state == 5)
            {
                currentKey = (char*)event.data.scalar.value;
                state = 6;
            }
            else if (state == 6)
            {
                string value = (char*)event.data.scalar.value;
                state = 5;
                if (currentKey == "input-field")
                    newFieldMapping->inputField = value;
                else if (currentKey == "output-field")
                    newFieldMapping->outputField = value;
                else if (currentKey == "transformation")
                    newFieldMapping->transformation = value;
                else
                {
                    fprintf(stderr, "Invalid document error!: field '%s' is not valid for tranformation-definitions->field-mappings section.\n", currentKey.c_str());
                    exit(EXIT_FAILURE);
                }
            }
            else {
                fputs("Invalid document error!\n", stderr);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            break;
        }
        if (event.type != YAML_STREAM_END_EVENT)
            yaml_event_delete(&event);
    } while (event.type != YAML_STREAM_END_EVENT);

    yaml_event_delete(&event);
}


void Process::LoadIndexDefinitions(yaml_parser_t *parser)
{
	yaml_event_t event;
    int state = 0;
	string currentKey;
	IndexDefinition *newIndexDefinition;
	KeyField *newKeyField;

    do {
        if (!yaml_parser_parse(parser, &event)) {
    	    fputs("Parse error!\n", stderr);
    	    exit(EXIT_FAILURE);
        }
        switch(event.type) {
    		case YAML_SEQUENCE_START_EVENT:
    			if (state == 0)
    				state = 1;
    			else if (state == 3) {
    				if (currentKey == "key-fields")
    					state = 4;
    				else if (currentKey == "included-fields")
    					state = 7;
        			else
        			{
               	       fputs("Invalid document error!\n", stderr);
               	       exit(EXIT_FAILURE);
        			}
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;
    		case YAML_SEQUENCE_END_EVENT:
    			if (state == 4 || state == 7)
    				state = 2;
    			else if (state == 1)
    				return;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;

        	case YAML_MAPPING_START_EVENT:
        		if (state == 1) {
        			state = 2;
        			newIndexDefinition = new IndexDefinition;
        		}
    			else if (state == 4) {
    				state = 5;
    				newKeyField = new KeyField();
    			}
    			else if (state == 7) {
    				state = 8;
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_MAPPING_END_EVENT:
        		if (state == 2) {
        			state = 1;
        			indexDefinitions.insert(pair<string,IndexDefinition>(newIndexDefinition->name, *newIndexDefinition));
        		}
    			else if (state == 5) {
    				state = 4;
    				newIndexDefinition->keyFields.push_back(*newKeyField);
    			}
    			else if (state == 8) {
    				state = 7;
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_SCALAR_EVENT:
        		if (state == 2)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 3;
        		}
        		else if (state == 3)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 2;
        			if (currentKey == "name")
        				newIndexDefinition->name = value;
        			else if ( currentKey == "input-file")
        			{
        				map<string,FileDefinition>::iterator it = fileDefinitions.find(value);
        				if (it != end(fileDefinitions))
        				{
        					newIndexDefinition->inputFile = it->second;
        				}
        				else
        				{
						   fprintf(stderr, "Invalid document error!: file-definition '%s' not found.\n", value.c_str());
						   exit(EXIT_FAILURE);
        				}
        			}
        		}
        		else if (state == 5)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 6;
        		}
        		else if (state == 6)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 5;
        			if (currentKey == "name")
        				newKeyField->fieldName = value;
        			else if (currentKey == "order")
        			{
        				if (value == "Asc")
        					newKeyField->sortOrder = SortOrder::Asc;
        				else if (value == "Desc")
        					newKeyField->sortOrder = SortOrder::Desc;
            			else
            			{
       					   fprintf(stderr, "Invalid document error!: Invalid Sort Order : %s .\n", value.c_str());
                   	       exit(EXIT_FAILURE);
            			}
        			}
        			else
        			{
 					   fprintf(stderr, "Invalid document error!: field '%s' is not valid for index-definitions->key-fields section.\n", currentKey.c_str());
               	       exit(EXIT_FAILURE);
        			}
        		}
        		else if (state == 8)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 9;
        		}
        		else if (state == 9)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 8;
        			if (currentKey == "name")
        				newIndexDefinition->includedFields.push_back(value);
        			else
        			{
 					   fprintf(stderr, "Invalid document error!: field '%s' is not valid for index-definitions->included-fields section.\n", currentKey.c_str());
               	       exit(EXIT_FAILURE);
        			}
        		}
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
            default:
            	break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
           yaml_event_delete(&event);
   } while(event.type != YAML_STREAM_END_EVENT);

   yaml_event_delete(&event);
}

bool is_digits(const string& str)
{
    for (int i = 0; i < str.size(); i++)
        if (!isdigit(str[i]))
            return false;
    return true;
}

void Process::LoadJobDefinitions(yaml_parser_t *parser)
{
	yaml_event_t event;
    int state = 0;
	string currentKey;
	JobDefinition *newJobDefinition;
    KeyField* newKeyField;
	string matchingDefinitionName;

    do {
        if (!yaml_parser_parse(parser, &event)) {
    	    fputs("Parse error!\n", stderr);
    	    exit(EXIT_FAILURE);
        }
        switch(event.type) {
    		case YAML_SEQUENCE_START_EVENT:
    			if (state == 0)
    				state = 1;
    			else if (state == 3) {
                    if (currentKey == "key-fields")
                        state = 4;
    				else
    				{
 					    fprintf(stderr, "Invalid document error!: field '%s' is not valid for job-definitions section.\n", currentKey.c_str());
    					exit(EXIT_FAILURE);
    				}
    			}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;
    		case YAML_SEQUENCE_END_EVENT:
    			if (state == 4)
    				state = 2;
    			else if (state == 1)
    				return;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;

        	case YAML_MAPPING_START_EVENT:
        		if (state == 1) {
        			state = 2;
        			newJobDefinition = new JobDefinition;
        		}
                else if (state == 4) {
                    state = 5;
                    newKeyField = new KeyField();
                }
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_MAPPING_END_EVENT:
        		if (state == 2) {
        			state = 1;
        			jobDefinitions.insert(pair<string,JobDefinition>(newJobDefinition->name, *newJobDefinition));
        		}
                else if (state == 5) {
                    state = 4;
                    newJobDefinition->keyFields.push_back(*newKeyField);
                }
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_SCALAR_EVENT:
        		if (state == 2)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 3;
        		}
        		else if (state == 3)
        		{
        			string value = (char*) event.data.scalar.value;
        			state = 2;
        			if (currentKey == "name")
        				newJobDefinition->name = value;
        			else if (currentKey == "type")
        			{
        				if (value == "Extract")
        					newJobDefinition->type = JobType::Extract;
        				else if (value == "Transform")
        					newJobDefinition->type = JobType::Transform;
        				else if (value == "Load")
        					newJobDefinition->type = JobType::Load;
        				else if (value == "Cluster")
        					newJobDefinition->type = JobType::Cluster;
            			else
            			{
       					   fprintf(stderr, "Invalid document error!: Invalid Job Type : %s .\n", value.c_str());
                   	       exit(EXIT_FAILURE);
            			}
        			}
        			else if (currentKey == "input-file")
        			{
        				map<string,FileDefinition>::iterator it = fileDefinitions.find(value);
        				if (it != end(fileDefinitions))
        				{
        					newJobDefinition->inputFile = it->second;
        				}
        				else
        				{
    					   fprintf(stderr, "Invalid document error!: file-definition '%s' not found.\n", value.c_str());
    					   exit(EXIT_FAILURE);
        				}
        			}
                    else if (currentKey == "output-file")
                    {
                        map<string, FileDefinition>::iterator it = fileDefinitions.find(value);
                        if (it != end(fileDefinitions))
                        {
                            newJobDefinition->outputFile = it->second;
                        }
                        else
                        {
                            fprintf(stderr, "Invalid document error!: file-definition '%s' not found.\n", value.c_str());
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if (currentKey == "matching-definition")
                    {
                        map<string, MatchingDefinition>::iterator it = matchingDefinitions.find(value);
                        if (it != end(matchingDefinitions))
                        {
                            newJobDefinition->matchingDefinition = &(it->second);
                        }
                        else
                        {
                            fprintf(stderr, "Invalid document error!: matching-definition '%s' not found.\n", value.c_str());
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if (currentKey == "transformation-definition")
                    {
                        map<string, TransformationDefinition>::iterator it = transformationDefinitions.find(value);
                        if (it != end(transformationDefinitions))
                        {
                            newJobDefinition->transformationDefinition = &(it->second);
                        }
                        else
                        {
                            fprintf(stderr, "Invalid document error!: transformation-definition '%s' not found.\n", value.c_str());
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if (currentKey == "input-limit")
                    {
                        if (is_digits(value))
                            newJobDefinition->inputLimit = atoi(value.c_str());
                    }
                    else if (currentKey == "id-field")
                    {
                        newJobDefinition->idField = value;
                    }
                    else if (currentKey == "score-threshold")
                    {
                        if (is_digits(value))
                            newJobDefinition->scoreThreshold = atoi(value.c_str());
                    }
                    else if (currentKey == "match-level")
                    {
                        if (value == "Strict")
                            newJobDefinition->matchLevel = MatchLevel::Strict;
                        else if (value == "Typical")
                            newJobDefinition->matchLevel = MatchLevel::Typical;
                        else if (value == "Loose")
                            newJobDefinition->matchLevel = MatchLevel::Loose;
                        else
                        {
                            fprintf(stderr, "Invalid document error!: '%s' is not a valid match level.\n", value.c_str());
                            exit(EXIT_FAILURE);
                        }
                    }

        		}
                else if (state == 5)
                {
                    currentKey = (char*)event.data.scalar.value;
                    state = 6;
                }
                else if (state == 6)
                {
                    string value = (char*)event.data.scalar.value;
                    state = 5;
                    if (currentKey == "name")
                        newKeyField->fieldName = value;
                    else if (currentKey == "order")
                    {
                        if (value == "Asc")
                            newKeyField->sortOrder = SortOrder::Asc;
                        else if (value == "Desc")
                            newKeyField->sortOrder = SortOrder::Desc;
                        else
                        {
                            fprintf(stderr, "Invalid document error!: Invalid Sort Order : %s .\n", value.c_str());
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Invalid document error!: field '%s' is not valid for index-definitions->key-fields section.\n", currentKey.c_str());
                        exit(EXIT_FAILURE);
                    }
                }
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
            default:
            	break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
           yaml_event_delete(&event);
   } while(event.type != YAML_STREAM_END_EVENT);

   yaml_event_delete(&event);
}

void Process::LoadProcessDefinition(yaml_parser_t *parser)
{
	yaml_event_t event;
    int state = 0;
	string currentKey;

    do {
        if (!yaml_parser_parse(parser, &event)) {
    	    fputs("Parse error!\n", stderr);
    	    exit(EXIT_FAILURE);
        }
        switch(event.type) {
    		case YAML_SEQUENCE_START_EVENT:
    			if (state == 2)
    				state = 3;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;
    		case YAML_SEQUENCE_END_EVENT:
    			if (state == 3)
    				state = 2;
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
    			break;

        	case YAML_MAPPING_START_EVENT:
        		if (state == 0) {
        			state = 1;
        		}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_MAPPING_END_EVENT:
        		if (state == 2) {
        			return;
        		}
                else {
            	     fputs("Invalid document error!\n", stderr);
            	     exit(EXIT_FAILURE);
                }
        		break;
        	case YAML_SCALAR_EVENT:
        		if (state == 1)
        		{
        			currentKey = (char*) event.data.scalar.value;
        			state = 2;
        		}
        		else if (state == 3)
        		{
        			string value = (char*) event.data.scalar.value;
        			if (currentKey == "jobs")
        			{
        				map<string,JobDefinition>::iterator it = jobDefinitions.find(value);
        				if (it != end(jobDefinitions))
        				{
        					jobs.push_back(it->second);
        				}
        				else
        				{
    					   fprintf(stderr, "Invalid document error!: job-definition '%s' not found.\n", value.c_str());
    					   exit(EXIT_FAILURE);
        				}
        			}
        			else
        			{
 					   fprintf(stderr, "Invalid document error!: field '%s' is not valid for process-definition section.\n", currentKey.c_str());
               	       exit(EXIT_FAILURE);
        			}
        		}
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
            default:
            	break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
           yaml_event_delete(&event);
   } while(event.type != YAML_STREAM_END_EVENT);

   yaml_event_delete(&event);
}


void Process::Load(string filename)
{
    FILE* fh = fopen(filename.c_str(), "r");
    yaml_parser_t parser;
    yaml_event_t  event;

    if (!yaml_parser_initialize(&parser))
        fputs("Failed to initialize parser!\n", stderr);
    if (fh == NULL)
        fputs("Failed to open file!\n", stderr);
    yaml_parser_set_input_file(&parser, fh);

    int state = 0;

    do {
        if (!yaml_parser_parse(&parser, &event)) {
	       fputs("Parse error!\n", stderr);
           exit(EXIT_FAILURE);
        }
        switch(event.type) {
        	case YAML_DOCUMENT_START_EVENT:
        		state = 1;
        		break;
        	case YAML_MAPPING_START_EVENT:
        		if (state == 1)
        			state = 2;
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
        	case YAML_SCALAR_EVENT:
        		if (state == 2)
        		{
        			string section = (char*) event.data.scalar.value;
        			if (section == "data-definitions")
                		LoadDataDefinitions(&parser);
        			else if (section == "file-definitions")
                		LoadFileDefinitions(&parser);
        			else if (section == "index-definitions") {
                		LoadIndexDefinitions(&parser);
        			}
        			else if (section == "matching-definitions") {
                		LoadMatchingDefinitions(&parser);
        			}
                    else if (section == "transformation-definitions") {
                        LoadTransformationDefinitions(&parser);
                    }
        			else if (section == "job-definitions") {
                    	LoadJobDefinitions(&parser);
        			}
        			else if (section == "process-definition") {
                        LoadProcessDefinition(&parser);
        			}
        		}
        		else {
        	        fputs("Invalid document error!\n", stderr);
        	        exit(EXIT_FAILURE);
        		}
        		break;
            default:
            	break;

        }
        if(event.type != YAML_STREAM_END_EVENT)
           yaml_event_delete(&event);
   } while(event.type != YAML_STREAM_END_EVENT);

   yaml_event_delete(&event);
   yaml_parser_delete(&parser);
   fclose(fh);
}

