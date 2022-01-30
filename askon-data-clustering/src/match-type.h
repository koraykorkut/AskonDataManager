/*
 * match-type.h
 *
 *  Created on: 30 Eki 2021
 *      Author: koray.korkut
 */

#ifndef SRC_MATCH_TYPE_H_
#define SRC_MATCH_TYPE_H_

enum class MatchType {
	Exact,
	Text,
	PersonName,
	OrganizationName,
	Address,
	Date
};

#endif /* SRC_MATCH_TYPE_H_ */
