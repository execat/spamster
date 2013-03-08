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

#include "multiNomialTF.h"
MultiNomialTF::MultiNomialTF(AttributeCollection& ACinput, int SOHM, int SOSM, double delta,
	map<string, int>& coder, bool integerTokens, bool rawTF, bool weightNorm, int NGrams, stringstream& fin)
{
	AC = ACinput;
	sumOfHamMessages = SOHM;
	sumOfSpamMessages = SOSM;
	sumOfMessages = SOHM + SOSM;
	Delta = delta;
	m = AC.numberOfAttributes();
	weightNormalization = weightNorm;
	//ifstream fin ("pmail");
	F = new double[AC.numberOfAttributes()];
	for (int i = 0; i < AC.numberOfAttributes(); i++)
		F[i] = 0.0;

	if (integerTokens)
	{
		int token;
		while (fin >> token)
		{
			for (int i = 0; i < AC.numberOfAttributes(); i++)
			{
				if(token == AC.getAttributeAt(i).id)
				{
					F[i] += 1.0;
					break;
				}
			}
		}
	}
	else
	{
		//ofstream logFile("logfile", ios::app);
		//int logCounterUnique =0;
		//int logCounterSum =0;
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
						F[i] += 1.0;
						//logCounterSum++;
						//if (F[i] == 1.0)
						//logCounterUnique ++;
						break;
					}
				}
			}
		}
		/*logFile << "AfterIGTokens = " <<  logCounterSum << " unique ones = "
<< logCounterUnique << endl;
		logFile.flush();
		logFile.close();*/
	}

	if (rawTF == false)
	{
		double lengthNorm = 0;
		for (int i = 0; i < AC.numberOfAttributes(); i++)
		{
			F[i] = log10(F[i] + 1.0);
			double IDF = ((double)AC.getAttributeAt(i).hamTotalCarrier + AC.getAttributeAt(i).spamTotalCarrier);
			IDF = sumOfMessages / IDF;
			IDF = log10 (IDF);
			F[i] = F[i] * IDF;
			lengthNorm = lengthNorm + (F[i] * F[i]);
		}

		if (lengthNorm != 0.0)  lengthNorm = sqrt(lengthNorm);
		else                    lengthNorm = 0.000001;

		for (int i = 0; i < AC.numberOfAttributes(); i++)
			F[i] = F[i] / lengthNorm;
	}
	//fin.close();
}

MultiNomialTF::~MultiNomialTF()
{
	delete [] F;
}

void MultiNomialTF::printF()
{
	for(int i = 0; i < AC.numberOfAttributes(); i++)
		cout << F[i] << " ";
	cout << endl;
}

bool MultiNomialTF::spamIdentifier(int isSpam, int k, double& Pham, double& Pspam)
{
	if (Delta == -1)
	 return true;
	double Pcs = ((double)sumOfSpamMessages) /  sumOfMessages;
	Pcs = log10(Pcs);
	double Pch = ((double)sumOfHamMessages) /  sumOfMessages;
	Pch = log10(Pch);

	unsigned int Ncs = 0;
	unsigned int Nch = 0;
	for (int i = 0; i < m; i++)
	{
		Ncs += AC.getAttributeAt(i).totalSpamSequence;
		Nch += AC.getAttributeAt(i).totalHamSequence;
	}

	double Pxcs = 0.0;
	double Pxch = 0.0;
	vector<double> PxcsCollection;
	vector<double> PxchCollection;
	for (int i = 0; i < AC.numberOfAttributes(); i++)
	{
		double PtcHam;
		double PtcSpam;
		PtcHam = ((double)(1 + AC.getAttributeAt(i).totalHamSequence)) / (Nch+m);
		PtcSpam = ((double)(1 + AC.getAttributeAt(i).totalSpamSequence)) / (Ncs+m);
		PtcHam = log10(PtcHam);
		PtcSpam = log10(PtcSpam);

		if(weightNormalization)
		{
			double S = fabs(PtcHam) + fabs(PtcSpam);
			PtcHam = PtcHam / S;
			PtcSpam = PtcSpam / S;
			PtcHam = F[i] * PtcHam;
			PtcSpam = F[i] * PtcSpam;
			//Pxch += PtcHam;
			//Pxcs += PtcSpam;
			PxchCollection.push_back(PtcHam);
			PxcsCollection.push_back(PtcSpam);
		}
		else
		{
			PtcHam = F[i] * PtcHam;
			PtcSpam = F[i] * PtcSpam;
			//Pxch += PtcHam;
			//Pxcs += PtcSpam;
			PxchCollection.push_back(PtcHam);
			PxcsCollection.push_back(PtcSpam);
		}
	}

	if (k != 0)
	{
		bool* Best = new bool[AC.numberOfAttributes()];
		for (int i = 0; i < AC.numberOfAttributes(); i++)
			Best[i] = false;

		for (int i = 0; i < k; i++)
		{
			double max = 0;
			int pointer = -1;

			for(int j = 0; j < AC.numberOfAttributes(); j++)
			{
				double absHam   = fabs(PxchCollection[j]);
				double absSpam  = fabs(PxcsCollection[j]);

				if (Best[j] == false)
				{
					if (absHam >= max || absSpam >= max)
					{
						pointer = j;
						if (absHam > absSpam) max = absHam;
						else                  max = absSpam;
					}
				}
			}
			Best[pointer] = true;
		}

		for (int i = 0; i < AC.numberOfAttributes(); i++)
		{
			if (Best[i] == false)
			{
				PxchCollection[i] = 0.0;
				PxcsCollection[i] = 0.0;
			}
		}
		delete Best;
	}

	for (int i = 0; i < AC.numberOfAttributes(); i++)
	{
		Pxcs += PxcsCollection[i];
		Pxch += PxchCollection[i];
	}
	Pspam =  Pcs + Pxcs;
	Pham = Pch + Pxch;

	if (Pspam - Pham > Delta) return true;
	else                      return false;
}
