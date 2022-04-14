#pragma once
#include <string>
#include <vector>
#include <iostream>

class CommandParser
{
    public: 
    CommandParser();
    static std::vector<std::string> tokenise(std::string Input, char separator);
};