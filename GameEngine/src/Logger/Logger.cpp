#include "Logger.h"

void Logger::Log(const std::string &string)
{
	std::cout<<"LOG::"<<string<<"\n";
}

void Logger::Err(const std::string &string)
{
	std::cerr<<"ERR::"<<string<<"\n";
}