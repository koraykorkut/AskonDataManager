/*
 * merge-heap-node.h
 *
 *  Created on: 4 Kas 2021
 *      Author: koray.korkut
 */

#ifndef MERGE_HEAP_NODE_H_
#define MERGE_HEAP_NODE_H_

#include <vector>
#include <string>

using namespace std;

class MergeHeapNode {
public:
	MergeHeapNode(vector<string> &data, int partitionIndex);
	vector<string> data;
	int partitionIndex;
};

#endif /* MERGE_HEAP_NODE_H_ */
