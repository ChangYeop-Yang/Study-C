#include "stdafx.h"
#include "WinSerial.h"

WinSerial::WinSerial(const S_PORT port, HWND hWnd) {

	

	try {

		this->handler = CreateFileA(static_cast<LPCSTR>("COM1"),
									GENERIC_READ | GENERIC_WRITE,
									0,
									NULL,
									OPEN_EXISTING,
									0,
									NULL);

		this->m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		if (this->handler == INVALID_HANDLE_VALUE) {
			if (GetLastError() == ERROR_FILE_NOT_FOUND) {
				// port error here
			}
			else {
				
			}
		}
		else {

			std::memset(&m_osRead, 0, sizeof(m_osRead));
			m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

			DCB dcbSerialParameters = { 0 };

			if (!GetCommState(this->handler, &dcbSerialParameters)) {}
			else {
				dcbSerialParameters.DCBlength = sizeof(dcbSerialParameters);
				dcbSerialParameters.BaudRate = CBR_115200; // BandWidth
				dcbSerialParameters.ByteSize = 8;
				dcbSerialParameters.StopBits = ONESTOPBIT;
				dcbSerialParameters.Parity = NOPARITY;
				dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

				if (!SetCommState(handler, &dcbSerialParameters)) {}
				else {

					/*------------------------------------ Setting Timeouts --------------------------------------------------*/
					COMMTIMEOUTS timeouts = { 0 };
					timeouts.ReadIntervalTimeout = 50;
					timeouts.ReadTotalTimeoutMultiplier = 10;
					timeouts.ReadTotalTimeoutConstant = 50;

					SetCommTimeouts(this->handler, &timeouts);

					this->connected = true;

					SetCommMask(this->handler, EV_RXCHAR);

					

					//PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

					auto afx_Thread = AfxBeginThread((AFX_THREADPROC) ThreadFirst, (LPVOID) this);
					//afx_Thread->m_bAutoDelete = false;

					this->hWindow = hWnd;

					//Sleep(SERIAL_TIME_OUT);
				}
			}
		}

	}
	catch (const int exception) {

		switch (exception) {
			case 1: 
				break;
		}

		exit(false);
	}

}

WinSerial::~WinSerial() {
	WinSerial::CloseWinSerial();
}

// MARK: - User Methods
UINT WinSerial::ThreadFirst(LPVOID _mothod) {

	// MARK: https://github.com/xanthium-enterprises/Serial-Programming-Win32API-C/blob/master/USB2SERIAL_Read/Reciever%20(PC%20Side)/USB2SERIAL_Read_W32.c
	auto handle = (WinSerial *) _mothod;
	
	while (true) {
		if (WaitCommEvent(handle->handler, &handle->dwEventMask, NULL)) {

			int ii = 0;
			DWORD  read = 0;
			char mChar;
			std::string msg = std::string();

			std::memset(handle->message, 0, sizeof(handle->message));
			//do {
				ReadFile(handle->handler, &handle->message, sizeof(handle->message), &read, NULL);
				std::cout << handle->message << std::endl;
			//} while (read > 0);

		}
	}
	
	return 0;
}

void WinSerial::ReadMessageSerial() {

	DWORD bytesRead;
	UINT toRead = 0;

	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue > 0) {
		if (this->status.cbInQue > BUFSIZ) {
			toRead = BUFSIZ;
		}
		else {
			toRead = this->status.cbInQue;
		}
	}

	std::string mm;

	//while (this->connected) {
		// MARK: Safe Thread Zone (Critical Section)
			ReadFile(this->handler, this->message, toRead, &bytesRead, NULL);
			std::cout << this->message << std::endl;

			CString msg = CString("msg");
			PostMessage(this->hWindow, 19, (WPARAM) &msg, 10);
		
	//}
}

const bool WinSerial::WriteMessageSerial(const std::string message) {

	DWORD byteSend;

	WriteFile(this->handler, (void *) message.c_str(), message.size(), &byteSend, 0);

	return false;
}

const bool WinSerial::GetConnectedSerialState() {
	return this->connected;
}

void WinSerial::CloseWinSerial() {
	if (this->connected) {
		CloseHandle(this->handler);
	}
}