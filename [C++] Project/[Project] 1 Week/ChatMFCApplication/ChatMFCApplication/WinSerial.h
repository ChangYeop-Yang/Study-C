#pragma once

#ifndef WIN_SERIAL_H
#define WIN_SERIAL_H

#include <mutex>
#include <string>
#include <thread>
#include <iostream>
#include <windows.h>

#define SERIAL_TIME_OUT 1000

// MARK: - Typedef
typedef std::pair<std::string, std::string> S_PORT;

class WinSerial
{
	// MARK: - System Methods
	public:
		WinSerial(const S_PORT port);
		~WinSerial();

	// MARK: - User Methods
	public:
		void ReadMessageSerial();
		const bool	WriteMessageSerial();
		const bool	GetConnectedSerialState();

	// MARK: - Object Variables
	private:
		HANDLE	handler;
		COMSTAT status;
		DWORD	errors;
		bool	connected;

		std::mutex mutex_lock;

		char message[BUFSIZ];
};

#endif