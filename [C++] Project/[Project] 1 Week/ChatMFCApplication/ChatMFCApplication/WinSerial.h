#pragma once

#ifndef WIN_SERIAL_H
#define WIN_SERIAL_H

#include <string>
#include <iostream>
#include <windows.h>

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
		std::string ReadMessageSerial();
		const bool	WriteMessageSerial();
		const bool	GetConnectedSerialState();
};

#endif