/*
 * job-definition.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_JOB_DEFINITION_H_
#define SRC_JOB_DEFINITION_H_

#include <string>
#include <vector>
#include <key-generator.h>
#include <matching-algorithms.h>
#include "job-type.h"
#include "index-definition.h"
#include "matching-definition.h"
#include "match-level.h"
#include "transformation-definition.h"

using namespace std;

class JobDefinition {
public:
	JobDefinition();
	string name;
	JobType type;
	FileDefinition inputFile;
	FileDefinition outputFile;
	vector<KeyField> keyFields;
	int inputLimit = 0;
	int scoreThreshold = 100;
	string idField;
	MatchingDefinition *matchingDefinition;
	MatchLevel matchLevel;
	TransformationDefinition *transformationDefinition;
	void Run();
	void Resume();
private:
	KeyGenerator* keyGenerator = NULL;
	MatchingAlgorithms * matchingAlgorithms = NULL;
	int nextClusterId = 1;
	void RunTransform();
	void RunCluster();
	void RunExtract();
	void RunLoad();
	vector<pair<int, MatchingField>> matchingFields;


	struct TempClusterCompare
	{
		bool operator()(const vector<string> rowA, vector<string> rowB)
		{
			return atol(rowA[1].c_str()) < atol(rowB[1].c_str());
		}
	};

	struct RowCompare
	{
		RowCompare(const JobDefinition& info) : m_info(info) { }
		const JobDefinition& m_info;

		bool operator()(const vector<string>& rowA, const vector<string>& rowB)
		{
			for (int i = 0; i < m_info.keyFields.size(); i++)
			{
				KeyField keyField = m_info.keyFields[i];
				if (keyField.sortOrder == SortOrder::Asc)
				{
					if (rowA[i] < rowB[i])
						return true;
					else if (rowA[i] > rowB[i])
						return false;
				}
				else
				{
					if (rowA[i] > rowB[i])
						return true;
					else if (rowA[i] < rowB[i])
						return false;
				}
			}

			return false;
		}
	};


	struct MergeHeapNodeCompare
	{
		MergeHeapNodeCompare(const JobDefinition& info) : m_info(info) { }
		const JobDefinition& m_info;

		bool operator()(const MergeHeapNode& nodeA, const MergeHeapNode& nodeB)
		{
			for (int i = 0; i < m_info.keyFields.size(); i++)
			{
				KeyField keyField = m_info.keyFields[i];
				if (keyField.sortOrder == SortOrder::Asc)
				{
					if (nodeA.data[i] > nodeB.data[i])
						return true;
					if (nodeA.data[i] < nodeB.data[i])
						return false;
				}
				else
				{
					if (nodeA.data[i] < nodeB.data[i])
						return true;
					if (nodeA.data[i] > nodeB.data[i])
						return false;
				}
			}

			return false;
		}
	};

	int memoryBufferSize = 50;
	void ReadPartition(list<vector<string>>& partition, ifstream& inputFileStream, long size);
	void GeneratePartitions();
	void GenerateSortedFile();
	void GenerateTempClusters();
	void IndexTempClusters();
	void GenerateTempClusterPartitions();
	void GenerateSortedTempClusters();
	void LinkClusters();
	void OutputClusters();
	void ReadTempClusterPartition(list<vector<string>>& partition, ifstream& inputFileStream, long size);
	void ClusterKeyGroupLines(const vector<vector<string>>& lines, ofstream* outputFileStream, const string& matchLevel);
	int CalculateMatchScore(const vector<string>& line1, const vector<string>& line2, const string& matchLevel);
};

#endif /* SRC_JOB_DEFINITION_H_ */
