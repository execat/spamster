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

#include "attributeCollection.h"

AttributeCollection::AttributeCollection ()
{
  collection.clear();
  collectionIG.clear();
  size = 0;
  spamAttributesCounter = 0;
  hamAttributesCounter = 0;
  lastSeenAttribute = -1;
  collectionIGReady = false;
  pointerIterator = collection.end();
  pointerIGIdIterator = collectionIG.end();
}

AttributeCollection::~AttributeCollection()
{
  clear();
}

void AttributeCollection::reset()
{
  size = collection.size();
  collectionIGReady = false;
  collectionIG.clear();
  pointerIterator = collection.end();
  pointerIGIdIterator = collectionIG.end();

  map<int, Attribute>::iterator pointerTempIterator = collection.begin();
  for (int i = 0; i < size; i++)
  {
    (*pointerTempIterator).second.seenBefore = false;
    (*pointerTempIterator).second.removed = false;
    pointerTempIterator ++;
  }
}
Attribute& AttributeCollection::getAttributeAt(int i, bool& flag)
{
  if (collectionIGReady)
  {
    if (lastSeenAttribute != -1)
    {
      if (i == lastSeenAttribute + 1)
      {
        lastSeenAttribute++;
        pointerIGIdIterator++;
      }
      else if (i != lastSeenAttribute)
      {
        pointerIGIdIterator = collectionIG.begin();
        lastSeenAttribute = 0;
        for(int x = 0; x < i; x++)
        {
          pointerIGIdIterator++;
          lastSeenAttribute ++;
        }
      }
    }
    else
    {
      pointerIGIdIterator = collectionIG.begin();
      lastSeenAttribute = 0;
    }
    int id = (*pointerIGIdIterator).second;
    return collection[id];
  }
  else
  {
    if (lastSeenAttribute != -1)
    {
      if (i == lastSeenAttribute + 1)
      {
        lastSeenAttribute++;
        pointerIterator ++;
      }
      else if (i != lastSeenAttribute)
      {
        pointerIterator = collection.begin();
        lastSeenAttribute = 0;
        for(int x = 0; x < i; x++)
        {
          pointerIterator ++;
          lastSeenAttribute ++;
        }
      }
    }
    else
    {
      pointerIterator = collection.begin();
      lastSeenAttribute = 0;
    }

    if (((*pointerIterator).second).removed == true)
      flag = false;
    return (*pointerIterator).second;
  }
}
Attribute& AttributeCollection::getAttributeAt(int i)
{
  if (collectionIGReady)
  {
    if (lastSeenAttribute != -1)
    {
      if (i == lastSeenAttribute + 1)
      {
        lastSeenAttribute++;
        pointerIGIdIterator ++;
      }
      else if (i != lastSeenAttribute)
      {
        pointerIGIdIterator = collectionIG.begin();
        lastSeenAttribute = 0;
        for (int x = 0; x < i; x++)
        {
          pointerIGIdIterator ++;
          lastSeenAttribute ++;
        }
      }
    }
    else
    {
      pointerIGIdIterator = collectionIG.begin();
      lastSeenAttribute = 0;
    }
    int id = (*pointerIGIdIterator).second;
    return collection[id];
  }
  else
  {
    if (lastSeenAttribute != -1)
    {
      if (i == lastSeenAttribute + 1)
      {
        lastSeenAttribute++;
        pointerIterator++;
      }
      else if (i != lastSeenAttribute)
      {
        pointerIterator = collection.begin();
        lastSeenAttribute = 0;
        for(int x = 0; x < i; x++)
        {
          pointerIterator++;
          lastSeenAttribute++;
        }
      }
    }
    else
    {
      pointerIterator = collection.begin();
      lastSeenAttribute = 0;
    }
    return (*pointerIterator).second;
  }
}
void AttributeCollection::addAttributeIGId (double IG, int id)
{
  collectionIG.insert(pair<double,int>(IG,id));
}
void AttributeCollection::removeAttributes(vector<int> att_tobe_deleted)
{
  size -= att_tobe_deleted.size();
  for (int i = 0; i < att_tobe_deleted.size(); i++)
  {
    int id = att_tobe_deleted[i];
    collection.erase(id);
  }
}
void AttributeCollection::fixSize(int m)
{
  if (size > m)
    size = m;
}
bool AttributeCollection::getAttribute(int id, Attribute& att)
{
  if (collection.find(id) != collection.end() )
  {
    att = collection[id];
    return true;
  }
  else
    return false;
}
void AttributeCollection::addAttribute(int id, bool spam, int sumOfTokens)
{
  double norm = 1.0 / ((double) sumOfTokens);
  // check if token exists in database
  if (collection.find(id) != collection.end())
  {
    if (collection[id].seenBefore == false)  //first time occured in this file
    {
      if(!spam)
      {
        collection[id].hamTotalCarrier++;
        collection[id].totalHamSequence++;
        hamAttributesCounter++;
      }
      else
      {
        collection[id].spamTotalCarrier++;
        collection[id].totalSpamSequence++;
        spamAttributesCounter++;
      }
      collection[id].seenBefore = true;
    } //met again in the file
    else
    {
      if (!spam) collection[id].totalHamSequence ++;
      else       collection[id].totalSpamSequence ++;
    }
  }
  else
  {
    Attribute att = Attribute(id);

    if (!spam)
    {
      att.hamTotalCarrier++;
      att.totalHamSequence++;
      hamAttributesCounter++;
    }
    else
    {
      att.spamTotalCarrier++;
      att.totalSpamSequence++;
      spamAttributesCounter++;
    }
    att.seenBefore = true;
    size++;
    collection.insert(make_pair(id, att));
  }
}

