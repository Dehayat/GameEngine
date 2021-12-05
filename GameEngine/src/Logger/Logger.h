#pragma once
#include <iostream>

class Logger
{
public:
	static void Log(const std::string &string);
	static void Err(const std::string &string);
};