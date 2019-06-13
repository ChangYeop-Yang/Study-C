#include "stdafx.h"
#include "WinSerial.h"
// https://github.com/xanthium-enterprises/Serial-Programming-Win32API-C/blob/master/USB2SERIAL_Read/Reciever%20(PC%20Side)/USB2SERIAL_Read_W32.c
// MARK: - System Methods https://goodjian.tistory.com/entry/%EC%8B%9C%EB%A6%AC%EC%96%BC-%ED%86%B5%EC%8B%A0OVERLAPPED-%EA%B5%AC%EC%A1%B0%EC%B2%B4%EB%A5%BC-%EC%9D%B4%EC%9A%A9%ED%95%98%EC%97%AC-%EB%B9%84%EB%8F%99%EA%B8%B0-read%EB%A5%BC-%ED%95%B4%EB%B3%B4%EC%9E%90?category=78065
WinSerial::WinSerial(const S_PORT port, HWND hWnd) {

	

	try {

		this->handler = CreateFileA(static_cast<LPCSTR>("COM1"),
									GENERIC_READ | GENERIC_WRITE,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
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
			DCB dcbSerialParameters = { 0 };

			if (!GetCommState(this->handler, &dcbSerialParameters)) {}
			else {
				dcbSerialParameters.BaudRate = CBR_115200; // BandWidth
				dcbSerialParameters.ByteSize = 8;
				dcbSerialParameters.StopBits = ONESTOPBIT;
				dcbSerialParameters.Parity = NOPARITY;
				dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

				if (!SetCommState(handler, &dcbSerialParameters)) {}
				else {

					this->connected = true;
					PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

					//AfxBeginThread(ThreadFirst, this);
					//winThread->m_bAutoDelete = false;

					this->hWindow = hWnd;

					Sleep(SERIAL_TIME_OUT);
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

	DWORD bytesRead;
	DWORD byte;
	UINT toRead = 0;

	auto handle = (WinSerial *) _mothod;
	
	char msg[BUFSIZ];

	while (handle->connected) {
		WaitCommEvent(handle->handler, &bytesRead, NULL);
		if ((bytesRead & EV_RXCHAR) == EV_RXCHAR) {
			ReadFile(handle->handler, msg, toRead, &byte, NULL);

			std::cout << msg << std::endl;
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