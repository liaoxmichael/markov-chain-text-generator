#include "textparser.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <deque>
#include <cctype>
#include <random>
#include <functional>

int N_GRAM_SIZE = 2;  // size of the n-gram which can be mapped to an output word

TextParser::TextParser(const std::string& training_text_filepath) {
    parseText(training_text_filepath);
}

std::string TextParser::preprocessWord(const std::string& word) {
    std::string result{};
    for (char c : word) {
        result += std::tolower(c);
    }
    return result;
}

void TextParser::parseText(const std::string& source_filepath) {
    std::ifstream input_text_file{source_filepath};
    if (!input_text_file) {
        throw std::runtime_error("Failed to open " + source_filepath);
    }
    std::unordered_map<std::string, std::unordered_map<std::string, int>> word_counts{};
    std::string word{};
    std::deque<std::string> n_gram{};
    // build initial n-gram
    for (int i=0; i < N_GRAM_SIZE && input_text_file >> word; i++) {
        std::string processed = preprocessWord(word);
        n_gram.push_back(processed);
    }

    while (input_text_file >> word) {
        std::string processed = preprocessWord(word);
        
        std::string key{};
        for (const std::string& keyword : n_gram) {
            key += keyword + ' ';
        }
        key.erase(key.length() - 1, 1); // remove trailing space from above join
        n_gram.pop_front();

        word_counts[key][processed]++;
        this->keys.push_back(key);

        n_gram.push_back(processed);
    }

    // convert word counts to cumulative distribution
    for (const auto& pair : word_counts) {
        const std::string& key = pair.first;
        const auto& word_count_map = pair.second;
        double total_count = 0.0;

        for (const auto& count_pair : word_count_map) {
            total_count += count_pair.second;
        }

        double cumulative_probability = 0.0;
        for (const auto& count_pair : word_count_map) {
            const std::string& word = count_pair.first;
            int count = count_pair.second;
            cumulative_probability += count / total_count;
            this->transition_matrix[key].cumulative_distribution.push_back({word, cumulative_probability});
        }
    }
}

std::string TextParser::generateText(int length_of_output) {
    // 1. randomly select a key to start with
    std::default_random_engine generator;
    std::uniform_int_distribution<int> int_distribution(0, this->keys.size() - 1);
    auto roll_random_index = std::bind(int_distribution, generator);
    std::string current_key = this->keys[roll_random_index()];

    // 2. repeatedly select words following probabilities
    std::uniform_real_distribution<double> real_distribution(0.0, 1.0);
    auto roll_random_probability = std::bind(real_distribution, generator);
    std::string output_text{};
    for (int i=0; i < length_of_output; i++) {
        // need to guard against generating a key that doesn't exist in the transition matrix
        if (this->transition_matrix.find(current_key) == this->transition_matrix.end()) {
            // in lieu of terminating, we will add a full stop and start with a new random key
            output_text.erase(output_text.length() - 1, 1);
            output_text += ". ";
            current_key = this->keys[roll_random_index()];
        }
        double random_prob = roll_random_probability();
        for (const auto& word_dist_pair : this->transition_matrix[current_key].cumulative_distribution) {
            const std::string& word = word_dist_pair.first;
            double cumulative_prob = word_dist_pair.second;
            if (random_prob <= cumulative_prob) {
                output_text += word + ' ';

                size_t space_pos = current_key.find(' ');
                if (space_pos != std::string::npos) {
                    current_key = current_key.substr(space_pos + 1) + ' ' + word;
                } else {
                    current_key = word;  // No space found, so this is unigram
                }
                break;
            }
        }
    }
    
    return output_text;
}