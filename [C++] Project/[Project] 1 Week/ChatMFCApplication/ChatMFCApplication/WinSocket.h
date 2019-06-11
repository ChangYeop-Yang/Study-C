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
		void OnSocketServerEventHandler(HWND hWnd, SOCKET sock, WORD eid, WORD err);
		void OnSocketClientEventHandler(HWND hWnd, SOCKET sock, WORD eid, WORD error);
		const CString GetCurrentTimeAndMessage(const CString message);
		const bool OnSendMessage(const SOCKET sock, const std::string message);
		void OnAllSendClientMessage(const std::string message);

	private:
		void OnAccept(HWND hDig, WORD eid, WORD error);
		void OnReceiveMessage(SOCKET sock, HWND hDig, WORD eid, WORD error);
		void OnCloseClientSocket(SOCKET sock, HWND hDig, WORD eid, WORD error);

	// MARK: - Object Methods
	public:
		CListBox * eventListBox;

	private:		
		int szClntAddr;
		char message[BUFSIZ];
		std::unordered_map<SOCKET, User> clients;

		WSADATA wasData;
		SOCKET hServSock;
		SOCKADDR_IN servAddr;
};

#endif