# ● DAOU DATA BEGINER EDUCATION

## 1️⃣ TCP/IP Socket Communication Project

|:camera: TCP/IP Socket Communication|:camera: Serial Comunication|
|:----------------------------------:|:--------------------------:|
|![](https://user-images.githubusercontent.com/20036523/59315384-856e0680-8cf4-11e9-8897-970753c28be1.jpg)|![](https://user-images.githubusercontent.com/20036523/59576705-a327db00-90fb-11e9-9919-9a1193d5c86a.png)|

* TCP/IP Socket Communication Project를 수행함으로써 TCP/IP에 대한 전반적인 개념을 이해하고자 한다. 또한 프로젝트를 수행하면서 발생하는 소켓통신에 대한 오류점에 대해서도 학습하고자 한다.

#### 📋 TCP/IP Header Source Code (Microsoft Foundation Class Library)

```C++
#pragma once

#ifndef WINSOCK_H_
#define WINSOCK_H_

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <WinSock2.h>

#include "WinSocketError.h"

#define SUCCESS_CODE 0
#define MAX_REQUEST_QUEUE_SIZE 5

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using std::cout;
using std::endl;

// MARK: - Typedef
typedef std::pair<CString, SOCKADDR_IN> User;

// MARK: - Enum
enum EventMessage {
	MWM_SERVER_EVENT_SOCK			= (WM_USER + 1),
	MWM_CLIENT_EVENT_SOCK			= (WM_USER + 2)
};

class WinSocket
{
	// MARK: - System Methods
	public:
		~WinSocket();

	// MARK: - User Methods
	public:
		void openTCPSocketServer(const int port, HWND hDig);
		void closeTCPSocketServer();
		
		void ConnectTCPClient(const std::string ip, const int port, HWND hDig);
		void DisConnectTCPSocketClient();

		void OnSocketServerEventHandler(HWND hWnd, SOCKET sock, WORD eid, WORD error, std::string message);
		void OnSocketClientEventHandler(HWND hWnd, SOCKET sock, WORD eid, WORD error);
				
		const bool OnSendMessage(const SOCKET sock, const std::string message);
		void OnAllSendClientMessage(const std::string message);
		void OnSendMessageServer(const std::string message);

	private:
		void OnAccept(HWND hDig, WORD eid, WORD error);
		void OnReceiveMessage(SOCKET sock, HWND hDig, WORD eid, WORD error);
		void OnReceiveClientMessage(SOCKET sock, HWND hDig, WORD eid, WORD error, std::string message);
		void OnCloseClientSocket(SOCKET sock, HWND hDig, WORD eid, WORD error);

	// MARK: - Object Methods
	public:
		bool socket_Mode;

	private:		
		int szClntAddr;
		char message[BUFSIZ];
		std::unordered_map<SOCKET, User> clients;

		WSADATA wasData;
		SOCKET hServSock;
		SOCKADDR_IN servAddr;
};

#endif
```

#### 📋 TCP/IP Header Source Code (Microsoft Foundation Class Library)

```C++
#include "stdafx.h"
#include "WinSocket.h"

WinSocket::~WinSocket() {
	this->closeTCPSocketServer();
	
	// MARK: Remove All Object Variables.
	this->clients.clear();
	std::memset(this->message, 0, sizeof(this->message));
}

void WinSocket::openTCPSocketServer(const int port, HWND hDig) {

	CString connection_mssage;

	try {

		// MARK: WinSock 초기화
		if (WSAStartup(MAKEWORD(2, 0), &this->wasData)) {
			throw Error::SETUP_WINSOCK_WSASTART_UP_ERROR;
		}

		// MARK: Create TCP Socket
		this->hServSock = socket(PF_INET, SOCK_STREAM, 0);
		if (this->hServSock == INVALID_SOCKET) {
			throw Error::SETUP_WINSOCK_ERROR;
		}

		std::memset(&this->servAddr, 0, sizeof(SOCKADDR_IN));
		this->servAddr.sin_family		= AF_INET;
		this->servAddr.sin_addr.s_addr	= htonl(INADDR_ANY); // INADDR_ANY 모든 IP 대역 접속을 허가한다.
		this->servAddr.sin_port			= htons(port);

		// MARK: bind 함수는 지역주소를 소켓과 함께 결합(연관)시킵니다.
		if (bind(this->hServSock, (sockaddr *)&this->servAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
			throw Error::BIND_WINSOCK_ERROR;
		}

		// MARK: listen 함수는 소켓을 들어오는 연결에 대해 listening 상태에 배치합니다.
		if (listen(this->hServSock, MAX_REQUEST_QUEUE_SIZE) == SOCKET_ERROR) {
			throw Error::LISTEN_WINSOCK_ERROR;
		}

		// MARK: WSAAsyncSelect()함수를 호출하면 해당 소켓은 자동으로 Non_Blocking 모드로 전환된다.
		WSAAsyncSelect(this->hServSock, hDig, MWM_SERVER_EVENT_SOCK, FD_ACCEPT | FD_CLOSE);
	}
	catch (const int exception) {

		switch (exception) {
			case Error::SETUP_WINSOCK_WSASTART_UP_ERROR: {
				connection_mssage = "※ Error, SETUP_WINSOCK_WSASTART_UP_ERROR";
				break;
			}
			case Error::SETUP_WINSOCK_ERROR: {
				connection_mssage = "※ Error, SETUP_WINSOCK_ERROR";
				break;
			}
			case Error::BIND_WINSOCK_ERROR: {
				connection_mssage = "※ Error, BIND_WINSOCK_ERROR";
				break;
			}
			case Error::LISTEN_WINSOCK_ERROR: {
				connection_mssage = "※ Error, LISTEN_WINSOCK_ERROR";
				break;
			}
		}

		closeTCPSocketServer();
	}
}

void WinSocket::OnSocketServerEventHandler(HWND hWnd, SOCKET sock, WORD eid, WORD error, std::string message) {

	switch (eid) {
		case FD_ACCEPT:
			WinSocket::OnAccept(hWnd, eid, error);
			break;
		case FD_READ:
			WinSocket::OnReceiveClientMessage(sock, hWnd, eid, error, message);
			break;
		case FD_CLOSE:
			WinSocket::OnCloseClientSocket(sock, hWnd, eid, error);
			break;
	}

}

void WinSocket::OnSocketClientEventHandler(HWND hWnd, SOCKET sock, WORD eid, WORD error) {

	switch (eid) {
		case FD_READ:
			WinSocket::OnReceiveMessage(sock, hWnd, eid, error);
			break;
		case FD_CLOSE:
			WinSocket::DisConnectTCPSocketClient();
			break;
	}

}

void WinSocket::closeTCPSocketServer() {
	WSACleanup(), closesocket(this->hServSock);
}

void WinSocket::OnReceiveMessage(SOCKET sock, HWND hDig, WORD eid, WORD error) {

	if ( const int length = recv(sock, this->message, BUFSIZ, 0) ) {

		const auto receive_msg = this->clients[sock].first + TEXT(" ") + CString(this->message);

		std::memset(&this->message, 0, sizeof(this->message));
	}

}

const bool WinSocket::OnSendMessage(const SOCKET sock, const std::string message) {
	return send(sock, message.c_str(), message.size(), 0) == 0 ? true : false; // ZERO == Success, EOF == Fail
}

void WinSocket::OnSendMessageServer(const std::string message) {
	send(this->hServSock, message.c_str(), message.size(), 0);
}

void WinSocket::OnAccept(HWND hDig, WORD eid, WORD error) {

	std::pair<SOCKET, SOCKADDR_IN> user;

	// MARK: Accpet 함수는 소켓에 들어오는 연결 시도에 대해서 허가한다.
	this->szClntAddr	= sizeof(SOCKADDR_IN);
	user.first			= accept(this->hServSock, (SOCKADDR *)&user.second, &this->szClntAddr);

	if (user.first == INVALID_SOCKET || user.first == SOCKET_ERROR) {
		cout << "Error, Accept." << endl;
		WSACleanup(), exit(false), closesocket(this->hServSock);
	}

	// MARK: Connection Message Output
	const auto ipaddress_str	= CString( inet_ntoa(user.second.sin_addr) );

	const auto value = std::make_pair(user.first, std::make_pair(ipaddress_str, user.second));
	this->clients.insert(value);

	/* 
		MARK: Client Socket의 FD_READ, FD_CLOSE Event를 Non_Blocking 작동한다.
		★ WSAAsyncSelect - The WSAAsyncSelect function requests Windows message-based notification of network events for a socket.
	*/
	WSAAsyncSelect(user.first, hDig, MWM_SERVER_EVENT_SOCK, FD_READ | FD_CLOSE);
}

void WinSocket::OnCloseClientSocket(SOCKET sock, HWND hDig, WORD eid, WORD error) {

	ULONG recv;

	if (const int value = ioctlsocket(sock, FIONREAD, &recv) == SUCCESS_CODE) {
		
		if (recv == SUCCESS_CODE) {
			closesocket(sock); // Client Socket Close.

			this->clients.erase(sock);
		}
	}

}

void WinSocket::OnAllSendClientMessage(const std::string message) {
	
	for (const auto user : this->clients) {
		OnSendMessage(user.first, message);
	}
}

// MARK: - Client TCP/IP Methods

void WinSocket::ConnectTCPClient(const std::string ip, const int port, HWND hDig) {

	CString connection_mssage;

	try {

		if ( WSAStartup(MAKEWORD(2, 2), &this->wasData) ) { 
			throw Error::SETUP_WINSOCK_WSASTART_UP_ERROR; 
		}

		this->hServSock = socket(PF_INET, SOCK_STREAM, 0);
		if (this->hServSock == INVALID_SOCKET) { 
			throw Error::SETUP_WINSOCK_ERROR; 
		}

		std::memset( &this->servAddr, 0, sizeof(SOCKADDR_IN) );
		this->servAddr.sin_family		= AF_INET; // IPv4
		this->servAddr.sin_addr.s_addr	= inet_addr(ip.c_str());
		this->servAddr.sin_port			= htons(port);

		// MARK: 생성한 소켓을 통해 서버로 접속을 요청합니다.
		if ( connect(this->hServSock, (SOCKADDR *)&servAddr, sizeof(SOCKADDR)) == SOCKET_FAIL_ERROR) {
			throw Error::CONNECT_SERVER_ERROR;
		}

		WSAAsyncSelect(this->hServSock, hDig, MWM_CLIENT_EVENT_SOCK, FD_READ | FD_CLOSE);
	} 
	catch (const int exception) {

		switch (exception) {
			case Error::SETUP_WINSOCK_WSASTART_UP_ERROR: {
				connection_mssage = "※ Error, SETUP_WINSOCK_WSASTART_UP_ERROR";
				break; 
			}
			case Error::SETUP_WINSOCK_ERROR: { 
				connection_mssage = "※ Error, SETUP_WINSOCK_ERROR";
				break; 
			}
			case Error::CONNECT_SERVER_ERROR: { 
				connection_mssage = "※ Error, CONNECT_SERVER_ERROR";
				break; 
			}
		}

		DisConnectTCPSocketClient();
	}

}

void WinSocket::DisConnectTCPSocketClient() {
	WSACleanup(), closesocket(this->hServSock);
}
```

#### 🔑 DEVLEOPMENT KEYWORD

* `WSAAsyncSelect`
* `TCP/IP`
* `SOCKET`
* `WINDOWS`
* `C/C++`
* `Serial`
* `Concurrency (Thread)`

#### 👀 TIP

* **MFC (Microsoft Foundation Class Library) Console** </br> `#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")`

#### [🚀 Project Source Code](https://github.com/ChangYeop-Yang/Study-C/tree/master/%5BC%2B%2B%5D%20Project/%5BProject%5D%201%20Week/ChatMFCApplication)

## 2️⃣ Android TCP/IP Socket Communication Framework Project

* 

#### 🔑 DEVLEOPMENT KEYWORD

* `Android`
* `JAVA`
* `TCP/IP`
* `C/C++`
* `Android NDK`

#### [🚀 Project Source Code]()

## ✈️ REFERENCE

#### [🚀 Google C++ Style Guide URL](https://google.github.io/styleguide/cppguide.html#Enumerator_Names)

#### [🚀 AsyncSelect Word File](https://github.com/ChangYeop-Yang/Study-C/files/3279696/AsyncSelect.docx)

#### [🚀 Concurrency in C++11 - The University of Chicago](https://www.classes.cs.uchicago.edu/archive/2013/spring/12300-1/labs/lab6/)

#### [🚀 Serial-Programming-Win32API-C - GitHub](https://github.com/xanthium-enterprises/Serial-Programming-Win32API-C)
