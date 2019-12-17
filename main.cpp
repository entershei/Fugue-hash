#include <utility>

#include <iostream>
#include <vector>

#include "tables.h"

using namespace std;

vector<uint32_t> translate_to_vector_uint32(string s) {
    vector<uint32_t> res;

    int a = s.size() % 4;

    for (int i = 0; i < 4 - a; ++i) {
        s.push_back(char(0));
    }

    for (int i = 0; i < s.size(); i += 4) {
        res.push_back((s[i] << 24) + (s[i + 1] << 16) + (s[i + 2] << 8) + s[i + 3]);
    }

    return res;
}

vector<uint32_t> s(30, 0);

void tix(uint32_t i) {
    s[10] += s[0];
    s[0] = i;
    s[8] += s[0];
    s[1] += s[24];
}

void ror(int x) {
    vector<uint32_t> ret(30);

    for (int i = x; i < 30; ++i) {
        ret[i] = s[i - x];
    }

    for (int i = 0; i < x; ++i) {
        ret[i] = s[30 - x + i];
    }

    s = ret;
}

void cmix() {
    s[0] += s[4];
    s[1] += s[5];
    s[2] += s[6];
    s[15] += s[4];
    s[16] += s[5];
    s[17] += s[6];
}

uint8_t get_byte(uint32_t number, int index) { // index = 0, 1, 2, 3
    uint32_t x = number << (8 * index);
    return uint8_t(x >> 24);
}

vector<vector<uint8_t >> M = {{1, 4, 7, 1},
                              {1, 1, 4, 7},
                              {7, 1, 1, 4},
                              {4, 7, 1, 1}};

vector<vector<uint8_t >> M_t = {{1, 1, 7, 4},
                                {4, 1, 1, 7},
                                {7, 4, 1, 1},
                                {1, 7, 4, 1}};

vector<vector<uint8_t >> multiply_matrix_4(vector<vector<uint8_t >> const &a, vector<vector<uint8_t >> const &b) {
    vector<vector<uint8_t >> result(4, vector<uint8_t>(4, 0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k)
                result[i][j] += (a[i][k] * b[k][j]);
        }
    }
    return result;
}

vector<vector<uint8_t >> sum_matrix_4(vector<vector<uint8_t >> const &a, vector<vector<uint8_t >> const &b) {
    vector<vector<uint8_t >> result(4, vector<uint8_t>(4, 0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

vector<vector<uint8_t >> get_u_matrix() {
    vector<vector<uint8_t >> result(4, vector<uint8_t>(4, 0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i][j] = get_byte(s[j], i);
        }
    }
    return result;
}

vector<vector<uint8_t >> get_sum_u_matrix(vector<vector<uint8_t >> const &u) {
    vector<vector<uint8_t >> result(4, vector<uint8_t>(4, 0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i == j) {
                continue;
            }
            result[i][i] += u[i][j];
        }
    }
    return result;
}

vector<vector<uint8_t >> rol_matrix(vector<vector<uint8_t >> const &w) {
    vector<vector<uint8_t >> result(4, vector<uint8_t>(4, 0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i][j] = w[i][(j + i) % 4];
        }
    }
    return result;
}

void set_rol_w_in_s(vector<vector<uint8_t >> const &w) {
    s[0] = s[1] = s[2] = s[3] = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            s[j] += (w[i][j] << (24 - i * 8));
        }
    }
}

void super_mixed() {
    vector<vector<uint8_t >> u = get_u_matrix();
    vector<vector<uint8_t >> m_u = multiply_matrix_4(M, u);
    vector<vector<uint8_t >> sum_u = get_sum_u_matrix(u);
    vector<vector<uint8_t >> sum_u_m_t = multiply_matrix_4(sum_u, M_t);
    vector<vector<uint8_t >> w = sum_matrix_4(m_u, sum_u_m_t);
    vector<vector<uint8_t >> rol_w = rol_matrix(w);
    set_rol_w_in_s(rol_w);
}

vector<uint32_t> fugue_hash(string message) {
    vector<uint32_t> p = translate_to_vector_uint32(std::move(message));

    for (int i = 0; i <= 7; ++i) {
        s[i + 22] = IV[i];
    }

    size_t m = p.size();

    for (int i = 0; i < m; ++i) {
        tix(p[i]);

        for (int j = 0; j < 2; ++j) {
            ror(3);
            cmix();
            super_mixed();
        }
    }

    for (int i = 0; i < 10; ++i) {
        ror(3);
        cmix();
        super_mixed();
    }

    for (int i = 0; i < 13; ++i) {
        s[4] += s[0];
        s[15] += s[0];
        ror(15);
        super_mixed();

        s[4] += s[0];
        s[16] += s[0];
        ror(14);
        super_mixed();
    }

    s[4] += s[0];
    s[15] += s[0];

    return {s[1], s[2], s[3], s[4], s[15], s[16], s[17], s[18]};
}

int main() {
    cout << "Enter your message to hashing:" << endl;

    string message;
    cin >> message;

    vector<uint32_t> hash_ = fugue_hash(message);

    cout << "Hash:" << endl;
    for (unsigned int i : hash_) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}