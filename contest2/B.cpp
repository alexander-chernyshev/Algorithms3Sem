#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>

const size_t ARR_SIZE = 1ull << 24u;

unsigned int a, b; // даны во входных данных
unsigned int cur = 0; // беззнаковое 32-битное число
unsigned int NextRand() {
    cur = cur * a + b; // вычисляется с переполнениями
    return cur >> 8u; // число от 0 до 224-1.
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int add_query_count, sum_query_count;
    std::cin >> add_query_count >> sum_query_count;
    std::cin >> a >> b;
    std::vector<unsigned int> prefix_sum(ARR_SIZE);
    std::vector<unsigned int> adds(ARR_SIZE);
    unsigned int left, right, add;
    for (size_t i = 0; i < add_query_count; ++i) {
        add = NextRand();
        left = NextRand();
        right = NextRand();
        if (left > right) {
            std::swap(left, right);
        }
        adds[left] += add;
        adds[right +1] -= add;
    }
    unsigned int value = adds[0];
    prefix_sum[0] = adds[0];
    for (size_t i = 1; i < ARR_SIZE; ++i) {
        value += adds[i];
        prefix_sum[i] = prefix_sum[i - 1] + value;
    }
    unsigned int query_sum = 0;
    for (size_t i = 0; i < sum_query_count; ++i) {
        left = NextRand();
        right = NextRand();
        if (left > right) {
            std::swap(left, right);
        }
        if (left == 0) {
            query_sum += prefix_sum[right];
        } else {
            query_sum += (prefix_sum[right] - prefix_sum[left - 1]);
        }
    }
    std::cout << query_sum << '\n';
    return 0;
}