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

std::vector<size_t> AFunction(std::string s) {
    size_t len = s.length();
    std::string r_s = s;
    std::reverse(r_s.begin(), r_s.end());
    s += r_s;
    std::vector<size_t> z_function = ZFunction(s);
    std::vector<size_t> a_function(z_function.begin() + len, z_function.end());
    return std::vector<size_t>(a_function.rbegin(), a_function.rend());
}

int main() {
    size_t length;
    std::cin >> length;
    std::string s;
    std::cin >> s;
    std::vector<size_t> a_function = AFunction(s);
    for (auto x : a_function) {
        std::cout << x << ' ';
    }
    return 0;
}