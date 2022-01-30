/*
 * phrase.cpp
 *
 *  Created on: 6 Kas 2021
 *      Author: koray.korkut
 */

#include "phrase.h"
#include <string-util.h>

Phrase::Phrase()
{

}

vector<string> Phrase::GetKeyWordSequence()
{
	return StringUtil::Split(text, ' ');
}

const string& Phrase::GetBaseKey() const {
	return baseKey;
}

void Phrase::SetBaseKey(const string &baseKey = "") {
	this->baseKey = baseKey;
}

const string& Phrase::GetCategory() const {
	return category;
}

void Phrase::SetCategory(const string &category) {
	this->category = category;
}

char Phrase::GetFrequency() const {
	return frequency;
}

void Phrase::SetFrequency(char frequency) {
	this->frequency = frequency;
}

const string& Phrase::GetKey() const {
	return key;
}

void Phrase::SetKey(const string &key = "") {
	this->key = key;
}

int Phrase::GetSeq() const {
	return seq;
}

void Phrase::SetSeq(int seq) {
	this->seq = seq;
}

const string& Phrase::GetText() const {
	return text;
}

void Phrase::SetText(const string &text) {
	this->text = text;
}

bool Phrase::IsTurkishCharacterTolerance() const {
	return turkishCharacterTolerance;
}

void Phrase::SetTurkishCharacterTolerance(bool turkishCharacterTolerance) {
	this->turkishCharacterTolerance = turkishCharacterTolerance;
}
