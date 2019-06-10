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
	// MARK: - Typedef
	typedef std::pair<std::string, SOCKADDR_IN> User;

	// MARK: - System Methods
	public:
		~WinSocket();

	// MARK: - User Methods
	public:
		void openTCPSocketServer(const int port, HWND hDig);
		void closeTCPSocketServer();
		void OnSocketEvent(HWND hWnd, SOCKET sock, WORD eid, WORD err);
		const CString GetCurrentTimeAndMessage(const CString message);
		const bool OnSendMessage(const SOCKET sock, const std::string message);
		const CString GetServerIP();

	private:
		void OnAccept(HWND hDig, WORD eid, WORD error);
		void OnReceiveMessage(SOCKET sock, HWND hDig, WORD eid, WORD error);
		void OnCloseClientSocket(SOCKET sock, HWND hDig, WORD eid, WORD error);
		void OnAllSendClientMessage(const std::string message);

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