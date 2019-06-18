#pragma once

#ifndef WIN_SERIAL_ERROR_H_
#define WIN_SERIAL_ERROR_H_

namespace SerialError {
	enum Error {
		SETUP_SERIAL_PORT_ERROR			= 1,
		CREATE_FILE_HANDLE_ERROR		= 2,
		CREATE_DCB_PADDING_ERROR		= 3,
		SETUP_DCB_PADDING_ERROR			= 4
	};
}

#endif