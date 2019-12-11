#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

class Alphabet {
private:
    size_t alphabet_mask = (1ull << 26u) - 1;
public:
    Alphabet() { alphabet_mask = (1ull << 26u) - 1; };

    void RemoveLetter(char c) {
        size_t index = c - 'a';
        alphabet_mask = alphabet_mask & ~(1u << index);
    }

    char GetAvailableLetter() {
        size_t index = 0;
        while ((alphabet_mask & (1u << index)) == 0) {
            ++index;
        }
        return char('a' + index);
    }
};

std::string MinimalStringFromPrefix(const std::vector<size_t> &prefix_function) {
    if (prefix_function.empty()) {
        return "";
    }
    size_t size = prefix_function.size();
    std::string s = "a";
    std::vector<Alphabet> alphabet_vector;
    alphabet_vector.resize(size);
    for (size_t i = 1; i < size; ++i) {
        if (prefix_function[i] == 0) {
            size_t prefix_len = prefix_function[i - 1];
            if (prefix_len < i) {
                for (size_t suff_size = 1; suff_size <= prefix_len; ++suff_size) {
                    bool match = true;
                    for (size_t j = 0; j < suff_size; ++j) {
                        if (s[j] != s[i - suff_size + j]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        alphabet_vector[i].RemoveLetter(s[suff_size]);
                    }
                }
            }
            alphabet_vector[i].RemoveLetter('a');
            s += alphabet_vector[i].GetAvailableLetter();
        } else {
            s += s[prefix_function[i] - 1];
        }
    }
    return s;
}

int main() {
    size_t length;
    std::cin >> length;
    std::vector<size_t> prefix_function(length);
    for (auto &x : prefix_function) {
        std::cin >> x;
    }
    std::cout << MinimalStringFromPrefix(prefix_function);
    return 0;
}