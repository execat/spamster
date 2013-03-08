/*************************************************************************
		This file is part of spamster.

		spamster is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		spamster is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with spamster.  If not, see <http://www.gnu.org/licenses/>.

		The entire source can be found at https://github.com/execat/spamster
*************************************************************************/

#ifndef _EMAILPARSER_H_
#define _EMAILPARSER_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "base64.h"

using namespace std;

class EmailParser
{
	private:
		bool base64;
		bool quotedP;
		bool inTag;
		bool inScript;

		void parseHeader(stringstream& fout, ifstream& fin);
		void parseText(stringstream& fout, ifstream& fin);
		bool parseInfo(stringstream& fout, ifstream& fin);
		void writeText(stringstream& fout, string& line);
		bool isNumber(char ch);
		bool isDex(char ch);
		void removeHTML(stringstream& fout, string& line);
		string& replaceAll(string& context, const string& from, const string& to);

	public:
		EmailParser(string inputMessageName, stringstream& finalStream, bool parserOn);
		~EmailParser();
};
#endif
