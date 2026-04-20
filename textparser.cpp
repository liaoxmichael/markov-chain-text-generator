#include "textparser.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <deque>
#include <cctype>

int N_GRAM_SIZE = 2;  // size of the n-gram which can be mapped to an output word

TextParser::TextParser(const std::string& training_text_filepath) {
    this.transition_matrix = parseText(training_text_filepath);
}

static std::string removePunctuation(const std::string& punctuated_string) {
    std::string result{};
    for (char c: punctuated_string) {
        if (!std::ispunct(c)) {
            result += c;
        }
    }
    return result;
}

std::unordered_map< std::string, std::unordered_map<std::string, int> >
TextParser::parseText(const std::string& source_filepath) {
    std::ifstream input_text_file{source_filepath};
    if (!input_text_file) {
        throw std::runtime_error("Failed to open " + source_filepath);
    }

    std::unordered_map< std::string, std::unordered_map<std::string, int> > transition_matrix{};

    std::string word{};
    std::deque<std::string> n_gram{};
    // build initial n-gram
    for (int i=0; i < N_GRAM_SIZE && input_text_file >> word; i++) {
        n_gram.push_back(removePunctuation(word));
    }

    while (input_text_file >> word) {
        std::string key{};
        for (const std::string& keyword : n_gram) {
            key += keyword + ' ';
        }
        key.erase(key.length() - 1, 1); // remove trailing space from above join
        n_gram.pop_front();

        transition_matrix[key][word]++;

        n_gram.push_back(removePunctuation(word));
    }
}
