#pragma once

#ifndef WIN_SERIAL_H
#define WIN_SERIAL_H

#include <string>
#include <thread>
#include <afxmt.h>
#include <iostream>
#include <windows.h>

#define SERIAL_TIME_OUT 1000
#define SERIAL_RECIVE_MESSAGE (WM_USER + 3);

// MARK: - Typedef
typedef std::pair<std::string, std::string> S_PORT;
typedef std::pair<OVERLAPPED, OVERLAPPED> EVENT;

class WinSerial
{
	// MARK: - System Methods
	public:
		WinSerial(const S_PORT port, HWND hWnd);
		~WinSerial();

	// MARK: - User Methods
	public:
		void CloseWinSerial();
		const bool	WriteMessageSerial(const std::string message);
		const bool	GetConnectedSerialState();

		static UINT ReadMessageSerial(LPVOID _mothod);

	// MARK: - Object Variables
	public:
		bool	connected;

		CCriticalSection critical;

	private:
		HANDLE	handler;
		COMSTAT status;
		DWORD	errors;
		HWND	hWindow;
		
		EVENT overlaped_event;

		BOOL  Status;                          // Status of the various operations 
		DWORD dwEventMask;                     // Event mask to trigger
		DWORD NoBytesRead;                     // Bytes read by ReadFile()
};

#endif