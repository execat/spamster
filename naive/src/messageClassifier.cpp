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

#include "messageClassifier.h"
MessageClassifier::MessageClassifier(AttributeCollection* att, double D, bool iTokens, int NG, string V, int K)
{
	stringstream ss(V);
	ss >> version;
	attributes = att;
	Delta = D;
	integerTokens = iTokens;
	NGram = NG;
	k = K;
}

MessageClassifier::MessageClassifier() {}

int MessageClassifier::Classifier(map<string, int>& coder, int isSpam, double& Pham, double& Pspam,
	int sumOfHamMessages, int sumOfSpamMessages, stringstream& fin)
{
	int spam;
	switch (version)
	{
		case 1: //Multivariate Bernulli
		{
			MultiVariateBernulliNB NB = MultiVariateBernulliNB(*attributes, sumOfHamMessages, sumOfSpamMessages,
				Delta, coder, integerTokens, NGram, fin);
			spam = NB.spamIdentifier(isSpam, Pham, Pspam);
		}
		break;

		case 2: //Multinomial TF Attributes(weight normalization = off)
		{
			MultiNomialTF NB = MultiNomialTF(*attributes, sumOfHamMessages, sumOfSpamMessages, Delta, coder,
				integerTokens, true, false, NGram, fin);
			spam = NB.spamIdentifier(isSpam, k, Pham, Pspam);
		}
		break;

		case 3: //Multinomial TF Attributes(weight normalization = on)
		{
			MultiNomialTF NB = MultiNomialTF(*attributes, sumOfHamMessages, sumOfSpamMessages, Delta, coder,
				integerTokens, true, true, NGram, fin);
			spam = NB.spamIdentifier(isSpam, k, Pham, Pspam);
		}
		break;

		case 4: //Multinomial TF transformed Attributes (weight normalization = off)
		{
			MultiNomialTF NB = MultiNomialTF(*attributes, sumOfHamMessages, sumOfSpamMessages, Delta, coder,
				integerTokens, false, false, NGram, fin);
			spam = NB.spamIdentifier(isSpam, k, Pham, Pspam);
		}
		break;

		case 5: //Multinomial TF transformed Attributes (weight normalization = on)
		{
			MultiNomialTF NB = MultiNomialTF(*attributes, sumOfHamMessages, sumOfSpamMessages, Delta, coder,
				integerTokens, false, true, NGram, fin);
			spam = NB.spamIdentifier(isSpam, k, Pham, Pspam);
		}
		break;

		case 6: //Multinomial Boolean Attributes (weight normalization = off)
		{
			MultiNomialBool NB = MultiNomialBool(*attributes, sumOfHamMessages, sumOfSpamMessages, Delta, coder,
				integerTokens, false, NGram, fin);
			spam = NB.spamIdentifier(isSpam, k, Pham, Pspam);
		}
		break;

		case 7: //Multinomial Boolean Attributes (weight normalization = on)
		{
			MultiNomialBool NB = MultiNomialBool(*attributes, sumOfHamMessages, sumOfSpamMessages, Delta, coder,
				integerTokens, true, NGram, fin);
			spam = NB.spamIdentifier(isSpam, k, Pham, Pspam);
		}
		break;

		case 8:  //Paul Graham's filter
		{
			SpamFilterPaulGraham SF = SpamFilterPaulGraham(*attributes, sumOfHamMessages, sumOfSpamMessages, Delta, coder,
				integerTokens, fin);
			spam = SF.spamIdentifier(isSpam, k, Pspam);
		}
		break;

		default : cout << "Error!!\n";
	}
	return spam;
}
void MessageClassifier::Statistics(int ham, int spam)
{
	switch (version)
	{
		case 8 :
		{
			attributes->computeP_Graham(ham, spam);
		}
		break;

		default: ;
	}
}
MessageClassifier::~MessageClassifier() {}
