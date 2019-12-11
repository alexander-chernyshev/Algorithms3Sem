#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

std::vector<size_t> PrefixFunction(const std::string& s) {
    std::vector<size_t> function_values(s.size());
    for (size_t i = 1; i < s.size(); ++i) {
        size_t k = function_values[i - 1];
        while (k > 0 && s[i] != s[k]) {
            k = function_values[k-1];
        }
        if (s[i] == s[k]) {
            ++k;
        }
        function_values[i] = k;
    }
    return function_values;
}

std::vector<size_t> KnuthMorrisPratt(const std::string& string, const std::string& pattern) {
    size_t str_length = string.size();
    size_t pattern_length = pattern.size();
    std::string tmp = pattern;
    tmp += "|";
    tmp += string;
    std::vector<size_t> prefix_function = PrefixFunction(tmp);
    std::vector<size_t> positions;
    for (size_t i = 0; i < str_length; ++i) {
        if (prefix_function[pattern_length + i + 1] == pattern_length) {
            positions.push_back(i - pattern_length + 1);
        }
    }
    return positions;
}

int main() {
    std::string pattern;
    std::string string;
    std::cin >> string >> pattern;
    std::vector<size_t> positions = KnuthMorrisPratt(string, pattern);
    for (auto x : positions) {
        std::cout << x << '\n';
    }
}