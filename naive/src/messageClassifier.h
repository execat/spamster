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

#ifndef _MESSAGECLASSIFIER_H_
#define _MESSAGECLASSIFIER_H_
#include <string>
#include <vector>
#include <sstream>
#include "attribute.h"
#include "attributeCollection.h"
#include "multiVariateBernulliNB.h"
#include "multiNomialTF.h"
#include "multiNomialBool.h"
#include "spamFilterPaulGraham.h"

using namespace std;

class MessageClassifier
{
  public:
    AttributeCollection* attributes;
    double Delta;
    bool integerTokens;
    int NGram;
    int version;
    int k;
    MessageClassifier(AttributeCollection* att, double D, bool iTokens, int NG, string V, int K);
    MessageClassifier();
    int Classifier(map<string, int>& coder, int isSpam, double& Pham, double& Pspam, int sumOfHamMessages,
      int sumOfSpamMessages, stringstream& fin);
    void Statistics(int ham, int spam);
    ~MessageClassifier();
};

#endif
