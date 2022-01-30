#include <iostream>
#include <string-util.h>
#include <matching-algorithms.h>
#include <dictionary-txt.h>
#include <key-generator.h>
#include <word-frequency-definition.h>
#include <string>
#include <damerau-levenshtein-distance.h>


int main() {		
	MatchingAlgorithms matchingAlgorithms;
	KeyGenerator keyGenerator;
	vector<string> keys1 = keyGenerator.GeneratePersonNameKeys("Typical", "Apdullah Korkut");
	vector<string> keys2 = keyGenerator.GeneratePersonNameSearchKeys("Typical", "Abtullah Korkut");
	int score = matchingAlgorithms.PersonNameMatch("Typical", "Apdullah Korkut", "Abtullah Korkut");
	cout << score << endl;
	return 0;
	
	/*	KeyGenerator keyGenerator;
	vector<set<string>> keys = keyGenerator.GeneratePersonNameSearchKeys("ABDURRAHIM KAPLAN");

	DictionaryTxt dictionary;
	for (WordFrequencyDefinition def : dictionary.GetSurnameFrequencyList())
	{
		cout << def.GetKey() << " - " << keyGenerator.GetSurnameWordKeys(def.GetKey())[0] << endl;
	}

*/
	return 0;

}