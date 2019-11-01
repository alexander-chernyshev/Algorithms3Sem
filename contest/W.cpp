#include <iostream>
#include <vector>
#include <algorithm>

const size_t MAX_PATH = 100000000;

std::vector<std::vector<size_t>> MakePathMatrix(size_t vertex_count) {
    std::vector<std::vector<size_t>> matrix(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        matrix[i].resize(vertex_count);
    }
    return matrix;
}

void Floyd(size_t vertex_count, std::vector<std::vector<size_t>> &path_matrix) {
    for (size_t i = 0; i < vertex_count; ++i) {
        for (size_t u = 0; u < vertex_count; ++u) {
            for (size_t v = 0; v < vertex_count; ++v) {
                path_matrix[u][v] = std::min(path_matrix[u][v], path_matrix[u][i] + path_matrix[i][v]);
            }
        }
    }
}

int main() {
    size_t vertex_count, start, finish;
    std::cin >> vertex_count >> start >> finish;
    std::vector<std::vector<size_t>> path_matrix = MakePathMatrix(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        for (size_t j = 0; j < vertex_count; ++j) {
            size_t weight;
            std::cin >> weight;
            if (weight == -1) {
                path_matrix[i][j] = MAX_PATH;
            } else {
                path_matrix[i][j] = weight;
            }
        }
    }
    Floyd(vertex_count, path_matrix);
    size_t path_len = path_matrix[start - 1][finish - 1];
    if (path_len == MAX_PATH) {
        std::cout << -1;
    } else {
        std::cout << path_len;
    }
    return 0;
}