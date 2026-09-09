#include "Logger.h"

#include <stdio.h>

namespace Engine
{
	Logger* Logger::instance = nullptr;

	Logger* Logger::GetInstance(void)
	{
		if (instance == nullptr)
		{
			instance = new Logger();
		}
		return instance;
	}

	void Logger::Info(const char* _string)
	{
		printf(_string);
	}
	void Logger::Warn(const char* _string)
	{
		printf(_string);
	}
	void Logger::Error(const char* _string)
	{
		printf(_string);
	}
	void Logger::Fatal(const char* _string)
	{
		printf(_string);
	}
}