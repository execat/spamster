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

#include "spamFilterPaulGraham.h"
SpamFilterPaulGraham::SpamFilterPaulGraham (AttributeCollection& ACinput, int SOHM, int SOSM, double delta,
  map<string, int>& coder, bool integerTokens, stringstream& fin)
{
  AC = ACinput;
  sumOfHamMessages = SOHM;
  sumOfSpamMessages = SOSM;
  sumOfMessages = SOHM + SOSM;
  Delta = delta;
  //ifstream fin ("pmail");
  map<int, double>::iterator iter;
  if (integerTokens)
  {
    int token;
    while (fin >> token)
    {
      iter = F.find(token);
      Attribute att;
      if (iter == F.end())
      {
        if (AC.getAttribute(token,att))
        {
          double p = att.P_Graham;
          if (p == -1 ) F.insert(make_pair(token,0.4));
          else          F.insert(make_pair(token,p));
        }
        else            F.insert(make_pair(token,0.4));
      }
    }
  }
  else
  {
    int token;
    string tokenStr;
    while (fin >> tokenStr)
    {
      if (coder.find(tokenStr) == coder.end())  token = 0;
      else                                      token = coder[tokenStr];

      iter = F.find(token);
      Attribute att;
      if (iter == F.end())
      {
        if (AC.getAttribute(token,att))
        {
          double p = att.P_Graham;
          cout << p << endl;
          if (p == -1 ) F.insert(make_pair(token,0.4));
          else          F.insert(make_pair(token,p));
        }
        else            F.insert(make_pair(token,0.4));
      }
    }
  }
  //fin.close();
}

SpamFilterPaulGraham::~SpamFilterPaulGraham() {}

void SpamFilterPaulGraham::printF()
{
  map<int, double>::iterator iter = F.begin();
  int f;
  double s;
  for(int i = 0; i < F.size(); i++)
  {
    f = (*iter).first; s = (*iter).second;
    cout << f << " " << s << endl;
    iter++;
  }
  string t;
}

bool SpamFilterPaulGraham::spamIdentifier(int isSpam, int k, double& p)
{
  int numberOfBestAtt;
  if(k <= F.size())   numberOfBestAtt = k;
  else                numberOfBestAtt = F.size();

  bool* Best = new bool[F.size()];
  double* Final = new double[numberOfBestAtt];
  for (int i = 0; i < F.size(); i++)
    Best[i] = false;

  for (int i = 0; i < numberOfBestAtt; i++)
  {
    double max = 0;
    int pointer = -1;
    int id;
    map<int,double>::iterator iter = F.begin();
    for (int j = 0; j < F.size(); j++)
    {
      double distance = (*iter).second;
      distance = fabs(0.5 - distance);
      if (Best[j] == false)
      {
        if (distance >= max)
        {
          max = distance;
          pointer = j;
          id = (*iter).first;
        }
      }
      iter++;
    }
    Best[pointer] = true;
    Final[i] = F[id];
  }
  double Pt1 = 1.0;
  double Pt2 = 1.0;
  for (int i = 0; i < numberOfBestAtt; i++)
  {
    Pt1 = Pt1 * Final[i];
    Pt2 = Pt2 * (1.0 - Final[i]);
  }
  p = Pt1 / (Pt1 + Pt2);
  delete []Best;
  delete []Final;

  if (p > Delta)  return true;
  else            return false;
}