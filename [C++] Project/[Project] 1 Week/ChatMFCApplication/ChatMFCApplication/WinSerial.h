#pragma once

#ifndef WIN_SERIAL_H
#define WIN_SERIAL_H

#include <string>
#include <thread>
#include <iostream>
#include <windows.h>

#define SERIAL_TIME_OUT 1000
#define SERIAL_RECIVE_MESSAGE (WM_USER + 3);

// MARK: - Typedef
typedef std::pair<std::string, std::string> S_PORT;

class WinSerial
{
	// MARK: - System Methods
	public:
		WinSerial(const S_PORT port, HWND hWnd);
		~WinSerial();

	// MARK: - User Methods
	public:
		void ReadMessageSerial();
		void CloseWinSerial();
		const bool	WriteMessageSerial(const std::string message);
		const bool	GetConnectedSerialState();

		static UINT ThreadFirst(LPVOID _mothod);

	// MARK: - Object Variables
	private:
		HANDLE	handler;
		COMSTAT status;
		DWORD	errors;
		HWND	hWindow;
		bool	connected;
		OVERLAPPED m_osRead;

		BOOL  Status;                          // Status of the various operations 
		DWORD dwEventMask;                     // Event mask to trigger
		DWORD NoBytesRead;                     // Bytes read by ReadFile()

		char message[BUFSIZ];
};

#endif