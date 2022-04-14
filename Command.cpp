#include "Command.h"

CommandParser::CommandParser()
{

}

std::vector<std::string> CommandParser::tokenise(std::string Input, char separator)
{
    std::vector<std::string> tokens;
    signed int begin, end;
    std::string token;

    begin = Input.find_first_not_of(separator, 0);
    do{
        end = Input.find_first_of(separator, begin);
        if(begin == Input.length() || begin == end) break;
        if(end >= 0) token = Input.substr(begin, end - begin);
        else token = Input.substr(begin, Input.length() - begin);
        tokens.push_back(token);
        begin = end + 1; 

    }while(end>0);
    
    return tokens;
}
