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

size_t GetMaxSuffix(const std::string& divided_string) {
    std::vector<size_t> prefix_values = PrefixFunction(divided_string);
    return prefix_values[prefix_values.size()-1];
}

int main() {
    size_t count;
    std::cin >> count;
    std::string message;
    for (size_t i = 0; i < count; ++i) {
        if (!i) {
            std::cin >> message;
        } else {
            std::string add;
            std::cin >> add;
            std::string tmp = add + '|';
            if (message.length() < add.length()) {
                tmp += message;
            } else {
                tmp += message.substr(message.length()-add.length(), add.length());
            }
            size_t suffix_len = GetMaxSuffix(tmp);
            message += add.substr(suffix_len);
        }
    }
    std::cout << message;
    return 0;
}