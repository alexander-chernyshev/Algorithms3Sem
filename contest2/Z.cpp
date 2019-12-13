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

std::string MinimalStringFromZFunction(const std::vector<size_t> &z_function) {
    size_t size = z_function.size();
    std::vector<Alphabet> alphabet_vector;
    std::vector<std::vector<char>> chars_to_remove(size);
    alphabet_vector.resize(size);
    std::string s = "a";
    size_t prefix_length = 0;
    size_t j = 0;
    for (size_t i = 1; i < size; ++i) {
        if (z_function[i] == 0 && prefix_length == 0) {
            alphabet_vector[i].RemoveLetter('a');
            for (auto x : chars_to_remove[i]) {
                alphabet_vector[i].RemoveLetter(x);
            }
            s += alphabet_vector[i].GetAvailableLetter();
        }
        if (z_function[i] > prefix_length) {
            prefix_length = z_function[i];
            j = 0;
        }
        if (prefix_length > 0) {
            s += s[j];
            ++j;
            --prefix_length;
        }
        if (z_function[i] > 0) {
            if (i + z_function[i] < size && i >= z_function[i]) {
                chars_to_remove[i + z_function[i]].push_back(s[z_function[i]]);
            }
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
    std::cout << MinimalStringFromZFunction(prefix_function);
    return 0;
}