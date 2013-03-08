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

// Definition of the Socket class
#ifndef Socket_class
#define Socket_class

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
	public:
		Socket();
		virtual ~Socket();

		// Server initialization
		bool create();
		bool bind(const int port);
		bool listen() const;
		bool accept(Socket&) const;

		// Client initialization
		bool connect(const std::string host, const int port);

		// Data Transimission
		bool send(const std::string) const;
		int recv(std::string&) const;

		void set_non_blocking(const bool);

		bool is_valid() const { return m_sock != -1; }

	private:
		int m_sock;
		sockaddr_in m_addr;
};

#endif
