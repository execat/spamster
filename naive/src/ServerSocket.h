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

// Definition of the ServerSocket class
#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"

class ServerSocket : private Socket
{
	public:
		ServerSocket(int port);
		ServerSocket(){};
		virtual ~ServerSocket();

		const ServerSocket& operator << (const std::string&) const;
		const ServerSocket& operator >> (std::string&) const;

		void accept(ServerSocket&);
};

#endif