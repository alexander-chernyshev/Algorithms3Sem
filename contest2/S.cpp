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

size_t CountDistinctSubstrings(const std::string& s) {
    if (s.length() == 0) {
        return 0;
    }
    std::string d;
    d += s[0];
    size_t count = 1;
    for (size_t i = 1; i < s.length(); ++i) {
        d+=s[i];
        std::string tmp = d;
        std::reverse(tmp.begin(), tmp.end());
        std::vector<size_t> pf = PrefixFunction(tmp);
        size_t pmax = 0;
        for (auto x : pf) {
            if (x > pmax) {
                pmax = x;
            }
        }
        count += (d.length() - pmax);
    }
    return count;
}

int main() {
    std::string str;
    std::cin >> str;
    std::cout << CountDistinctSubstrings(str);
    return 0;
}