int AttributeCollection::numberOfAttributes()
{
  return size;
}

void AttributeCollection::printAttributes()
{
  map<int, Attribute>::iterator pointerTempIterator = collection.begin();
  Attribute temp;
  string t;
  for (int i = 0; i < size; i++)
  {
    temp = (*pointerTempIterator).second;
    cout << temp.id << "M Ham (" << temp.Mham << ") M spam(" << temp.Mspam<< ") S Ham(" << temp.Sham <<
      ")S spam(" << temp.Sspam << ")\n";
    pointerTempIterator++;
  }
}

void AttributeCollection::computeP_Graham(int sumOfHamMessages,
          int sumOfSpamMessages)
{
  Attribute att;
  for(int i = 0; i < size; i++)
  {
    att = getAttributeAt(i);
    double g = att.hamTotalCarrier * 2.0;
    double b = ((double)att.spamTotalCarrier);
    if (b + g < 5)
      getAttributeAt(i).P_Graham = -1;
    else
    {
      double minb = min(1.0, b / sumOfSpamMessages);
      double ming = min(1.0, g / b / sumOfHamMessages);
      double temp = minb  / (minb + ming);
      temp = min(0.99,temp);
      temp = max(0.01,temp);
      getAttributeAt(i).P_Graham = temp;
    }
  }
}

void AttributeCollection::clear()
{
 collection.clear();
 size = 0;
 spamAttributesCounter = 0;
 hamAttributesCounter = 0;
 lastSeenAttribute = -1;
}

void AttributeCollection::getAttributesFromFile(string nameOffile, map<string, int>& coder,
  int& sumOfHamMessages, int& sumOfSpamMessages, int& lastUsedCode)
{
  string line;
  vector<string> vline;
  ifstream fin (nameOffile.c_str());
  getline(fin, line);
  vline = getTokens(line, " ");

  stringstream ss1(vline[0]);    ss1 >> sumOfHamMessages;
  stringstream ss2(vline[1]);    ss2 >> sumOfSpamMessages ;
  stringstream ss11(vline[2]);   ss11 >> hamAttributesCounter;
  stringstream ss22(vline[3]);   ss22 >> spamAttributesCounter;
  stringstream ssLuC(vline[4]);  ssLuC >> lastUsedCode;

  while (getline(fin, line))
  {
    if (line == "")
      break;
    string token = line;
    getline(fin, line);
    int id, spamTotalCarrier, hamTotalCarrier;
    unsigned int totalHamSequence, totalSpamSequence;

    vline = getTokens(line,"$");
    stringstream ss3(vline[0]);  ss3 >> id;
    coder.insert(make_pair(token,id));
    stringstream ss4(vline[1]);  ss4 >> totalHamSequence;
    stringstream ss5(vline[2]);  ss5 >> totalSpamSequence;
    stringstream ss6(vline[3]);  ss6 >> hamTotalCarrier;
    stringstream ss7(vline[4]);  ss7 >> spamTotalCarrier;
    Attribute att = Attribute(id, totalHamSequence,totalSpamSequence, spamTotalCarrier, hamTotalCarrier);
    size++;
    collection.insert(make_pair(id, att));
  }
  fin.close();
}
void AttributeCollection::writeAttributesToFile(string nameOffile, map<string, int>& coder, int sumOfHamMessages,
  int sumOfSpamMessages, int last_used_code)
{
  ofstream fout(nameOffile.c_str());
  Attribute a;
  string token;
  int id;
  map<string,int>::iterator coderIterator = coder.begin();
  fout << sumOfHamMessages << " " << sumOfSpamMessages << " " << hamAttributesCounter << " " << spamAttributesCounter <<
    " " << last_used_code << endl;
  for (int i = 0; i < coder.size(); i++)
  {
    token = (*coderIterator).first;
    id = (*coderIterator).second;
    a = collection[id];
    fout << token << endl;
    fout << a.id << "$" << a.totalHamSequence << "$" << a.totalSpamSequence << "$" << a.hamTotalCarrier <<
      "$"<< a.spamTotalCarrier;
    fout << endl;
    coderIterator++;
  }
  fout.flush();
  fout.close();
}

vector<string> AttributeCollection::getTokens(const string& var, const string& sSeparators)
{
  vector<string> vRes;
  ostringstream sOut;

  for (unsigned int iCnt=0; iCnt < var.length(); iCnt++)
  {
    if (sSeparators.find(var.at(iCnt)) != string::npos)
    {
      if (sOut.str().length() > 0)
      {
        vRes.push_back(string(sOut.str()));
        sOut.str(string());
      }
      // Skip all whitespace
      while (iCnt < var.length() - 1)
      {
        if (sSeparators.find(var.at(++iCnt)) == string::npos)
        {
          iCnt--;
          break;
        }
      }
    }
    else if (iCnt < var.length())
      sOut << var.at(iCnt);
  }

  // Add last token
  if (sOut.str().length() > 0)
    vRes.push_back(string(sOut.str()));

  return vRes;
}
