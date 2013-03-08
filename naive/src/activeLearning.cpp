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

#include "activeLearning.h"

ActiveLearning::ActiveLearning(string fileName, int N, int C)
{
	sumOfScores = 500;
	srand((unsigned)(time(0)));
	ifstream fin (fileName.c_str());
	k = ((double)N) / ((double)C);
	thresholdMessages = k * sumOfScores;
	if (thresholdMessages == 0)
		thresholdMessages = 2;
	string line;
	while (getline(fin,line))
	{
		if (line != "")
		{
			vector<string> v = getTokens(line, "=");
			stringstream ss;
			ss << v[1];
			double score;
			ss >> score;
			distanceScoreMap.insert(make_pair(fabs(0.5 - score), score));
		}
	}
	computeIninitialX();
	fin.close();
}

bool ActiveLearning::trainPermission(int N, int C, double score)
{
	addScore(score);
	if (N == 0) 			return false;
	else if (N >= C) 	return true;
	else
	{
		st_dev = x;
		double prob;
		if (score <= 0.5)	prob = cdf(score) * 2;
		else							prob = cdf(1.0 - score) * 2;

		double random_integer = rand()/(float(RAND_MAX)+1);
		//cout << "r_i = " << random_integer <<  " prob =" << prob << endl;

		if (random_integer <= prob)	return true;
		else												return false;
	}
}

double ActiveLearning::xValue()
{
	currentDistanceScoreMap = distanceScoreMap.begin();
	for(int i = 0; i < thresholdMessages; i++)
		currentDistanceScoreMap++;

  return (*currentDistanceScoreMap).first;
}

void ActiveLearning::computeIninitialX()
{
	x = xValue();
}

void ActiveLearning::addScore(double score)
{
	sumOfScores++;
  thresholdMessages = k * sumOfScores;
	distanceScoreMap.insert(make_pair(fabs(0.5 - score), score));
	x = xValue();
}

void ActiveLearning::printMMap(int counter)
{
	int count = 0;

	currentDistanceScoreMap = distanceScoreMap.begin();
	while (currentDistanceScoreMap != distanceScoreMap.end())
	{
		count++;
		if (count > counter)
			break;
		cout << "Dist = " << (*currentDistanceScoreMap).first << "Score = " << (*currentDistanceScoreMap).second << endl;
		currentDistanceScoreMap++;
	}
}

vector<string> ActiveLearning::getTokens(const string& var, const string& sSeparators)
{
	vector<string> vRes;
  ostringstream sOut;

  for (unsigned int iCnt = 0; iCnt < var.length(); iCnt++)
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
		else
			if (iCnt < var.length())
				sOut << var.at(iCnt);
	}
	// Add last token
	if (sOut.str().length() > 0)
			vRes.push_back(string(sOut.str()));

	return vRes;
}

double ActiveLearning::erf(double x)
{
	static const double two_sqrtpi=  1.128379167095512574;        // 2/sqrt(pi)
	if (fabs(x) > 2.2)
		return 1.0 - erfc(x);        //use continued fraction
	double sum = x, term = x, xsqr = x*x;
	int j = 1;
	do
	{
		term *= xsqr/j;
		sum -= term/(2*j + 1);
		++j;
		term *= xsqr/j;
		sum += term/(2*j + 1);
		++j;
	} while (fabs(term/sum) > rel_error);   // CORRECTED LINE

	return two_sqrtpi * sum;
}

double ActiveLearning::erfc(double x)
{
	static const double one_sqrtpi = 0.564189583547756287;        // 1/sqrt(pi)
	if (fabs(x) < 2.2) 	return 1.0 - erf(x);        //use series when fabs(x) < 2.2
	if (signbit(x)) 		return 2.0 - erfc(-x);			//continued fraction only valid for x>0
	double a = 1, b = x;                //last two convergent numerators
	double c = x, d = x*x + 0.5;        //last two convergent denominators
	double q1, q2 = b/d;             		//last two convergents (a/c and b/d)
	double n = 1.0, t;
	do
	{
		t = a*n + b*x;
		a = b;
		b = t;
		t = c*n + d*x;
		c = d;
		d = t;
		n += 0.5;
		q1 = q2;
		q2 = b/d;
	} while (fabs(q1-q2)/q2 > rel_error);
	return one_sqrtpi * exp(-x*x) * q2;
}

double ActiveLearning::cdf(double score)
{
	double cdf = (score - 0.5) / (st_dev * sqrt(2));
	cdf = erf(cdf);
	cdf = 0.5 * (1 + cdf);
	return cdf;
}

