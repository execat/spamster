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

#include "attribute.h"
Attribute::Attribute(int id_input)
{
  id                = id_input;
  totalHamSequence  = 0;
  totalSpamSequence = 0;
  spamTotalCarrier  = 0;
  hamTotalCarrier   = 0;
  Mham              = 0.0;
  Mspam             = 0.0;
  Sham              = 0.0;
  Sspam             = 0.0;
  IG                = 0.0;
  P_Graham          = 0.0;
  //active            = false;
  seenBefore        = false;
  removed           = false;
}

Attribute::Attribute(int id_in, unsigned int totalHamSequence_in, unsigned int totalSpamSequence_in,
  int spamTotalCarrier_in, int hamTotalCarrier_in)
{
  id                = id_in;
  totalHamSequence  = totalHamSequence_in;
  totalSpamSequence = totalSpamSequence_in;
  spamTotalCarrier  = spamTotalCarrier_in;
  hamTotalCarrier   = hamTotalCarrier_in;
  Mham              = 0.0;
  Mspam             = 0.0;
  Sham              = 0.0;
  Sspam             = 0.0;
  IG                = 0.0;
  P_Graham          = 0.0;
  //active            = false;
  seenBefore        = false;
  removed           = false;
}

Attribute::Attribute() {}
Attribute::~Attribute() {}
