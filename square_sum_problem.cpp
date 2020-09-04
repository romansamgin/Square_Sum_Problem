#include <vector>
#include <cmath>
#include <numeric>

using std::vector;

const long double eps = 10e-16;

bool is_square(int i, int j) {
    double d = sqrt(i + j);
    int t = sqrt(i + j);
    return (d - t) < eps;
}

bool valid_path(int v, const vector<vector<int>>& g, const vector<int>& path, int pos) {
    if (g[path[pos - 1]][v] == 0) {
        return false;
    }
    for (int i = 0; i < pos; ++i) {
        if (path[i] == v) {
            return false;
        }
    }
    return true;
}

bool make_path(const vector<vector<int>>& g, vector<int>& path, int pos) {

    if (pos == g.size()) {
        return true;
    }
    for (int v = 1; v < g.size(); ++v) {
        if (valid_path(v, g, path, pos)) {
            path[pos] = v;
            if (make_path(g, path, pos + 1)) {
                return true;
            }
            path[pos] = -1;
        }
    }
    return false;
}


enum class WhatMove {
    SWAP,
    REVERSE_1st,
    REVERSE_2nd,
    DEFAULT
};

WhatMove generate_move() {
    switch (rand() % 3) {
    case 0:
        return WhatMove::SWAP;
    case 1:
        return WhatMove::REVERSE_1st;
    case 2:
        return WhatMove::REVERSE_2nd;
    default:
        return WhatMove::DEFAULT;
    }
}

vector<int> find_path(int n, vector<int>& general) {
    vector<int> second_part = { n };
    if (is_square(n, general.front())) {
        general.insert(general.begin(), n);
        return general;
    }
    if (is_square(n, general.back())) {
        general.push_back(n);
        return general;
    }
    vector<int> tmp1;
    vector<int> tmp2;

    while (true) {
        switch (generate_move()) {
        case WhatMove::SWAP: {
            swap(general, second_part);
            break;
        }
        case WhatMove::REVERSE_1st: {
            reverse(general.begin(), general.end());
            break;
        }
        case WhatMove::REVERSE_2nd: {
            reverse(second_part.begin(), second_part.end());
            break;
        }
        default:
            break;
        }

        bool flag = false;
        size_t i = 0;
        for (i = 0; i < general.size(); ++i) {
            if (is_square(general[i], second_part.front())) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            continue;
        }

        i = 0;
        for (i = 0; i < general.size(); ++i) {
            tmp1.push_back(general[i]);
            if (is_square(general[i], second_part.front())) {
                break;
            }
        }
        for (i = i + 1; i < general.size(); ++i) {
            tmp2.push_back(general[i]);
        }

        general = move(tmp1);
        general.insert(general.end(), second_part.begin(), second_part.end());
        second_part = move(tmp2);

        if (general.size() == n) {
            return general;
        }
        if (is_square(general.back(), second_part.front())) {
            general.insert(general.end(), second_part.begin(), second_part.end());
            return general;
        }
        if (is_square(second_part.back(), general.front())) {
            second_part.insert(second_part.end(), general.begin(), general.end());
            return second_part;
        }
    }
    return vector<int>();
}


vector<int> square_sum_from32(int n) {
    //N = 32 - start
    vector<int> result = {
        1, 8, 28, 21, 4, 32, 17, 19, 30, 6, 3, 13, 12, 24, 25, 11, 5, 31, 18, 7, 29, 20, 16, 9, 27, 22, 14, 2, 23, 26, 10, 15
    };
    for (int i = 33; i <= n; ++i) {
        result = find_path(i, result);
    }
    return result;
}

vector<int> square_sums_row(int n) {
    if (n <= 32) {
        vector<vector<int>> g(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= i - 1; ++j) {
                if (is_square(i, j)) {
                    g[i][j] = 1;
                    g[j][i] = 1;
                }
            }
        }
        for (int i = 1; i <= n; ++i) {
            vector<int> path(n + 1);
            for (int i = 0; i <= n; ++i) {
                path[i] = -1;
            }
            path[1] = i;
            if (make_path(g, path, 2)) {
                path.erase(path.begin());
                return path;
            }
        }
        return vector<int>();
    }
    else {
        return square_sum_from32(n);
    }
}

