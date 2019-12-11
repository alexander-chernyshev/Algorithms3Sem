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

int main() {
    std::string s;
    std::cin >> s;
    std::vector<size_t> prefix_function = PrefixFunction(s);
    for (auto x : prefix_function) {
        std::cout << x << ' ';
    }
    return 0;
}