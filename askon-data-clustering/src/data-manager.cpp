/*
 * data-manager.cpp
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <windows.h>
#include <locale>
#include "process.h"


using namespace std;

int main(int argc, char *argv[]){

	//TestHeap2();
	//TestHeap();
	//TestSort();
	//return 0;


	if (argc < 2) {
		cout << "usage: datamanager [command] [parameter1,parameter2, ...]";
		return -1;
	}

	if (string(argv[1]) == "run")
	{
		string filename;
		if (argc <=2)
			filename = "process.yaml";
		else {
			filename = string(argv[2]) + ".yaml";
		}

		Process process(filename);
		process.dictionary = new DictionaryTxt;
		process.Run();
	}

	return 0;
}
