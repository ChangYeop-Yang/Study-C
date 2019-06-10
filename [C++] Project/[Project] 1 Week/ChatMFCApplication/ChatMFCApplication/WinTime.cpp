#include "WinTime.h"

/* Null, because instance will be initialized on demand. */
WinTime * WinTime::shared = nullptr;

WinTime * WinTime::getInstance() {

	if (shared == nullptr) {
		shared = new WinTime();
	}

	return shared;
}

const std::string WinTime::GetCurrentTime() {

	const auto time = std::time(nullptr);
	const auto timeInfo = *std::localtime(&time);

	// MARK: http://www.cplusplus.com/reference/iomanip/put_time/
	std::stringstream stream;
	stream << std::put_time(&timeInfo, "%F %T");

	return stream.str;
}