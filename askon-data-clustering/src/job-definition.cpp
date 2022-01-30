/*
 * job-definition.cpp
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#include "job-definition.h"
#include <algorithm>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <forward_list>
#include <list>
#include "merge-heap-node.h"
#include "string-util.h"
#include "util.h"

enum class TransformationRoutine {
	PersonNameKeyStrict,
	PersonNameKeyTypical,
	PersonNameKeyLoose,
	PersonNameSearchKeyStrict,
	PersonNameSearchKeyTypical,
	PersonNameSearchKeyLoose,
	CompanyNameKeyStrict,
	CompanyNameKeyTypical,
	CompanyNameKeyLoose,
	CompanyNameSearchKeyStrict,
	CompanyNameSearchKeyTypical,
	CompanyNameSearchKeyLoose,
	AddressNameKeyStrict,
	AddressNameKeyTypical,
	AddressNameKeyLoose,
	AddressNameSearchKeyStrict,
	AddressNameSearchKeyTypical,
	AddressNameSearchKeyLoose,
	Custom,
	None
};

JobDefinition::JobDefinition() {
	// TODO Auto-generated constructor stub	
}

void JobDefinition::Run() {
	switch (this->type)
	{
		case JobType::Transform: RunTransform();break;
		case JobType::Cluster: RunCluster();break;
		case JobType::Extract: RunExtract();break;
		case JobType::Load: RunLoad();break;
	}
}

vector<string> AddLinesFieldValues(const vector<string>& lines, const vector<string>& values)
{
	vector<string> result;	
	for (string line : lines)
		for (string value : values)
			result.push_back(line+value);

	return result;
}

void JobDefinition::RunTransform()
{
	ifstream inputFileStream(inputFile.path);
	ofstream outputFileStream(this->outputFile.path);
	map<string, int> inputFileFieldIndexes;
	map<string, int> outputFileFieldIndexes;
	vector<int> outputInputIndexMapping = vector<int>(outputFile.dataDefinition.fields.size(),-1);
	vector<string> fieldTransformation = vector<string>(outputFile.dataDefinition.fields.size(), "");
	vector<TransformationRoutine> fieldTransformationRoutine = vector<TransformationRoutine>(outputFile.dataDefinition.fields.size(), TransformationRoutine::None);

	int n = 0;
	for (Field field : inputFile.dataDefinition.fields)
		inputFileFieldIndexes.insert(pair<string, int>(field.name, n++));
	n = 0;
	for (Field field : outputFile.dataDefinition.fields)
		outputFileFieldIndexes.insert(pair<string, int>(field.name, n++));

	for (FieldMapping fieldMapping : transformationDefinition->fieldMappings)
	{
		auto it = outputFileFieldIndexes.find(fieldMapping.outputField);
		if (it != outputFileFieldIndexes.end())
		{
			auto it2 = inputFileFieldIndexes.find(fieldMapping.inputField);
			if (it2 != inputFileFieldIndexes.end())
			{
				outputInputIndexMapping[it->second] = it2->second;
				if (fieldMapping.transformation != "")
					if (fieldMapping.transformation == "PersonNameKey(Strict)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::PersonNameKeyStrict;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}					
					else if (fieldMapping.transformation == "PersonNameKey(Typical)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::PersonNameKeyTypical;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "PersonNameKey(Loose)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::PersonNameKeyLoose;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "PersonNameSearchKey(Strict)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::PersonNameSearchKeyStrict;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "PersonNameSearchKey(Typical)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::PersonNameSearchKeyTypical;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "PersonNameSearchKey(Loose)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::PersonNameSearchKeyLoose;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "CompanyNameKey(Strict)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::CompanyNameKeyStrict;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "CompanyNameKey(Typical)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::CompanyNameKeyTypical;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "CompanyNameKey(Loose)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::CompanyNameKeyLoose;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "CompanyNameSearchKey(Strict)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::CompanyNameSearchKeyStrict;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "CompanyNameSearchKey(Typical)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::CompanyNameSearchKeyTypical;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "CompanyNameSearchKey(Loose)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::CompanyNameSearchKeyLoose;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "AddressNameKey(Strict)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::AddressNameKeyStrict;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "AddressNameKey(Typical)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::AddressNameKeyTypical;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "AddressNameKey(Loose)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::AddressNameKeyLoose;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "AddressNameSearchKey(Strict)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::AddressNameSearchKeyStrict;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "AddressNameSearchKey(Typical)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::AddressNameSearchKeyTypical;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else if (fieldMapping.transformation == "AddressNameSearchKey(Loose)")
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::AddressNameSearchKeyLoose;
						if (keyGenerator == NULL)
							keyGenerator = new KeyGenerator;
					}
					else
					{
						fieldTransformationRoutine[it->second] = TransformationRoutine::Custom;
						fieldTransformation[it->second] = fieldMapping.transformation;
					}
			}
		}
	}

	int lineCount = 0;
	string line;
	while (std::getline(inputFileStream, line))
	{
		vector<string> columns = StringUtil::Split(line, Util::ConvertDelimiterToChar(this->inputFile.delimiter));		
		vector<string> outputLines = vector<string>(1, "");
		for (int n = 0; n < outputFile.dataDefinition.fields.size(); n++)
		{
			if (outputInputIndexMapping[n] != -1) {
				if (fieldTransformationRoutine[n] == TransformationRoutine::None) 
				{
					outputLines = AddLinesFieldValues(outputLines, vector<string>(1, columns[outputInputIndexMapping[n]]));
				}
				else
				{
					vector<string> values;
					switch (fieldTransformationRoutine[n])
					{
						case TransformationRoutine::PersonNameKeyStrict: values = keyGenerator->GeneratePersonNameKeys("Strict", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::PersonNameKeyTypical: values = keyGenerator->GeneratePersonNameKeys("Typical", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::PersonNameKeyLoose: values = keyGenerator->GeneratePersonNameKeys("Loose", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::PersonNameSearchKeyStrict: values = keyGenerator->GeneratePersonNameSearchKeys("Strict", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::PersonNameSearchKeyTypical: values = keyGenerator->GeneratePersonNameSearchKeys("Typical", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::PersonNameSearchKeyLoose: values = keyGenerator->GeneratePersonNameSearchKeys("Loose", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::CompanyNameKeyStrict: values = keyGenerator->GenerateCompanyNameKeys("Strict", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::CompanyNameKeyTypical: values = keyGenerator->GenerateCompanyNameKeys("Typical", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::CompanyNameKeyLoose: values = keyGenerator->GenerateCompanyNameKeys("Loose", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::CompanyNameSearchKeyStrict: values = keyGenerator->GenerateCompanyNameSearchKeys("Strict", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::CompanyNameSearchKeyTypical: values = keyGenerator->GenerateCompanyNameSearchKeys("Typical", columns[outputInputIndexMapping[n]]); break;
						case TransformationRoutine::CompanyNameSearchKeyLoose: values = keyGenerator->GenerateCompanyNameSearchKeys("Loose", columns[outputInputIndexMapping[n]]); break;
					}
					if (!values.empty())
						outputLines = AddLinesFieldValues(outputLines, values);
				}
			}
			
			if (n < outputFile.dataDefinition.fields.size() - 1)
				outputLines = AddLinesFieldValues(outputLines, vector<string>(1,string(1, Util::ConvertDelimiterToChar(this->outputFile.delimiter))));

		}

		for (string line : outputLines)
			outputFileStream << line << endl;

		if (inputLimit > 0)
			if (++lineCount >= inputLimit)
				break;
	}

	inputFileStream.close();
	outputFileStream.close();
}

void JobDefinition::RunCluster()
{
	cout << "Phase A" << endl;
	GeneratePartitions();
	cout << "Phase B" << endl;
	GenerateSortedFile();
	cout << "Phase C" << endl;
	GenerateTempClusters();
	cout << "Phase D" << endl;
	IndexTempClusters();
	cout << "Phase E" << endl;
	GenerateTempClusterPartitions();
	cout << "Phase F" << endl;
	GenerateSortedTempClusters();
	cout << "Phase G" << endl;
	LinkClusters();
	cout << "Phase H" << endl;
	OutputClusters();
	cout << "Phase I" << endl;
}

vector<unsigned int> GetPrevClusters(unsigned int clusterId, fstream* clusterIndexStream)
{
	vector<unsigned int> result;
	
	unsigned int readClusterId;
	unsigned int prevClusterId;
//	unsigned int nextClusterId;
	long clusterPosition;

	clusterIndexStream->seekg((clusterId - 1) * (sizeof(unsigned int) * 3 + sizeof(long)));
	clusterIndexStream->read(reinterpret_cast<char*>(&readClusterId), sizeof(unsigned int));

	if (readClusterId != clusterId)
	{
		fprintf(stderr, "Cluster index file error");
		exit(EXIT_FAILURE);
	}

	clusterIndexStream->read(reinterpret_cast<char*>(&clusterPosition), sizeof(long));
	clusterIndexStream->read(reinterpret_cast<char*>(&prevClusterId), sizeof(unsigned int));
//	clusterIndexStream->read(reinterpret_cast<char*>(&nextClusterId), sizeof(unsigned int));

	if (prevClusterId != 0) {
		result = GetPrevClusters(prevClusterId, clusterIndexStream);
		result.push_back(prevClusterId);
	}

	return result;
}

vector<unsigned int> GetNextClusters(unsigned int clusterId, fstream* clusterIndexStream)
{
	vector<unsigned int> result;

	unsigned int readClusterId;
	unsigned int prevClusterId;
	unsigned int nextClusterId;
	long clusterPosition;

	clusterIndexStream->seekg((clusterId - 1) * (sizeof(unsigned int) * 3 + sizeof(long)));
	clusterIndexStream->read(reinterpret_cast<char*>(&readClusterId), sizeof(unsigned int));

	if (clusterId != clusterId)
	{
		fprintf(stderr, "Cluster index file error");
		exit(EXIT_FAILURE);
	}

	clusterIndexStream->read(reinterpret_cast<char*>(&clusterPosition), sizeof(long));
	clusterIndexStream->read(reinterpret_cast<char*>(&prevClusterId), sizeof(unsigned int));
	clusterIndexStream->read(reinterpret_cast<char*>(&nextClusterId), sizeof(unsigned int));

	if (nextClusterId != 0) {
		result = GetNextClusters(nextClusterId, clusterIndexStream);
		result.insert(result.begin(), nextClusterId);
	}

	return result;
}

void LinkTwoClusters(unsigned int sourceClusterId, unsigned int targetClusterId, fstream *clusterIndexStream)
{
	if (sourceClusterId == targetClusterId)
		return;

	unsigned int clusterId;
	unsigned int prevClusterId;
	unsigned int nextClusterId;
	long clusterPosition;

	clusterIndexStream->seekg((targetClusterId - 1) * (sizeof(unsigned int) * 3 + sizeof(long)));
	clusterIndexStream->read(reinterpret_cast<char*>(&clusterId), sizeof(unsigned int));

	if (clusterId != targetClusterId)
	{
		fprintf(stderr, "Cluster index file error");
		exit(EXIT_FAILURE);
	}

	clusterIndexStream->read(reinterpret_cast<char*>(&clusterPosition), sizeof(long));
	clusterIndexStream->read(reinterpret_cast<char*>(&prevClusterId), sizeof(unsigned int));
	clusterIndexStream->read(reinterpret_cast<char*>(&nextClusterId), sizeof(unsigned int));

	vector<unsigned int> previousClusters = GetPrevClusters(targetClusterId, clusterIndexStream);
	vector<unsigned int> nextClusters = GetNextClusters(targetClusterId, clusterIndexStream);


	for (unsigned int id : previousClusters)
		if (id == sourceClusterId)
			return;

	for (unsigned int id : nextClusters)
		if (id == sourceClusterId)
			return;

	unsigned int tailClusterId;
	
	if (nextClusters.size() == 0)
		tailClusterId = targetClusterId;
	else
		tailClusterId = nextClusters.back();

	vector<unsigned int> prevSourceClusters = GetPrevClusters(sourceClusterId, clusterIndexStream);

	unsigned int headClusterId;

	if (prevSourceClusters.size() == 0)
		headClusterId = sourceClusterId;
	else
		headClusterId = prevSourceClusters.front();

	clusterIndexStream->seekp((headClusterId - 1) * (sizeof(unsigned int) * 3 + sizeof(long))+ sizeof(long) + sizeof(unsigned int));
	clusterIndexStream->write(reinterpret_cast<const char*>(&tailClusterId), sizeof(unsigned int));

	clusterIndexStream->seekp((tailClusterId - 1) * (sizeof(unsigned int) * 3 + sizeof(long))+ sizeof(unsigned int)*2 + sizeof(long));
	clusterIndexStream->write(reinterpret_cast<const char*>(&headClusterId), sizeof(unsigned int));
}

void JobDefinition::LinkClusters()
{
	ifstream idClusterStream("data/" + this->name + "_id_tmp");
	fstream clusterIndexStream("data/" + this->name + "_tmp.ndc", ios::in | ios::out | ios::binary);

	string line;
	string prevId = "";
	unsigned int targetClusterId = 0;
	while (std::getline(idClusterStream, line))	
	{
		vector<string> row = StringUtil::Split(line, ',');
		if (row[1] == prevId)
		{
			unsigned int sourceClusterId = atol(row[0].c_str());
			LinkTwoClusters(sourceClusterId, targetClusterId, &clusterIndexStream);
		}
		else
		{
			targetClusterId = atol(row[0].c_str());
			prevId = row[1];
		}
	}
	
	idClusterStream.close();
	clusterIndexStream.close();
}

void JobDefinition::OutputClusters()
{
	ifstream clusterIndexStream("data/" + this->name + "_tmp.ndc", std::ios::binary);
	ifstream clustersStream("data/" + this->name + "_tmp");
	ofstream outputFileStream(this->outputFile.path);
	long clusterPosition;
	unsigned int clusterId;
	unsigned int prevClusterId;
	unsigned int nextClusterId;
	unsigned int clusterIdSeq = 1;	
	char inputDelimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);
	char outputDelimiter = Util::ConvertDelimiterToChar(this->outputFile.delimiter);
	vector<int> outputFieldsIndex = vector<int>(this->outputFile.dataDefinition.fields.size() + 1, -1);
	int clusterFieldIndex;
	int idFieldIndex = -1;

	for (int i = 0; i < this->inputFile.dataDefinition.fields.size(); i++)
	{
		if (this->inputFile.dataDefinition.fields[i].name == this->idField) {
			idFieldIndex = i + 1;
			break;
		}
	}

	for (int i = 0; i < this->outputFile.dataDefinition.fields.size(); i++)
	{
		string outputFieldName = this->outputFile.dataDefinition.fields[i].name;
		if (outputFieldName == "ClusterId")
			clusterFieldIndex = i;
		else {
			for (int j = 0; j < this->inputFile.dataDefinition.fields.size(); j++)
			{

				if (outputFieldName == this->inputFile.dataDefinition.fields[j].name) {
					outputFieldsIndex[i] = j+1;
					break;
				}
			}
		}
	}

	while (clusterIndexStream.read(reinterpret_cast<char*>(&clusterId), sizeof(unsigned int)))
	{
		clusterIndexStream.read(reinterpret_cast<char*>(&clusterPosition), sizeof(long));
		clusterIndexStream.read(reinterpret_cast<char*>(&prevClusterId), sizeof(unsigned int));
		clusterIndexStream.read(reinterpret_cast<char*>(&nextClusterId), sizeof(unsigned int));	
		if (prevClusterId == 0)
		{
			long lastPosition = clusterIndexStream.tellg();
			unordered_set<string> idSet;
			while(1)
			{
				clustersStream.seekg(clusterPosition);
				string line;
				while (std::getline(clustersStream, line))
				{
					vector<string> columns = StringUtil::Split(line, inputDelimiter);
					if (atol(columns[0].c_str()) == clusterId) {
						string idValue = columns[idFieldIndex];
						if (idSet.find(idValue) == idSet.end())
						{
							idSet.insert(idValue);
							for (int i = 0; i < this->outputFile.dataDefinition.fields.size(); i++)
							{
								if (i == clusterFieldIndex)
									outputFileStream << clusterIdSeq;
								else if (outputFieldsIndex[i] != -1)
									outputFileStream << columns[outputFieldsIndex[i]];

								if (i < this->outputFile.dataDefinition.fields.size() - 1)
									outputFileStream << outputDelimiter;
								else
									outputFileStream << endl;
							}
						}
					}
					else
						break;
				}
				if (nextClusterId != 0) {
					clusterIndexStream.seekg((nextClusterId - 1) * (sizeof(unsigned int) * 3 + sizeof(long)));
					clusterIndexStream.read(reinterpret_cast<char*>(&clusterId), sizeof(unsigned int));
					clusterIndexStream.read(reinterpret_cast<char*>(&clusterPosition), sizeof(long));
					clusterIndexStream.read(reinterpret_cast<char*>(&prevClusterId), sizeof(unsigned int));
					clusterIndexStream.read(reinterpret_cast<char*>(&nextClusterId), sizeof(unsigned int));
				}
				else
					break;
			}
			clusterIndexStream.seekg(lastPosition);
			clusterIdSeq++;
		}
	}

	clusterIndexStream.close();
	clustersStream.close();
	outputFileStream.close();
}


void JobDefinition::IndexTempClusters()
{
	unsigned int currentClusterId = 1;
	ifstream intputFileStream("data/" + this->name + "_tmp");
	ofstream outputFileStream("data/" + this->name + "_tmp.ndc", std::ios::binary);

	string line;
	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);
	unsigned int dummyClusterId = 0;
	unsigned int prevClusterId = dummyClusterId;
	long lastInputFilePosition = static_cast<long>(intputFileStream.tellg());
	while (std::getline(intputFileStream, line))
	{
		unsigned int clusterId = atol(line.substr(0, line.find(delimiter)).c_str());
		if (clusterId != prevClusterId)
		{
			long dummyPosition = 0;
			while (currentClusterId < clusterId)
			{
				outputFileStream.write(reinterpret_cast<const char*>(&currentClusterId), sizeof(unsigned int));
				outputFileStream.write(reinterpret_cast<const char*>(&dummyPosition), sizeof(long));
				outputFileStream.write(reinterpret_cast<const char*>(&dummyClusterId), sizeof(unsigned int));
				outputFileStream.write(reinterpret_cast<const char*>(&dummyClusterId), sizeof(unsigned int));

			}
			outputFileStream.write(reinterpret_cast<const char*>(&currentClusterId), sizeof(unsigned int));
			outputFileStream.write(reinterpret_cast<const char*>(&lastInputFilePosition), sizeof(long));
			outputFileStream.write(reinterpret_cast<const char*>(&dummyClusterId), sizeof(unsigned int));
			outputFileStream.write(reinterpret_cast<const char*>(&dummyClusterId), sizeof(unsigned int));
			currentClusterId++;
			prevClusterId = clusterId;
		}
		lastInputFilePosition = intputFileStream.tellg();
	}

	intputFileStream.close();
	outputFileStream.close();
}

void JobDefinition::GenerateTempClusters()
{
	
	
	string matchLevel;

	switch (this->matchLevel) {
		case MatchLevel::Strict: matchLevel = "Strict";break;
		case MatchLevel::Typical: matchLevel = "Typical";break;
		case MatchLevel::Loose: matchLevel = "Loose";break;
	}

	if (matchingAlgorithms == NULL)
		matchingAlgorithms = new MatchingAlgorithms;
	
	for (MatchingField matchingField : this->matchingDefinition->matchingFields)
		for (int i = 0; i < this->inputFile.dataDefinition.fields.size(); i++)
			if (matchingField.name == this->inputFile.dataDefinition.fields[i].name)
				matchingFields.push_back(pair<int, MatchingField>(i, matchingField));
	
	ifstream inputFileStream("data/" + this->name + ".ndx");
	ofstream outputFileStream("data/" + this->name + "_tmp");
	string prevKey = "";

	string line;
	int lineCount = 1;

	char inputDelimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);

	vector<int> keyFieldColumns;
	
	for (KeyField keyField : this->keyFields)
		for (int i = 0; i < this->inputFile.dataDefinition.fields.size(); i++)
			if (keyField.fieldName == this->inputFile.dataDefinition.fields[i].name)
				keyFieldColumns.push_back(i);

	vector<vector<string>> keyGroupLines;
	
	while (std::getline(inputFileStream, line))
	{
		vector<string> columns = StringUtil::Split(line, inputDelimiter);
		string currentKey = "";

		for (int keyFieldIndex : keyFieldColumns)
			currentKey += columns[keyFieldIndex];

		if (currentKey != prevKey) {
			ClusterKeyGroupLines(keyGroupLines, &outputFileStream, matchLevel);
			keyGroupLines.clear();
			prevKey = currentKey;
		}
		keyGroupLines.push_back(columns);
	}

	ClusterKeyGroupLines(keyGroupLines, &outputFileStream, matchLevel);

	inputFileStream.close();
	outputFileStream.close();

}

void JobDefinition::ClusterKeyGroupLines(const vector<vector<string>>& lines, ofstream* outputFileStream, const string& matchLevel)
{
	vector<unsigned int> clusterAssignments;
	map<unsigned int, vector<unsigned int>> clusters;
	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);
	for (unsigned int i = 0; i < lines.size(); i++) {
		clusterAssignments.push_back(i);
		clusters.insert(pair<unsigned int, vector<unsigned int>>(i, { i }));
	}
	vector<vector<unsigned int>> lineMatchScores = vector<vector<unsigned int>>(lines.size(), vector<unsigned int>(lines.size(), 0));
	if (lines.size() > 1) {
		for (unsigned int i = 0; i < lines.size() - 1;i++) {
			unsigned int targetCluster = clusterAssignments[i];
			for (unsigned int j = i + 1; j < lines.size(); j++) {
				int score = CalculateMatchScore(lines[i], lines[j], matchLevel);
				lineMatchScores[i][j] = score;
				lineMatchScores[j][i] = score;
				if (score >= this->scoreThreshold)
				{
					try {
						unsigned int sourceCluster = clusterAssignments[j];
						if (sourceCluster != targetCluster) {
							for (unsigned int p : clusters[sourceCluster])
							{
								clusterAssignments[p] = targetCluster;
								clusters[targetCluster].push_back(p);
							}
							clusters.erase(sourceCluster);
						}
					}
					catch (...)
					{
						cout << "Exception7 : ";
						exit(-1);
					}
				}
			}
		}
	}

	map<unsigned int, vector<unsigned int>> ::iterator it;

	for (it = clusters.begin(); it != clusters.end(); it++)
	{
		for (unsigned int clusterMember : it->second)
		{
			if (lines[clusterMember].size() > 0)
			{
				(*outputFileStream) << nextClusterId << delimiter;
				for (unsigned int i = 0; i < lines[clusterMember].size() - 1; i++)
					(*outputFileStream) << lines[clusterMember][i] << delimiter;
				(*outputFileStream) << lines[clusterMember][lines[clusterMember].size() - 1] << endl;
			}
		}
		nextClusterId++;
	}
}

int JobDefinition::CalculateMatchScore(const vector<string>& line1, const vector<string>& line2, const string& matchLevel)
{
	int totalScore = 0;
	int totalWeight = 0;

	for (pair<int, MatchingField> it : this->matchingFields)
	{
		int score;
		switch (it.second.type)
		{
		case MatchType::PersonName: score = matchingAlgorithms->PersonNameMatch(matchLevel, line1[it.first], line2[it.first]); break;
		case MatchType::OrganizationName: score = matchingAlgorithms->CompanyNameMatch(matchLevel, line1[it.first], line2[it.first], 2); break;
			//			case MatchType::Address: score = matchingAlgorithms->AddressMatch(matchLevel, line1[it.first], line2[it.first]); break;
		case MatchType::Exact: score = line1[it.first] == line2[it.first] ? 100 : 0; break;
		case MatchType::Text: {
			int distance = matchingAlgorithms->LevenshteinDistance(line1[it.first], line2[it.first]);
			score = distance * 100 / std::max(line1[it.first].length(), line2[it.first].length());
			break;
		}
		default: score = 0; break;
		}

		if (score < it.second.minSignificantScore)
			score = 0;

		if (score > it.second.noLowScore) {
			totalScore += score * it.second.weight;
			totalWeight += it.second.weight;
		}
	}

	return totalWeight == 0 ? 0 : totalScore / totalWeight;
}

void JobDefinition::RunExtract()
{

}

void JobDefinition::RunLoad()
{

}


void JobDefinition::GeneratePartitions()
{
	vector<int> keyFieldIndexes;
	vector<Field> dataDefinitionFields = this->inputFile.dataDefinition.fields;

	for (KeyField keyField : this->keyFields)
		for (int n = 0; n < dataDefinitionFields.size(); n++)
			if (keyField.fieldName == dataDefinitionFields[n].name)
				keyFieldIndexes.push_back(n);

	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);

	int partitionNumber = 1;

	ifstream inputFileStream(this->inputFile.path);
	string line;

	int partitionBytes = 0;
	int totalFieldCount = this->inputFile.dataDefinition.fields.size();
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
			ofstream outputFileStream("data/" + this->inputFile.name + "_tmp" + to_string(partitionNumber++));
			for (vector<string> item : tempBuffer)
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
		ofstream outputFileStream("data/" + this->inputFile.name + "_tmp" + to_string(partitionNumber));
		for (vector<string> item : tempBuffer)
		{
			string outputStr = StringUtil::Join(item, delimiter);
			outputFileStream << outputStr << "\n";
		}
		outputFileStream.close();
	}
}

void JobDefinition::GenerateSortedFile()
{
	string path = "data/";
	vector<string> partitionFilenames;
	for (const auto& entry : filesystem::directory_iterator(path))
	{
		string fileName = entry.path().filename().string();
		if (fileName.rfind(this->inputFile.name + "_tmp") == 0 && fileName != this->inputFile.name + "_tmp")
			partitionFilenames.push_back(fileName);
	}

	int partitionCount = partitionFilenames.size();

	long bufferSizeInBytes = this->memoryBufferSize * 1048576 / (partitionCount + 1);

	vector<list<vector<string>>> partitions = vector<list<vector<string>>>(partitionCount);
	vector<ifstream> partitionInputFileStreams = vector<ifstream>(partitionCount);
	ofstream outputFileStream("data/" + this->name + ".ndx");
	vector<MergeHeapNode> partitionHeadHeap;

	for (int i = 0; i < partitionCount; i++)
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
		pop_heap(partitionHeadHeap.begin(), partitionHeadHeap.end(), MergeHeapNodeCompare(*this));
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
			push_heap(partitionHeadHeap.begin(), partitionHeadHeap.end(), MergeHeapNodeCompare(*this));
		}
	}

	for (int i = 0; i < partitionCount; i++)
	{
		partitionInputFileStreams[i].close();
	}

	outputFileStream.close();
}


void JobDefinition::ReadPartition(list<vector<string>>& partition, ifstream& inputFileStream, long size)
{
	partition.clear();
	string line;

	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);
	int totalFieldCount = this->inputFile.dataDefinition.fields.size();
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

void JobDefinition::GenerateTempClusterPartitions()
{
	int IdFieldIndex;
	for (int i=0; i < this->inputFile.dataDefinition.fields.size(); i++)
	{
		if (this->inputFile.dataDefinition.fields[i].name == this->idField)
		{
			IdFieldIndex = i+1;
			break;
		}
	}
	int partitionNumber = 1;

	ifstream inputFileStream("data/" + this->name + "_tmp");
	string line;

	int partitionBytes = 0;
	int totalFieldCount = this->inputFile.dataDefinition.fields.size()+1;
	int lineCount = 1;
	char delimiter = Util::ConvertDelimiterToChar(this->inputFile.delimiter);

	int bufferSizeInBytes = this->memoryBufferSize * 1048576;
	forward_list<vector<string>> tempBuffer;
	while (std::getline(inputFileStream, line))
	{
		vector<string> columns = StringUtil::Split(line, delimiter);
		if (columns.size() != totalFieldCount) {
			fprintf(stderr, "Invalid number of columns in temp file during temp cluster file partitioning: Temp file line %d\n", lineCount);
			exit(EXIT_FAILURE);
		}
		vector<string> outputColumns = { columns[0], columns[IdFieldIndex] };

		tempBuffer.push_front(outputColumns);
		partitionBytes += ((columns[0].size() + columns[1].size()) * sizeof(char));
		lineCount++;
		if (partitionBytes >= bufferSizeInBytes)
		{
			tempBuffer.sort(TempClusterCompare());
			ofstream outputFileStream("data/" + this->name + "_id_tmp" + to_string(partitionNumber++));
			for (auto it = tempBuffer.begin(); it != tempBuffer.end(); it++)
			{
				string outputStr = (*it)[0] + "," + (*it)[1];
				outputFileStream << outputStr << "\n";
			}
			partitionBytes = 0;
			outputFileStream.close();
			tempBuffer.clear();
		}
	}
	inputFileStream.close();

	if (!tempBuffer.empty()) {
		tempBuffer.sort(TempClusterCompare());
		ofstream outputFileStream("data/" + this->name + "_id_tmp" + to_string(partitionNumber));
		for (auto it = tempBuffer.begin(); it != tempBuffer.end(); it++)
		{
			string outputStr = (*it)[0] + "," + (*it)[1];
			outputFileStream << outputStr << "\n";
		}
		outputFileStream.close();
	}
}

void JobDefinition::ReadTempClusterPartition(list<vector<string>>& partition, ifstream& inputFileStream, long size)
{
	partition.clear();
	string line;

	long partitionBytes = 0;

	while (partitionBytes <= size && std::getline(inputFileStream, line))
	{
		vector<string> columns = StringUtil::Split(line, ',');
		if (columns.size() != 2) {
			fprintf(stderr, "Invalid number of columns in temp cluster partition.");
			exit(EXIT_FAILURE);
		}
		partition.push_back(columns);
		partitionBytes += line.size() * sizeof(char);
	}
}

void JobDefinition::GenerateSortedTempClusters()
{
	string path = "data/";
	vector<string> partitionFilenames;
	for (const auto& entry : filesystem::directory_iterator(path))
	{
		string fileName = entry.path().filename().string();
		if (fileName.rfind(this->name + "_id_tmp") == 0 && fileName != this->name + "_id_tmp")
			partitionFilenames.push_back(fileName);
	}

	int partitionCount = partitionFilenames.size();

	long bufferSizeInBytes = this->memoryBufferSize * 1048576 / (partitionCount + 1);

	vector<list<vector<string>>> partitions = vector<list<vector<string>>>(partitionCount);
	vector<ifstream> partitionInputFileStreams = vector<ifstream>(partitionCount);
	ofstream outputFileStream("data/" + this->name + "_id_tmp");
	vector<MergeHeapNode> partitionHeadHeap;

	for (int i = 0; i < partitionCount; i++)
	{
		partitionInputFileStreams[i] = ifstream("data/" + partitionFilenames[i]);
		ReadTempClusterPartition(partitions[i], partitionInputFileStreams[i], bufferSizeInBytes);
		MergeHeapNode node(partitions[i].front(), i);
		partitionHeadHeap.push_back(node);
		partitions[i].pop_front();
	}

	make_heap(partitionHeadHeap.begin(), partitionHeadHeap.end(), MergeHeapNodeCompare(*this));

	int writtenLines = 0;
	while (!partitionHeadHeap.empty())
	{
		pop_heap(partitionHeadHeap.begin(), partitionHeadHeap.end(), MergeHeapNodeCompare(*this));
		MergeHeapNode head = partitionHeadHeap.back();
		partitionHeadHeap.pop_back();
		string outputStr = StringUtil::Join(head.data, ',');
		outputFileStream << outputStr << "\n";
		writtenLines++;

		if (partitions[head.partitionIndex].empty())
			ReadTempClusterPartition(partitions[head.partitionIndex], partitionInputFileStreams[head.partitionIndex], bufferSizeInBytes);

		if (!partitions[head.partitionIndex].empty())
		{
			vector<string> nextRow = partitions[head.partitionIndex].front();
			partitions[head.partitionIndex].pop_front();
			partitionHeadHeap.push_back(MergeHeapNode(nextRow, head.partitionIndex));
			push_heap(partitionHeadHeap.begin(), partitionHeadHeap.end(), MergeHeapNodeCompare(*this));
		}
	}

	for (int i = 0; i < partitionCount; i++)
	{
		partitionInputFileStreams[i].close();
	}

	outputFileStream.close();
}
