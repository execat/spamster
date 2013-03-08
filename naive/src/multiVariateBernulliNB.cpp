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

#include "multiVariateBernulliNB.h"
MultiVariateBernulliNB::MultiVariateBernulliNB(AttributeCollection& ACinput, int SOHM, int SOSM, double delta,
  map<string, int>& coder, bool integerTokens, int NGrams, stringstream& fin)
{
  AC = ACinput;
  sumOfHamMessages = SOHM;
  sumOfSpamMessages = SOSM;
  sumOfMessages = SOHM + SOSM;
  Delta = delta;
  //ifstream fin ("pmail");
  F = new bool[AC.numberOfAttributes()];
  for (int i=0;i<AC.numberOfAttributes();i++)
    F[i] = false;

  if (integerTokens)
  {
    int token;
    while (fin >> token)
    {
      for (int i=0;i<AC.numberOfAttributes();i++)
      {
        if(token == AC.getAttributeAt(i).id)
        {
          F[i] = true;
          break;
        }
      }
    }
  }
  else
  {
    int token;
    string tokenStr;
    if (NGrams <= 0)
    {
      while (fin >> tokenStr)
      {
        if (tokenStr.size() >= 100)               tokenStr = tokenStr.substr(0,100);
        if (coder.find(tokenStr) == coder.end())  token = 0;
        else                                      token = coder[tokenStr];

        for (int i = 0; i < AC.numberOfAttributes(); i++)
        {
          if(token == AC.getAttributeAt(i).id)
          {
            F[i] = true;
            break;
          }
        }
      }
    }
  }
  //fin.close();
}

MultiVariateBernulliNB::~MultiVariateBernulliNB()
{
  delete [] F;
}

void MultiVariateBernulliNB::printF()
{
  for(int i = 0; i < AC.numberOfAttributes(); i++)
    cout << F[i] << " ";
  cout << endl;
}

bool MultiVariateBernulliNB::spamIdentifier(int isSpam, double& Pham, double& Pspam)
{
  double Pcs = ((double)sumOfSpamMessages) / sumOfMessages;
  Pcs = log10(Pcs);
  double Pch = ((double)sumOfHamMessages) / sumOfMessages;
  Pch = log10(Pch);

  double Pxcs = 0;
  for (int i = 0; i < AC.numberOfAttributes(); i++)
  {
    double Ptc;
    if (F[i] == false)
    {
      Ptc = ((double)(1 + AC.getAttributeAt(i).spamTotalCarrier)) / (sumOfSpamMessages+2);
      Ptc = 1.0 - Ptc;

      if (Ptc != 0)   Ptc = log10(Ptc);
      else            Ptc = log10(0.00001);
      //Ptc = log10(Ptc);
    }
    else
    {
      Ptc = ((double)(1 + AC.getAttributeAt(i).spamTotalCarrier)) / (sumOfSpamMessages+2);

      if (Ptc != 0)   Ptc = log10(Ptc);
      else            Ptc = log10(0.00001);
      //Ptc = log10(Ptc);
    }
    Pxcs += Ptc;
  }
  double Pxch = 0;
  for (int i = 0; i < AC.numberOfAttributes(); i++)
  {
    double Ptc;
    if (F[i] == false)
    {
      Ptc = ((double)(1 + AC.getAttributeAt(i).hamTotalCarrier)) / (sumOfHamMessages+2);
      Ptc = 1.0 - Ptc;

      if (Ptc != 0)   Ptc = log10(Ptc);
      else            Ptc = log10(0.00001);
    }
    else
    {
      Ptc = ((double)(1 + AC.getAttributeAt(i).hamTotalCarrier)) /  (sumOfHamMessages+2);

      if (Ptc != 0)   Ptc = log10(Ptc);
      else            Ptc = log10(0.00001);
    }
    Pxch += Ptc;
  }
  Pspam =  Pcs + Pxcs;
  Pham = Pch + Pxch;

  if ( Pspam - Pham > Delta)  return true;
  else                        return false;
}