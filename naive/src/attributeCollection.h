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

#ifndef _ATTRIBUTECOLLECTION_H_
#define _ATTRIBUTECOLLECTION_H_

#include <map>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "attribute.h"

class AttributeCollection
{
	private:
		map<int,Attribute> collection;      //A map with all atrributes
		multimap<double, int> collectionIG;	//A multimap of IG and id

		map<int, Attribute>::iterator pointerIterator;         //A map iterator that remembers the last seen Attribute
		multimap<double, int>::iterator pointerIGIdIterator;   //A map iterator that remembers the last seen Attribute

		int lastSeenAttribute;  //integer that remembers the last aksed Attribute with the getAttributeAt(int i) function

		Attribute* attributePointer;

	public:
		int size;                   //size of map
		int spamAttributesCounter;  //counts the number of different attributes that at appear at spam messages
		int hamAttributesCounter;   //counts the number of different attributes that at appear at ham messages
		bool collectionIGReady;     //indicates that collectionIG multimap is ready for use;

		AttributeCollection ();
		~AttributeCollection();
		Attribute& getAttributeAt(int i,bool& flag);
		Attribute& getAttributeAt(int i);
		void reset();
		bool getAttribute(int id,Attribute& att);
		void removeAttributes(vector<int> att_tobe_deleted);
		void addAttribute (int id,bool spam,int sumOfTokens);

		/*
		void copyAttribute (int id, unsigned int totalHamSequence, unsigned int totalSpamSequence, int spamTotalCarrier,
		int hamTotalCarrier, double IG, vector<int>& hamFrequencies, vector<int>& spamFrequencies,
		vector<double>& hamFrequenciesNorm, vector<double>& spamFrequenciesNorm, vector<double>& hamFrequenciesTFIDF,
		vector<double>& spamFrequenciesTFIDF);

		void computeStatisticVariables(int sumOfHamMessages, int sumOfSpamMessages, bool normalized);
		*/
		void addAttributeIGId (double IG, int id);
		void computeP_Graham(int sumOfHamMessages, int sumOfSpamMessages);
		int numberOfAttributes();
		void fixSize(int m);
		void printAttributes();
		void clear();
		void getAttributesFromFile(string nameOffile, map<string, int>& coder, int& sumOfHamMessages,
				int& sumOfSpamMessages, int& lastUsedCode);
		void writeAttributesToFile(string nameOffile, map<string, int>& coder, int sumOfHamMessages,
				int sumOfSpamMessages, int last_used_code);
		vector<string> getTokens(const string& var, const string& sSeparators);

		void printInfo()
		{
			cout << this->numberOfAttributes() << "BIG = "  << collection.size() << "AIG = " << collectionIG.size() << endl;
		}
};

#endif
