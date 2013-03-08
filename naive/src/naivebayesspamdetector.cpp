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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include "attributeCollection.h"
#include "messageClassifier.h"
#include "multiVariateBernulliNB.h"
#include "multiNomialTF.h"
#include "multiNomialBool.h"
#include "spamFilterPaulGraham.h"
#include "emailParser.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "SocketException.h"
#include <time.h>
#include "activeLearning.h"

using namespace std;

//reads the training data and retrieves the attributes
void getAttributes(bool spam, AttributeCollection* attributes, map<string, int>& coder, int& lastUsedCode,
	bool integerTokens, int NGram, stringstream& fin);
void parseEmail(string inputMessage, stringstream& finalstream, bool parserOn);
double strToDouble(string s);
int strToInt(string s);
string doubleToString(double d);
string intToString(int i);

int main(int argc, char *argv[])
{
	string para = argv[1];
	if (para == "-i")
	{
		//BASIC VARIABLES
		//A token must apppear in at least MOinteger training messages in order to be used
		int MOinteger;
		//Threshold tuning strictness of the filter (0.0 - 1.0)
		//High values lead to high acuracy but low recall
		double K;
		//m is the number of best IG features that will be used.
		int m;
		// If NGram = 0 then no Ngram used. else NGram shows the size of NGram
		int NGram = 0;
		//feature selection
		int k;

		//secondary variables
		string NB_version = "";	//change algorithm
		double Delta = 0;
		map<string,int> coder;
		coder.clear();
		int lastUsedCode = 1;
		bool integerTokens = false;
		bool combinedNgramsSinglewords = false;
		//String with the names of the files that we use as training data
		int sumOfHamMessages = 0;
		int sumOfSpamMessages = 0;
		int sumOfMessages = 0;

		int MessagesWeWillSee = 0;
		int MessagesWeWillTrain = 0;
		bool firstMessage = true;
		bool parserOn;

		//carries the initial attributes
		AttributeCollection* attributes;
		attributes = new AttributeCollection();
		string DataFileName = argv[2];
		NB_version = argv[3];	//version of NB

		string AdvansedParameters = argv[4];
		if (AdvansedParameters == "-default")
		{
			MOinteger = 5;
			m = 3000;
			K = 0.5;
			parserOn = true;
			k = 0;
		}
		else
		{
			if (AdvansedParameters == "-parserON")	parserOn = true;
			else 																		parserOn = false;

			MOinteger = strToInt(argv[5]);
			m = strToInt(argv[6]);
			K = strToDouble(argv[7]);
			k =  strToInt(argv[8]);
		}

		if (DataFileName != "-none")
			attributes->getAttributesFromFile(DataFileName, coder, sumOfHamMessages, sumOfSpamMessages, lastUsedCode);

		ActiveLearning al;
		bool alive = true;

		ServerSocket server (32167);
		while (alive)
		{
			string so_message = "";
			ServerSocket newSock;
			server.accept(newSock);

			newSock >> so_message;
			vector<string> so_messageVector = attributes->getTokens(so_message, " ");
			string flag = so_messageVector[0];

			string inputFile;

			if (flag == "-c")
			{
				//Active learning stuff
				if (firstMessage && so_messageVector.size() == 4)
				{
					MessagesWeWillSee = strToInt(so_messageVector[3]);
					MessagesWeWillTrain = strToInt(so_messageVector[2]);

					string readyScores;
					if (NB_version == "4")				readyScores = "./TFIDFscoresReady";
					else if (NB_version == "6")		readyScores = "./BoolscoresReady";
					al =  ActiveLearning(readyScores, MessagesWeWillTrain, MessagesWeWillSee);
				}	//end of Active learning

				firstMessage = false;
				inputFile = so_messageVector[1];
				//inputFile = "/" + inputFile;

				stringstream finalStream;
				parseEmail(inputFile, finalStream,parserOn);
				sumOfMessages = sumOfHamMessages + sumOfSpamMessages;

				//Keep attributes that appear in at least MOinteger messages
				int removedAttributes = 0;
				for (int i = 0; i < attributes->numberOfAttributes(); i++)
				{
					int sum = attributes->getAttributeAt(i).hamTotalCarrier +
						attributes->getAttributeAt(i).spamTotalCarrier;
					if (sum < MOinteger)
					{
						attributes->getAttributeAt(i).removed = true;
						removedAttributes ++;
					}
				}

				//IG calculation
				if (m != 0)
				{
					double S = 0;
					for (int i = 0; i < attributes->numberOfAttributes(); i++)
					{
						S = 0;
						bool notRemoved = true;
						Attribute att = attributes->getAttributeAt(i,notRemoved);
						if (notRemoved == true)
						{
							double PXx = ((double)
								(att.hamTotalCarrier + att.spamTotalCarrier)) / sumOfMessages;
							double PCc = ((double)(sumOfHamMessages)) / sumOfMessages;
							double PXxCc1 = ((double)(att.hamTotalCarrier + 0.1)) / (sumOfHamMessages+0.2);
							double PXxCc = PXxCc1 * PCc;
							double temp = PXxCc / (PXx * PCc);
							S = S + (PXxCc * (log(temp) / log(2)));
							//X =0 C = ham
							PXx = 1 - PXx;
							if (PXx == 0)
								PXx = 0.0000001;
							PXxCc1 =  1 - PXxCc1;
							if (PXxCc1 == 0)
								PXxCc1 = 0.0000001;
							PXxCc = PXxCc1 * PCc;
							temp = PXxCc / (PXx * PCc);
							S = S + (PXxCc * (log(temp)/log(2)));
							//X=1 C = spami
							PXx = ((double)(att.hamTotalCarrier + att.spamTotalCarrier)) / sumOfMessages;
							PCc = ((double)(sumOfSpamMessages)) / sumOfMessages;
							PXxCc1 = ((double)(att.spamTotalCarrier + 0.1)) / (sumOfSpamMessages+0.2);
							PXxCc = PXxCc1 * PCc;
							temp = PXxCc / (PXx * PCc);
							S = S + (PXxCc * (log(temp)/log(2)));
							//X =0 C = spam
							PXx = 1 - PXx;
							if (PXx == 0)
								PXx = 0.0000001;
							PXxCc1 =  1 - PXxCc1;
							if (PXxCc1 == 0)
								PXxCc1 = 0.0000001;
							PXxCc = PXxCc1 * PCc;
							temp = PXxCc / (PXx * PCc);
							S = S + (PXxCc * (log(temp)/log(2)));

							att.IG = S;
							attributes->addAttributeIGId((S * (-1)), att.id);
						}
					}
					attributes->size -= removedAttributes;
					attributes->fixSize(m);
				}
				else
				{
					for (int i = 0; i < attributes->numberOfAttributes(); i++)
					{
						bool notRemoved = true;
						Attribute att = attributes->getAttributeAt(i, notRemoved);
						if (notRemoved == true)
							attributes->addAttributeIGId (0.0, att.id);
					}
					attributes->size -= removedAttributes;
				}
				attributes->collectionIGReady = true;
				MessageClassifier* MC;
				MC = new MessageClassifier(attributes, Delta, integerTokens, NGram, NB_version, k);

				MC->Statistics(sumOfHamMessages, sumOfSpamMessages);
				int isSpam;
				double Pham;
				double Pspam;
				double score;
				isSpam = MC->Classifier(coder, isSpam, Pham, Pspam, sumOfHamMessages, sumOfSpamMessages, finalStream);

				if (NB_version == "8") //in case of PaulGraham
					score = Pspam;
				else
				{
					if (Pspam <= -100.0 || Pham <= -100.0)
					{
						Pspam = Pspam /10;
						Pham = Pham /10;
					}
					score = pow(10, Pspam) / (pow(10, Pham) + pow(10, Pspam));
				}
				string Class;
				if (score >= K)	Class = "spam";
				else 						Class = "ham";
				string label = " labelReq=";

				if (so_messageVector.size() == 4)
				{
					int MessagesWeWillSeeTemp = strToInt(so_messageVector[3]);
					int MessagesWeWillTrainTemp = strToInt(so_messageVector[2]);
					if (al.trainPermission(MessagesWeWillTrainTemp, MessagesWeWillSeeTemp, score))
						label += "labelN";
					else
						label += "noRequest";
				}

				string filterOutput;
				filterOutput = "class=" + Class + " score=";
				string scroreString = doubleToString (score);
				filterOutput += scroreString + " tfile=" + argv[2];
				if(so_messageVector.size() == 4)
					filterOutput += label;
				bool notConnected = true;
				while (notConnected)
				{
					try
					{
						ClientSocket client_socket ("localhost", 32168);
						client_socket << filterOutput;
						notConnected = false;
					}
					catch (SocketException e)
					{
						usleep (50000);
					}
				}

				delete MC;
				attributes->reset();
			}
			else if (flag == "-h" || flag == "-s")//training
			{
				inputFile = so_messageVector[so_messageVector.size()-1];
				//inputFile = "/" + inputFile;
				stringstream finalStream;
				parseEmail(inputFile, finalStream, parserOn);
				bool spamInputMessage;
				if (flag == "-h")
				{
					spamInputMessage = false;
					sumOfHamMessages += 1;
				}
				else
				{
					spamInputMessage = true;
					sumOfSpamMessages += 1;
				}
				getAttributes (spamInputMessage, attributes, coder, lastUsedCode, integerTokens, NGram, finalStream);
				//attributes->writeAttributesToFile("testData",coder,sumOfHamMessages,sumOfSpamMessages,lastUsedCode);
			}
			else if (flag == "-t")//termination
				alive = false;
			else
			{
				cout << "Fatal error classsifier" << endl;
				alive = false;
			}

			if (flag != "-c")
			{
				bool notConnected = true;
				while (notConnected)
				{
					try
					{
						ClientSocket client_socket("localhost", 32168);
						client_socket << "DONE";
						notConnected = false;
					}
					catch (SocketException e)
					{
						usleep (50000);
					}
				}
			}
		}
		delete attributes;
		coder.clear();
	}
	else
	{
		bool notConnected = true;
		while (notConnected)
		{
			try
			{
				ClientSocket client_socket("localhost", 32167);
				notConnected = false;

				string DataFileName = argv[2];
			  if ( argc == 5 )	client_socket << para + " " + DataFileName + " " + argv[3] + " " + argv[4];
				else 							client_socket << para + " " + DataFileName;
			}
			catch (SocketException e)
			{
				usleep (50000);
			}
		}

		ServerSocket server(32168);
		ServerSocket newSock;
		server.accept(newSock);
		string so_message = "";
		newSock >>  so_message;
		if (so_message != "DONE")
			cout << so_message << endl;             //alagi
	}
  return EXIT_SUCCESS;
}

