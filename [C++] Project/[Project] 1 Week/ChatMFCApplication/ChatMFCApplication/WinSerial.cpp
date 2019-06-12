#include "stdafx.h"
#include "WinSerial.h"

// MARK: - System Methods
WinSerial::WinSerial(const S_PORT port) {

}

WinSerial::~WinSerial() {

}

// MARK: - User Methods
std::string WinSerial::ReadMessageSerial() {
	return "";
}

const bool WinSerial::WriteMessageSerial() {
	return false;
}

const bool WinSerial::GetConnectedSerialState() {
	return false;
}