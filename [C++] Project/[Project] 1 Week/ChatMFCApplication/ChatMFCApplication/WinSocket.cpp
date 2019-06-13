#include "stdafx.h"
#include "WinSocket.h"

WinSocket::~WinSocket() {
	this->closeTCPSocketServer();
	
	// MARK: Remove All Object Variables.
	this->clients.clear();
	std::memset(this->message, 0, sizeof(this->message));
}

const CString WinSocket::GetCurrentTimeAndMessage(const CString message) {

	const auto now = CTime::GetCurrentTime();

	CString time_str;
	time_str.Format(_T("※ [%04d-%02d-%02d %02d:%02d:%02d] %s"), now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond(), message);

	return time_str;
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
		
		connection_mssage = GetCurrentTimeAndMessage(TEXT("Success, Create TCP/IP Socket."));
		this->eventListBox->AddString(connection_mssage);

		std::memset(&this->servAddr, 0, sizeof(SOCKADDR_IN));
		this->servAddr.sin_family		= AF_INET;
		this->servAddr.sin_addr.s_addr	= htonl(INADDR_ANY); // INADDR_ANY 모든 IP 대역 접속을 허가한다.
		this->servAddr.sin_port			= htons(port);

		// MARK: bind 함수는 지역주소를 소켓과 함께 결합(연관)시킵니다.
		if (bind(this->hServSock, (sockaddr *)&this->servAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
			throw Error::BIND_WINSOCK_ERROR;
		}
		
		connection_mssage = GetCurrentTimeAndMessage(TEXT("Success, Bind TCP/IP Socket."));
		this->eventListBox->AddString(connection_mssage);

		// MARK: listen 함수는 소켓을 들어오는 연결에 대해 listening 상태에 배치합니다.
		if (listen(this->hServSock, MAX_REQUEST_QUEUE_SIZE) == SOCKET_ERROR) {
			throw Error::LISTEN_WINSOCK_ERROR;
		}
		
		connection_mssage = GetCurrentTimeAndMessage(TEXT("Success, Open Connection-Request-Queue."));
		this->eventListBox->AddString(connection_mssage);

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
		this->eventListBox->AddString(connection_mssage);
	}
}

void WinSocket::OnSocketServerEventHandler(HWND hWnd, SOCKET sock, WORD eid, WORD error) {

	switch (eid) {
		case FD_ACCEPT:
			WinSocket::OnAccept(hWnd, eid, error);
			break;
		case FD_READ:
			WinSocket::OnReceiveMessage(sock, hWnd, eid, error);
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

		const auto msg = GetCurrentTimeAndMessage(receive_msg);
		this->eventListBox->AddString(msg);

		// Here Serial Sending. (Message) - Server Mode 일경우에만
		

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
	const auto message			= GetCurrentTimeAndMessage(ipaddress_str) + TEXT("의 주소로 접속하였습니다.");
	this->eventListBox->AddString(message);

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

		const auto message = GetCurrentTimeAndMessage( this->clients[sock].first + CString("의 클라이언트가 접속을 종료하였습니다.") );
		
		if (recv == SUCCESS_CODE) {
			closesocket(sock); // Client Socket Close.

			this->clients.erase(sock);
			this->eventListBox->AddString(message);
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
		this->eventListBox->AddString(connection_mssage);
	}

}

void WinSocket::DisConnectTCPSocketClient() {
	
	const CString message = GetCurrentTimeAndMessage( TEXT("서버와의 연결이 종료되었습니다.") );
	this->eventListBox->AddString(message);

	WSACleanup(), closesocket(this->hServSock);
}