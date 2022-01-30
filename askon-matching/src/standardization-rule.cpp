/*
 * standardization-rule.cpp
 *
 *  Created on: 7 Kas 2021
 *      Author: koray.korkut
 */

#include "standardization-rule.h"

StandardizationRule::StandardizationRule() {
	// TODO Auto-generated constructor stub
}


StandardizationRule::StandardizationRule(string textToFind,string replaceWith,bool matchTurkishCharacters,bool beforeDelimited,bool afterDelimited)
{
	this->textToFind = textToFind;
	this->replaceWith = replaceWith;
	this->matchTurkishCharacters = matchTurkishCharacters;
	this->beforeDelimited = beforeDelimited;
	this->afterDelimited = afterDelimited;
	this->key = textToFind;
}


bool StandardizationRule::IsAfterDelimited() const {
	return afterDelimited;
}

void StandardizationRule::SetAfterDelimited(const bool afterDelimited) {
	this->afterDelimited = afterDelimited;
}

bool StandardizationRule::IsBeforeDelimited() const {
	return beforeDelimited;
}

void StandardizationRule::SetBeforeDelimited(const bool beforeDelimited) {
	this->beforeDelimited = beforeDelimited;
}

const string& StandardizationRule::GetKey() const {
	return this->key;
}

void StandardizationRule::SetKey(const string &key = "") {
	this->key = key;
}

bool StandardizationRule::IsMatchTurkishCharacters() const {
	return matchTurkishCharacters;
}

void StandardizationRule::SetMatchTurkishCharacters(const bool matchTurkishCharacters) {
	this->matchTurkishCharacters = matchTurkishCharacters;
}

const string& StandardizationRule::GetReplaceWith() const {
	return this->replaceWith;
}

void StandardizationRule::SetReplaceWith(const string &replaceWith) {
	this->replaceWith = replaceWith;
}

int StandardizationRule::GetSeq() const {
	return this->seq;
}

void StandardizationRule::SetSeq(int seq) {
	this->seq = seq;
}

const string& StandardizationRule::GetTextToFind() const {
	return textToFind;
}

void StandardizationRule::SetTextToFind(const string &textToFind) {
	this->textToFind = textToFind;
}
