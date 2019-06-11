#pragma once

#ifndef WINSOCK_ERROR_H_
#define WINSOCK_ERROR_H_

#define SOCKET_FAIL_ERROR -1

namespace Error
{
	enum SocketError
	{
		SETUP_WINSOCK_WSASTART_UP_ERROR = 1,
		SETUP_WINSOCK_ERROR				= 2,
		CONNECT_SERVER_ERROR			= 3,
		BIND_WINSOCK_ERROR				= 4,
		LISTEN_WINSOCK_ERROR			= 5
	};
}

#endif