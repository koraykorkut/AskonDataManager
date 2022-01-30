/*
 * util.cpp
 *
 *  Created on: 5 Kas 2021
 *      Author: koray.korkut
 */

#include "util.h"

Util::Util() {
	// TODO Auto-generated constructor stub

}


char Util::ConvertDelimiterToChar(Delimiter delimiter)
{
	switch(delimiter)
		{
			case Delimiter::Comma: return ','; break;
			case Delimiter::Tab: return '\t'; break;
			case Delimiter::Space: return ' '; break;
			case Delimiter::Pipe: return '|'; break;
			default: return ' ';break;
		}
}

