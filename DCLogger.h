#ifndef DCLogger_h
#define DCLogger_h

#include <string>
#include <fstream>

namespace DevilsCanvas 
{
#define LOG_TO_FILE(MESSAGE)	\
	DCLogger::GetLogger().LogToFile(#MESSAGE);

#define LOG_TO_CONSOLE(MESSAGE)	\
	DCLogger::GetLogger().LogToConsole(#MESSAGE);

#define LOG_TO_FILE_NEW(MESSAGE)	\
	DCLogger::GetLogger().LogToFile(MESSAGE);

	class DCLogger
	{
	public:
		~DCLogger();
		static DCLogger& GetLogger();
		void LogToConsole(const std::string iString);
		void LogToConsole(const int iData);
		void LogToConsole(const float iData);
		void LogToConsole(const double iData);

		void LogToFile(const std::string iString);
		void LogToFile(const int iData);
		void LogToFile(const float iData);
		void LogToFile(const double iData);

	private:
		DCLogger();
		DCLogger(const DCLogger&);
		DCLogger& operator=(const DCLogger&);

		std::ofstream m_stream;
	};

}

#endif