void getAttributes (bool spam, AttributeCollection* attributes, map<string, int>& coder, int& lastUsedCode,
	bool integerTokens, int NGrams, stringstream& fin)
{
  attributes->reset();
  string temp = "";
  if (integerTokens)
  {
   	int token;
   	vector<int> tokens;
   	tokens.clear();
   	while (fin >> token)
   		tokens.push_back(token);
	  for (int j = 0; j < tokens.size(); j++)
   		attributes->addAttribute(tokens[j], spam, tokens.size());
  }
  else
  {
    if (NGrams == 0)
		{
			string token;
			vector<int> tokens;
			tokens.clear();
			while (fin >> token)
			{
				if (token.size() >= 100)
					token = token.substr(0, 100);
				if (coder.find(token) == coder.end())
				{
					int luc = lastUsedCode;
					coder.insert(make_pair(token, luc));
					lastUsedCode++;
					tokens.push_back(luc);
				}
				else
					tokens.push_back(coder[token]);
			}

			for (int j = 0; j < tokens.size(); j++)
				attributes->addAttribute(tokens[j], spam, tokens.size());
  	}
  }
}

void parseEmail(string inputMessage, stringstream& finalstream, bool parserOn)
{
	EmailParser ep = EmailParser(inputMessage, finalstream, parserOn);
}

double strToDouble (string s)
{
	stringstream ss;
	ss << s;
	double ret;
	ss >> ret;
	return ret;
}

int strToInt (string s)
{
	stringstream ss;
	ss << s;
	int ret;
	ss >> ret;
	return ret;
}

string doubleToString(double d)
{
	stringstream ss;
  ss << d;
  string ret;
  ss >> ret;
  return ret;
}

string intToString(int i)
{
	stringstream ss;
  ss << i;
  string ret;
  ss >> ret;
  return ret;
}
