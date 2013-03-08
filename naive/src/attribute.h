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

#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_
#include <string>
#include <vector>
using namespace std;

class Attribute
{
  public:
    int id;                         //attribute id
    unsigned int totalHamSequence;  //times attribute appears in ham texts
    unsigned int totalSpamSequence; //times attribute appears in spam texts
    int spamTotalCarrier;           //number of Spam messages that contain the attribute
    int hamTotalCarrier;            //number of Ham messages that contain the attribute
    //name of the file that we last saw this attribute

    double IG;        //Information Gain of the attribute
    double Mham;      //statistic mean of attribute in ham messages
    double Mspam;     //statistic mean of attribute in spam messages
    double Sham;      //Typical deviation of attribute in ham messages
    double Sspam;     //Typical deviation of attribute in spam messages
    double P_Graham;  //This "possibility is only used for Paul Graham's spam filter"

    //vector that holds the frequencies of this attribute for each ham Message
    //vector<int> hamFrequencies;
    //vector that holds the normalized frequencies of this attribute for each ham Message
    //vector<double> hamFrequenciesNorm;
    //vector that holds the TFIDF frequencies of this attribute for each ham Message
    //vector<double> hamFrequenciesTFIDF;

    //vector that holds the frequencies of this attribute for each spam Message
    //vector<int> spamFrequencies;
    //vector that holds the normalized frequencies of this attribute for each spam Message
    //vector<double> spamFrequenciesNorm;
    //vector that holds the TFIDF frequencies of this attribute for each spam Message
    //vector<double> spamFrequenciesTFIDF;

    //attribute used for classification
    //bool active;
    bool seenBefore;

    bool removed;
    Attribute(int id);
    Attribute(int id, unsigned int totalHamSequence,unsigned int  totalSpamSequence, int spamTotalCarrier, int hamTotalCarrier);
    Attribute();
    ~Attribute();
};

#endif
