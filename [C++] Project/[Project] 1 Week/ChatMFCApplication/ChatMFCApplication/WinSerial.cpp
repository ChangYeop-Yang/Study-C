#include "stdafx.h"
#include "WinSerial.h"

WinSerial::WinSerial(const S_PORT port, HWND hWnd) {

	try {

		this->handler = CreateFileA(static_cast<LPCSTR>(port.second.c_str()),
									GENERIC_READ | GENERIC_WRITE,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
									NULL);

		if (this->handler == INVALID_HANDLE_VALUE) {
			if (GetLastError() == ERROR_FILE_NOT_FOUND) {
				// port error here
			}
			else {
				
			}
		}
		else {

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
					timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
					timeouts.ReadTotalTimeoutMultiplier = 0;
					timeouts.ReadTotalTimeoutConstant = 0;
					timeouts.WriteTotalTimeoutConstant = 5000;
					timeouts.WriteTotalTimeoutMultiplier = 0;

					SetCommTimeouts(this->handler, &timeouts);

					this->connected = true;

					//
					this->overlaped_event.second.Offset = 0;
					this->overlaped_event.second.OffsetHigh = 0;
					this->overlaped_event.second.hEvent = CreateEvent(0, 1, 0, 0);

					//
					this->overlaped_event.first.Offset = 0;
					this->overlaped_event.first.OffsetHigh = 0;
					this->overlaped_event.first.hEvent = CreateEvent(0, 1, 0, 0);

					SetCommMask(this->handler, EV_RXCHAR);

					PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

					auto afx_Thread = AfxBeginThread((AFX_THREADPROC) ReadMessageSerial, (LPVOID) this);
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

UINT WinSerial::ReadMessageSerial(LPVOID _mothod) {

	// MARK: https://github.com/xanthium-enterprises/Serial-Programming-Win32API-C/blob/master/USB2SERIAL_Read/Reciever%20(PC%20Side)/USB2SERIAL_Read_W32.c
	auto handle = (WinSerial *) _mothod;

	char message[BUFSIZ];
	while (handle->connected) {

		if ( WaitCommEvent(handle->handler, &handle->dwEventMask, 0) ) {
			handle->critical.Lock();
			{
				DWORD read_Byte = 0;
				std::memset(message, 0, sizeof(message));

				if (handle->dwEventMask & EV_RXCHAR) {

					const auto isSuccess = ReadFile(handle->handler, &message, BUFSIZ, &read_Byte, &handle->overlaped_event.first);
					if (isSuccess) {
						CString * msg = new CString(message);
						PostMessage(handle->hWindow, 19, NULL, (LPARAM)msg);
						std::cout << message << std::endl;
					}
				}

				
			}
			handle->critical.Unlock();
		}
	}
	
	handle->WinSerial::CloseWinSerial();
	return 0;
}

const bool WinSerial::WriteMessageSerial(const std::string message) {

	DWORD byteSend;

	this->critical.Lock();
	{
		WriteFile(this->handler, (void *) message.c_str(), message.size(), &byteSend, &this->overlaped_event.second);
	}
	this->critical.Unlock();

	return false;
}

const bool WinSerial::GetConnectedSerialState() {
	return this->connected;
}

void WinSerial::CloseWinSerial() {
	if (this->connected) {
		// MARK: http://www.devpia.com/MAEUL/Contents/Detail.aspx?BoardID=50&MAEULNo=20&no=232925&ref=232925
		PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		CloseHandle(this->handler);
	}
}