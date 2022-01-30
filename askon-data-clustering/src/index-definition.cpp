#include "index-definition.h"
/*
 * index-definition.cpp
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <forward_list>
#include <list>
#include "merge-heap-node.h"
#include "string-util.h"
#include "util.h"

using namespace std;

struct IndexFileRow
{
	vector<string> data;

};

IndexDefinition::IndexDefinition() {
	// TODO Auto-generated constructor stub

}

void IndexDefinition::GenerateTempFile()
{
	vector<int> keyFieldIndexes;
	vector<int> includedFieldIndexes;
	vector<Field> dataDefinitionFields = this->inputFile.dataDefinition.fields;

	for (KeyField keyField : this->keyFields)
		for(int n = 0; n < dataDefinitionFields.size(); n++ )
			if (keyField.fieldName == dataDefinitionFields[n].name)
				keyFieldIndexes.push_back(n);

	for (string includedFieldName : this->includedFields)
		for(int n = 0; n < dataDefinitionFields.size(); n++ )
			if (includedFieldName == dataDefinitionFields[n].name)
				includedFieldIndexes.push_back(n);

	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);

	ifstream inputFileStream(inputFile.path);
	ofstream outputFileStream("data/" + this->name+"_tmp");
	string line;

	while (std::getline(inputFileStream, line))
	{
		vector<string> columns = StringUtil::Split(line, delimiter);

		string outputStr = "";

		for(int i = 0; i<keyFieldIndexes.size(); i++) {
			outputStr += columns[keyFieldIndexes[i]];
			if (i < keyFieldIndexes.size() - 1 || includedFieldIndexes.size() > 0)
				outputStr += delimiter;
		}

		for(int i = 0; i<includedFieldIndexes.size(); i++) {
			outputStr += columns[includedFieldIndexes[i]];
			if (i < includedFieldIndexes.size() - 1)
				outputStr += delimiter;
		}

		outputFileStream << outputStr << "\n";
	}

	inputFileStream.close();
	outputFileStream.close();
}

void IndexDefinition::GeneratePartitions()
{
	vector<int> keyFieldIndexes;
	vector<int> includedFieldIndexes;
	vector<Field> dataDefinitionFields = this->inputFile.dataDefinition.fields;

	for (KeyField keyField : this->keyFields)
		for(int n = 0; n < dataDefinitionFields.size(); n++ )
			if (keyField.fieldName == dataDefinitionFields[n].name)
				keyFieldIndexes.push_back(n);

	for (string includedFieldName : this->includedFields)
		for(int n = 0; n < dataDefinitionFields.size(); n++ )
			if (includedFieldName == dataDefinitionFields[n].name)
				includedFieldIndexes.push_back(n);

	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);

	int partitionNumber = 1;

	ifstream inputFileStream("data/" + this->name+"_tmp");
	string line;

	int partitionBytes = 0;
	int totalFieldCount = this->keyFields.size() + this->includedFields.size();
	int lineCount = 1;

	int bufferSizeInBytes = this->memoryBufferSize * 1048576;
	forward_list<vector<string>> tempBuffer;
	while (std::getline(inputFileStream, line))
	{
		vector<string> columns = StringUtil::Split(line, delimiter);
		if (columns.size() != totalFieldCount) {
		   fprintf(stderr, "Invalid number of columns in temp file during index creation: Temp file line %d\n", lineCount);
		   exit(EXIT_FAILURE);
		}
		tempBuffer.push_front(columns);
		partitionBytes += line.size() * sizeof(char);
		lineCount++;
		if (partitionBytes >= bufferSizeInBytes)
		{
			tempBuffer.sort(RowCompare(*this));
			ofstream outputFileStream("data/" + this->name+"_tmp"+to_string(partitionNumber++));
			for(vector<string> item : tempBuffer)
			{
				string outputStr = StringUtil::Join(item, delimiter);
				outputFileStream << outputStr << "\n";
			}
			partitionBytes = 0;
			outputFileStream.close();
			tempBuffer.clear();
		}
	}
	inputFileStream.close();

	if (!tempBuffer.empty()) {
		tempBuffer.sort(RowCompare(*this));
		ofstream outputFileStream("data/" + this->name+"_tmp"+ to_string(partitionNumber));
		for(vector<string> item : tempBuffer)
		{
			string outputStr = StringUtil::Join(item, delimiter);
			outputFileStream << outputStr << "\n";
		}
		outputFileStream.close();
	}
}

void IndexDefinition::GenerateSortedFile()
{
    string path = "data/";
    vector<string> partitionFilenames;
    for (const auto & entry : filesystem::directory_iterator(path))
    {
    	string fileName = entry.path().filename().string();
    	if (fileName.rfind(this->name + "_tmp") == 0 && fileName != this->name + "_tmp")
    		partitionFilenames.push_back(fileName);
    }

    int partitionCount = partitionFilenames.size();

	long bufferSizeInBytes = this->memoryBufferSize * 1048576 / (partitionCount + 1) ;

	vector<list<vector<string>>> partitions = vector<list<vector<string>>>(partitionCount);
	vector<ifstream> partitionInputFileStreams = vector<ifstream>(partitionCount);
	ofstream outputFileStream("data/" + this->name);
	vector<MergeHeapNode> partitionHeadHeap;

	for (int i=0; i < partitionCount; i++)
	{
		partitionInputFileStreams[i] = ifstream("data/" + partitionFilenames[i]);
		ReadPartition(partitions[i], partitionInputFileStreams[i], bufferSizeInBytes);
		MergeHeapNode node(partitions[i].front(), i);
		partitionHeadHeap.push_back(node);
		partitions[i].pop_front();
	}

	make_heap(partitionHeadHeap.begin(), partitionHeadHeap.end(), MergeHeapNodeCompare(*this));
	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);

	int writtenLines = 0;
	while (!partitionHeadHeap.empty())
	{
		pop_heap(partitionHeadHeap.begin(),partitionHeadHeap.end(), MergeHeapNodeCompare(*this));
		MergeHeapNode head = partitionHeadHeap.back();
		partitionHeadHeap.pop_back();
		string outputStr = StringUtil::Join(head.data, delimiter);
		outputFileStream << outputStr << "\n";
		writtenLines++;

		if (partitions[head.partitionIndex].empty())
				ReadPartition(partitions[head.partitionIndex], partitionInputFileStreams[head.partitionIndex], bufferSizeInBytes);

		if (!partitions[head.partitionIndex].empty())
		{
			vector<string> nextRow = partitions[head.partitionIndex].front();
			partitions[head.partitionIndex].pop_front();
			partitionHeadHeap.push_back(MergeHeapNode(nextRow, head.partitionIndex));
			push_heap(partitionHeadHeap.begin(),partitionHeadHeap.end(), MergeHeapNodeCompare(*this));
		}
	}
}


void IndexDefinition::ReadPartition(list<vector<string>> &partition, ifstream &inputFileStream, long size)
{
	partition.clear();
	string line;

	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);
	int totalFieldCount = this->keyFields.size() + this->includedFields.size();
	long partitionBytes = 0;

	while (partitionBytes <= size && std::getline(inputFileStream, line))
	{
		vector<string> columns = StringUtil::Split(line, delimiter);
		if (columns.size() != totalFieldCount) {
		   fprintf(stderr, "Invalid number of columns in temp index file during index creation.");
		   exit(EXIT_FAILURE);
		}
		partition.push_back(columns);
		partitionBytes += line.size() * sizeof(char);
	}
}

void IndexDefinition::Build()
{
	this->GenerateTempFile();
	this->GeneratePartitions();
	this->GenerateSortedFile();
}
