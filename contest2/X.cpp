#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

std::vector<size_t> ZFunction(const std::vector<size_t> &s) {
    std::vector<size_t> z_function(s.size());
    size_t left = 0;
    size_t right = 1;
    z_function[0] = s.size();
    for (size_t i = 1; i < s.size(); ++i) {
        if (i < right) {
            z_function[i] = std::min(right - i, z_function[i - left]);
        }
        while (i + z_function[i] < s.size() && s[z_function[i]] == s[i + z_function[i]]) {
            ++z_function[i];
        }
        if (i + z_function[i] > right) {
            left = i;
            right = i + z_function[i];
        }
    }
    return z_function;
}

std::vector<size_t> AFunction(std::vector<size_t> s) {
    size_t len = s.size();
    std::vector<size_t > r_s = s;
    std::reverse(r_s.begin(), r_s.end());
    s.resize(s.size()*2);
    for (auto& x : r_s) {
        s.push_back(x);
    }
    std::vector<size_t> z_function = ZFunction(s);
    std::vector<size_t> a_function(z_function.begin() + len, z_function.end());
    return std::vector<size_t>(a_function.rbegin(), a_function.rend());
}

std::vector<size_t> GetPossibleBrickCount(const std::vector<size_t>& visible_bricks) {
    std::vector<size_t> a_function = AFunction(visible_bricks);
    std::vector<size_t> possible_count;
    possible_count.push_back(visible_bricks.size());
    for (size_t i = 1; i <= visible_bricks.size() / 2; ++i) {
        if (a_function[i*2 - 1] == i * 2) {
            possible_count.push_back(visible_bricks.size() - i);
        }
    }
    return possible_count;
}

int main() {
    size_t len, color_count;
    std::cin >> len >> color_count;
    std::vector<size_t> visible_bricks(len);
    for (auto& x : visible_bricks) {
        std::cin >> x;
    }
    std::vector<size_t> possible_count = GetPossibleBrickCount(visible_bricks);
    for (auto x = possible_count.rbegin(); x != possible_count.rend(); ++x) {
        std::cout << *x << ' ';
    }
    return 0;
}