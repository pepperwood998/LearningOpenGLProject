#include "Log.hpp"
#include <iostream>
#include <string>

void unnamed::Log (const char *tag, const char *message)
{
    std::cout << tag << "\n>> " << message << std::endl;
}

void unnamed::SimpleLog (const char *message)
{
    std::cout << message << std::endl;
}

void unnamed::SimpleLog (float message)
{
    SimpleLog(std::to_string(message).c_str());
}
