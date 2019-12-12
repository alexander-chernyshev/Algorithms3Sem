#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

std::vector<size_t> ZFunction(const std::string &s) {
    std::vector<size_t> z_function(s.length());
    size_t left = 0;
    size_t right = 1;
    z_function[0] = s.length();
    for (size_t i = 1; i < s.size(); ++i) {
        if (i < right) {
            z_function[i] = std::min(right - i, z_function[i - left]);
        }
        while (i + z_function[i] < s.length() && s[z_function[i]] == s[i + z_function[i]]) {
            ++z_function[i];
        }
        if (i + z_function[i] > right) {
            left = i;
            right = i + z_function[i];
        }
    }
    return z_function;
}

int main() {
    std::string str;
    std::cin >> str;
    std::vector<size_t> z_function = ZFunction(str);
    for (auto &x : z_function) {
        std::cout << x << ' ';
    }
    return 0;
}