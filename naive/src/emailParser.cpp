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

#include "emailParser.h"
EmailParser::EmailParser(string inputMessageName, stringstream& finalStream, bool parserOn)
{
	inTag = false;
	inScript = false;
	stringstream premail;
	ifstream firstFin(inputMessageName.c_str());
	base64 = false;
	quotedP = false;

	if (parserOn == true)
	{
		parseHeader(premail, firstFin);
		parseText (premail, firstFin);

		firstFin.close();

		string line;
		while (getline(premail, line))
			removeHTML(finalStream, line);
	}
 	else
	{
		string line;
		while (getline(firstFin, line))
			finalStream << line << endl;
	}
}

EmailParser::~EmailParser () {}

void EmailParser::parseHeader(stringstream& fout, ifstream& fin)
{
	string line = "";
	while (getline(fin,line))
	{
		if (line == "" || line == "\r")
			break;
		else
		{
			if (line.substr(0,8) == "Subject:")
				fout << line.substr(8) << endl;
			else if (line == "Content-Transfer-Encoding: base64")
				base64 = true;
			else if (line == "Content-Transfer-Encoding: quoted-printable")
				quotedP = true;
		}
	}
}

void EmailParser::parseText (stringstream& fout, ifstream& fin)
{
	string line = "";
	while (getline(fin,line))
	{
		if (line != "" && line != "\r")
		{
			if (line.substr(0, 8) == "------=_")
			{
				if (parseInfo(fout, fin) ==  false)
						break;
			}
			else if (line.substr(0, 2) == "--" && line.size() > 10)
			{
				char ch [5];
				for (int i = 0; i < 5; i++)
					ch[i] = line[i+2];

				if (ch[0] == '-' && isNumber(ch[1]) && isNumber(ch[2]) && isNumber(ch[3]) && isNumber(ch[4]))
					if (parseInfo(fout, fin) ==  false)
						break;
				else if (isDex(ch[0]) && isDex(ch[1]) && isDex(ch[2]) && isDex(ch[3]) && isDex(ch[4]))
					if (parseInfo(fout, fin) ==  false)
						break;
				else
					writeText(fout,line);
			}
			else
				writeText(fout,line);
		}
	}
}
bool EmailParser::parseInfo(stringstream& fout, ifstream& fin)
{
	string line = "";
	bool encoding = false;

	while (getline(fin,line))
	{
		if (line == "" || line == "\r")
		{
			if (encoding == false)
			{
				base64 = false;
				encoding = false;
			}
			return true;
		}
		else if (line.substr(0,13) == "Content-Type:")
		{
			if (line.substr(14,4) != "text" && line.substr(14,9) != "multipart")
				return false;
		}
		else if (line =="Content-Transfer-Encoding: base64")
		{
			base64 = true;
			quotedP = false;
			encoding = true;
		}
		else if (line == "Content-Transfer-Encoding: quoted-printable")
		{
			quotedP = true;
			base64 = false;
			encoding = true;
		}
	}
}

void EmailParser::writeText (stringstream& fout, string& line)
{
	if (base64) 			fout << base64_decode(line) << endl;
	else if (quotedP) fout << QPdecode(line) << endl;
	else 							fout << line << endl;
}

bool EmailParser::isNumber(char ch)
{
	if (ch >= 48 && ch <= 57)	return true;
	else 											return false;
}

bool EmailParser::isDex(char ch)
{
	if ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 70) || (ch >= 97 && ch <= 102))
		return true;
	else
		return false;
}

void EmailParser::removeHTML(stringstream& fout, string& line)
{
	string s = line;
	bool tagDone = false;
	bool scriptDone = false;
	while(tagDone == false || scriptDone == false)
	{
		if (inScript)
		{
			size_t rightPos = s.find("</SCRIPT>");
			if (rightPos == string::npos)
				rightPos = s.find("</script>");
			if(rightPos != string::npos)
			{
				inScript = false;
				s.erase(0, rightPos + 9);
			}
			else
			{
				scriptDone = true;
				tagDone = true;
				s.erase();
			}
		}
		else
		{
			// Look for start of tag:
			size_t leftPos = s.find("<SCRIPT");
			if (leftPos == string::npos)
				leftPos = s.find("<script");
			if(leftPos != string::npos)
			{
				inScript = scriptDone = true;
				s.erase(leftPos);
			}
			else
			{
				scriptDone = true;
				if(inTag)
				{
					// The previous line started an HTML tag
					// but didn't finish. Must search for '>'.
					size_t rightPos2 = s.find('>');
					if(rightPos2 != string::npos)
					{
						inTag = false;
						s.erase(0, rightPos2 + 1);
					}
					else
					{
						tagDone = true;
						s.erase();
					}
				}
				else
				{
					// Look for start of tag:
					size_t leftPos2 = s.find('<');
					if(leftPos2 != string::npos)
					{
						// See if tag close is in this line:
						size_t rightPos2 = s.find('>',leftPos2);
						if(rightPos2 == string::npos)
						{
							inTag = tagDone = true;
							s.erase(leftPos2);
						}
						else
							s.erase(leftPos2, rightPos2 - leftPos2 + 1);
					}
					else
						tagDone = true;
				}
			}
		}
	}
	// Remove all special HTML characters
	replaceAll(s, "&lt;", "<");
	replaceAll(s, "&gt;", ">");
	replaceAll(s, "&amp;", "&");
	replaceAll(s, "&nbsp;", " ");

	// etc...
	if (s != "" && s != "\r")
		fout << s << endl;
}

string& EmailParser::replaceAll(string& context, const string& from, const string& to)
{
	size_t lookHere = 0;
	size_t foundHere;
	while((foundHere = context.find(from, lookHere)) != string::npos)
	{
		context.replace(foundHere, from.size(), to);
		lookHere = foundHere + to.size();
	}
	return context;
}
