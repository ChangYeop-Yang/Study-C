#pragma once

#ifndef WINTIME_H_
#define WINTIME_H_

#include <ctime>
#include <iostream>

class WinTime 
{
	// MARK: - System Methods
	private	:
		WinTime();

	// MARK: - Object Variables
	private :
		static WinTime * shared;

	// MARK: - User Methods
	public :
		static WinTime * getInstance();
		const CString GetCurrentTime_String();
};

#endif