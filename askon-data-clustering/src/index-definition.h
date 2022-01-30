/*
 * index-definition.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_INDEX_DEFINITION_H_
#define SRC_INDEX_DEFINITION_H_

#include <string>
#include <vector>
#include <list>
#include "file-definition.h"
#include "key-field.h"
#include "merge-heap-node.h"

using namespace std;

class IndexDefinition {
	   struct RowCompare
	   {
	       RowCompare( const IndexDefinition& info ) : m_info(info) { }
	       const IndexDefinition& m_info;

	       bool operator()(const vector<string> &rowA, const vector<string> &rowB)
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
		   MergeHeapNodeCompare( const IndexDefinition& info ) : m_info(info) { }
	       const IndexDefinition& m_info;

	       bool operator()(const MergeHeapNode &nodeA, const MergeHeapNode &nodeB)
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


	public:
		IndexDefinition();
		string name;
		FileDefinition inputFile;
		vector<KeyField> keyFields;
		vector<string> includedFields;
		void Build();
		int memoryBufferSize = 50;
	private:
		void ReadPartition(list<vector<string>> &partition, ifstream &inputFileStream, long size);
		void GenerateTempFile();
		void GeneratePartitions();
		void GenerateSortedFile();
};

#endif /* SRC_INDEX_DEFINITION_H_ */
