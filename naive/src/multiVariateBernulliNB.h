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

#ifndef _MULTIVARIATEBERNULLINB_H_
#define _MULTIVARIATEBERNULLINB_H_

#include <iostream>
#include <fstream>
#include "attribute.h"
#include "attributeCollection.h"

class MultiVariateBernulliNB
{
  private:
    AttributeCollection AC;
    bool* F;
    int sumOfHamMessages;
    int sumOfSpamMessages;
    int sumOfMessages;
    double Delta;
  public:
    MultiVariateBernulliNB (AttributeCollection& ACinput, int SOHM, int SOSM, double delta,
      map<string, int>& coder, bool integerTokens, int NGrams, stringstream& fin);
    ~MultiVariateBernulliNB ();
    bool spamIdentifier(int isSpam, double& Pham, double& Pspam);
    void printF();
};
#endif
