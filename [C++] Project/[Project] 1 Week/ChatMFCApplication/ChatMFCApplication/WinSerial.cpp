#include "stdafx.h"
#include "WinSerial.h"

// MARK: - System Methods
WinSerial::WinSerial(const S_PORT port) {

	try {

	}
	catch (const int exception) {

		switch (exception) {
			case 1: 
				break;
		}

	}

	this->handler = CreateFileA(static_cast<LPCSTR>(port.first.c_str()),
								GENERIC_READ | GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	if (this->handler == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			// port error here
		} else {

		}
	} else {
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(this->handler, &dcbSerialParameters)) {  }
		else {
			dcbSerialParameters.BaudRate = CBR_9600; // BandWidth
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handler, &dcbSerialParameters)) {}
			else {
				this->connected = true;
				
				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				
				// here thread
				std::thread serial_thread(&WinSerial::ReadMessageSerial, this);
				serial_thread.join();

				Sleep(SERIAL_TIME_OUT);
			}
		}
	}

}

WinSerial::~WinSerial() {
	if (this->connected) {
		CloseHandle(this->handler);
	}
}

// MARK: - User Methods
void WinSerial::ReadMessageSerial() {

	DWORD bytesRead;
	UINT toRead = 0;

	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue > 0) {
		if (this->status.cbInQue > BUFSIZ) {
			toRead = BUFSIZ;
		} else {
			toRead = this->status.cbInQue;
		}
	}

	while (true) {

		// MARK: Safe Thread Zone (Critical Section)
		this->mutex_lock.lock();
		{
			const int length = ReadFile(this->handler, this->message, toRead, &bytesRead, NULL);
		}
		this->mutex_lock.unlock();

	}
}

const bool WinSerial::WriteMessageSerial() {
	return false;
}

const bool WinSerial::GetConnectedSerialState() {
	return this->connected;
}