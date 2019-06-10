#pragma once

#ifndef WINSOCK_H_
#define WINSOCK_H_

#include <memory>
#include <iostream>
#include <unordered_map>

#include <WinSock2.h>

#define MAX_REQUEST_QUEUE_SIZE 5

#define MWM_EVENT_SOCK (WM_USER + 1)

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using std::cout;
using std::endl;

class WinSocket
{
	// MARK: - System Methods
	public:
		~WinSocket();

	// MARK: - User Methods
	public:
		void openTCPSocketServer(const int port, HWND hDig);
		void closeTCPSocketServer();
		void OnSocketEvent(HWND hWnd, SOCKET sock, WORD eid, WORD err);
		const bool OnSendMessage(const SOCKET sock, const std::string message);

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

		WSADATA wasData;
		SOCKET hServSock, hClntSock;
		SOCKADDR_IN servAddr, clntAddr;
};

#endif