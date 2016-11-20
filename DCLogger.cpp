#include "DCLogger.h"
#include <iostream>

namespace DevilsCanvas
{
	DCLogger::DCLogger()
	{
		std::ofstream clearFile;
		clearFile.open("Log.txt", std::ios::out | std::ios::trunc);
		clearFile.close();

		m_stream.open("Log.txt", std::ios::out | std::ios::app);
	}

	DCLogger::~DCLogger()
	{
	}

	DCLogger& DCLogger::GetLogger()
	{
		static DCLogger s_logger;
		return s_logger;
	}

	void DCLogger::LogToConsole(const std::string iString)
	{
		std::cout << iString << std::endl;
	}

	void DCLogger::LogToConsole(const int iData)
	{
		std::cout << iData << std::endl;
	}

	void DCLogger::LogToConsole(const float iData)
	{
		std::cout << iData << std::endl;
	}

	void DCLogger::LogToConsole(const double iData)
	{
		std::cout << iData << std::endl;
	}

	void DCLogger::LogToFile(const std::string iString)
	{
		m_stream << iString << std::endl;
	}

	void DCLogger::LogToFile(const int iData)
	{
		m_stream << iData << std::endl;
	}

	void DCLogger::LogToFile(const float iData)
	{
		m_stream << iData << std::endl;
	}

	void DCLogger::LogToFile(const double iData)
	{
		m_stream << iData << std::endl;
	}

}
