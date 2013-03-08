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

#ifndef _ACTIVELEARNING_H_
#define _ACTIVELEARNING_H_
#include <map>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

class ActiveLearning
{
	private:
		//distance , score
		multimap<double, double> distanceScoreMap;
		multimap<double, double>::iterator currentDistanceScoreMap;
		int sumOfScores;
		int thresholdMessages;
		double k;
		double x;
		double st_dev;
		static const double rel_error = 1E-12;        //calculate 12 significant figures

	public:
		ActiveLearning(){};
		ActiveLearning(string fileName, int N, int C);
		void printMMap(int counter);
		bool trainPermission(int N, int C, double score);

	private:
		double xValue();
		void computeIninitialX();
		void addScore(double score);

		vector<string> getTokens(const string& var, const string& sSeparators);
		double erf(double x);
		double erfc(double x);
		double cdf(double score);
};

#endif
