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

void smix() {

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
            smix();
        }
    }

    for (int i = 0; i < 10; ++i) {
        ror(3);
        cmix();
        smix();
    }

    for (int i = 0; i < 13; ++i) {
        s[4] += s[0];
        s[15] += s[0];
        ror(15);
        smix();

        s[4] += s[0];
        s[16] += s[0];
        ror(14);
        smix();
    }

    s[4] += s[0];
    s[15] += s[0];

    return {s[1], s[2], s[3], s[4], s[15], s[16], s[17], s[18]};
}

int main() {
    string s;
    cin >> s;

    vector<uint32_t> hash_ = fugue_hash(s);

    for (int i = 0; i < hash_.size(); ++i) {
        cout << hash_[i] << " ";
    }
    cout << endl;

    return 0;
}