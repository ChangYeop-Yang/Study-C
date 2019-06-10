#include "stdafx.h"
#include "WinSocket.h"

WinSocket::~WinSocket() {
	this->closeTCPSocketServer();
}

void WinSocket::openTCPSocketServer(const int port, HWND hDig) {

	// MARK: WinSock 초기화
	if ( WSAStartup(MAKEWORD(2, 2), &this->wasData) ) {
		cout << "Error, SetUp WinSock." << endl;
		exit(false);
	}
		
	// MARK: Create TCP Socket
	this->hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (this->hServSock == INVALID_SOCKET) {
		cout << "Error, Set Socket." << endl;
		WSACleanup(), exit(false);
	}
	this->eventListBox->AddString(TEXT("※ Success, Create TCP Socket."));

	std::memset(&this->servAddr, 0, sizeof(SOCKADDR_IN));
	this->servAddr.sin_family = AF_INET;
	this->servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY 모든 IP 대역 접속을 허가한다.
	this->servAddr.sin_port = htons(port);

	// MARK: bind 함수는 지역주소를 소켓과 함께 결합(연관)시킵니다.
	if ( bind(this->hServSock, (sockaddr *)&this->servAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		cout << "Error, Bind TCP Socket." << endl;
		WSACleanup(), exit(false), closesocket(this->hServSock);
	}
	this->eventListBox->AddString(TEXT("※ Success, Bind TCP Socket."));

	// MARK: listen 함수는 소켓을 들어오는 연결에 대해 listening 상태에 배치합니다.
	if ( listen(this->hServSock, MAX_REQUEST_QUEUE_SIZE) == SOCKET_ERROR ) {
		cout << "Error, Open Connection-Request-Queue. (Listen)" << endl;
		WSACleanup(), exit(false), closesocket(this->hServSock);
	}
	this->eventListBox->AddString(TEXT("※ Success, Open Connection-Request-Queue. (Listen)"));
	
	// MARK: WSAAsyncSelect()함수를 호출하면 해당 소켓은 자동으로 Non_Blocking 모드로 전환된다.
	WSAAsyncSelect(this->hServSock, hDig, MWM_EVENT_SOCK, FD_ACCEPT | FD_CLOSE);
}

void WinSocket::OnSocketEvent(HWND hWnd, SOCKET sock, WORD eid, WORD error) {

	switch (eid) {
		case FD_ACCEPT:
			WinSocket::OnAccept(hWnd, eid, error);
			break;
		case FD_READ:
			WinSocket::OnReceiveMessage(sock, hWnd, eid, error);
			break;
		case FD_CLOSE:
			break;
	}

}

void WinSocket::closeTCPSocketServer() {
	WSACleanup(), closesocket(this->hServSock), closesocket(this->hClntSock);
}

void WinSocket::OnReceiveMessage(SOCKET sock, HWND hDig, WORD eid, WORD error) {

	if ( const int length = recv(sock, this->message, BUFSIZ, 0) ) {
		const auto msg = CString(this->message);
		this->eventListBox->AddString(msg);

		std::memset(&this->message, 0, sizeof(this->message));
	}

}

const bool WinSocket::OnSendMessage(const SOCKET sock, const std::string message) {
	const auto * msg = message.c_str();
	return send(sock, msg, sizeof(msg), 0) == 0 ? true : false; // ZERO == Success, EOF == Fail
}

void WinSocket::OnAccept(HWND hDig, WORD eid, WORD error) {

	// MARK: Accpet 함수는 소켓에 들어오는 연결 시도에 대해서 허가한다.
	this->szClntAddr	= sizeof(this->clntAddr);
	this->hClntSock		= accept(this->hServSock, (SOCKADDR *)&this->clntAddr, &this->szClntAddr);

	if (this->hClntSock == INVALID_SOCKET || this->hClntSock == SOCKET_ERROR) {
		cout << "Error, Accept." << endl;
		WSACleanup(), exit(false), closesocket(this->hServSock);
	}

	// MARK: Client Socket의 FD_READ, FD_CLOSE Event를 Non_Blocking 작동한다.
	WSAAsyncSelect(this->hClntSock, hDig, MWM_EVENT_SOCK, FD_READ | FD_CLOSE);
}

void WinSocket::OnCloseClientSocket(SOCKET sock, HWND hDig, WORD eid, WORD error) {

}