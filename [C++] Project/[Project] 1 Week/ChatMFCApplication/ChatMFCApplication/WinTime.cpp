#include "stdafx.h"
#include "WinTime.h"

/* Null, because instance will be initialized on demand. */
WinTime * WinTime::shared = nullptr;

WinTime * WinTime::getInstance() {

	if (shared == nullptr) {
		shared = new WinTime();
	}

	return shared;
}

const CString WinTime::GetCurrentTime_String() {

	const auto now = CTime::GetCurrentTime();
	
	CString time_str;
	time_str.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond());

	return time_str;
}