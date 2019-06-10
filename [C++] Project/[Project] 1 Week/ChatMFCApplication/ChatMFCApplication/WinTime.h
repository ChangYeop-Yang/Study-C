#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

class WinTime 
{
	// MARK: - System Methods
	private	:
		WinTime();
		~WinTime();

	// MARK: - Object Variables
	private :
		static WinTime * shared;

	// MARK: - User Methods
	public :
		static WinTime * getInstance();
		const std::string GetCurrentTime();
};