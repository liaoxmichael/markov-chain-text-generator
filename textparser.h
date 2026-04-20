#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <unordered_map>
#include <string>

class TextParser
{
    std::unordered_map< std::string, std::unordered_map<std::string, int> > transition_matrix{};
public:
    TextParser();
};

#endif // TEXTPARSER_H
