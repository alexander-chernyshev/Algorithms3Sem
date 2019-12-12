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

bool ContainsString(const std::string& string, const std::string& pattern) {
    size_t str_length = string.size();
    size_t pattern_length = pattern.size();
    std::string tmp = pattern;
    tmp += "|";
    tmp += string;
    std::vector<size_t> prefix_function = PrefixFunction(tmp);
    for (size_t i = 0; i < str_length; ++i) {
        if (prefix_function[pattern_length + i + 1] == pattern_length) {
            return true;
        }
    }
    return false;
}

bool CheckPossibility(const std::string& first_state, const std::string& second_state, char last_char) {
    std::string tmp = second_state + last_char + second_state;
    return ContainsString(tmp, first_state);
}

enum Prediction {
    YES, NO, RANDOM
};

Prediction PredictResult(const std::string& first_state, const std::string& second_state) {
    bool zero_possible = CheckPossibility(first_state, second_state, '0');
    bool one_possible = CheckPossibility(first_state, second_state, '1');
    if (zero_possible && one_possible) {
        return RANDOM;
    }
    if (zero_possible) {
        return NO;
    }
    if (one_possible) {
        return YES;
    }
    return RANDOM;
}

int main() {
    size_t n;
    std::cin >> n;
    std::string first_state, second_state;
    std::cin >> first_state >> second_state;
    Prediction result = PredictResult(first_state, second_state);
    if (result == RANDOM) {
        std::cout << "Random";
    }
    if (result == YES) {
        std::cout << "Yes";
    }
    if (result == NO) {
        std::cout << "No";
    }
    return 0;
}