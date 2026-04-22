#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <unordered_map>
#include <string>

struct CumulativeWordDistribution {
    // we use a cumulative distribution to allow us to find a word given a random 0-1 
    std::vector< std::pair<std::string, double> > cumulative_distribution;
};

class TextParser
{
    std::unordered_map< std::string, CumulativeWordDistribution > transition_matrix{};
    std::vector<std::string> keys{};
public:
    TextParser(const std::string &training_text_filepath);

    static std::string preprocessWord(const std::string &word);
    std::string generateText(int length_of_output);

private:
    void parseText(const std::string &source_filepath);
};

#endif // TEXTPARSER_H
