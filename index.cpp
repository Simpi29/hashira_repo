// ========== Offline C++ Version (two JSON files) ==========
// - Place input1.json and input2.json in the same folder as this source.
// - Uses nlohmann/json (single‑header) to parse JSON.
// - Compile with:
//     g++ -std=c++17 offline.cpp -o offline
// - Run:
//     ./offline

#include <bits/stdc++.h>
#include "json.hpp"           // https://github.com/nlohmann/json
using namespace std;
using json = nlohmann::json;
using ll = long long;

// decode string s in base b into a 64‑bit integer
ll decode(const string &s, int b) {
    ll v = 0;
    for (char c : s) {
        int d = isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
        v = v * b + d;
    }
    return v;
}

// compute constant term c = f(0) via Lagrange interpolation
long double lagrange_c(const vector<ll>& x, const vector<long double>& y) {
    int k = x.size();
    long double c = 0;
    for (int i = 0; i < k; ++i) {
        long double L = 1;
        for (int j = 0; j < k; ++j) {
            if (i == j) continue;
            L *= (0 - x[j]) / (long double)(x[i] - x[j]);
        }
        c += y[i] * L;
    }
    return c;
}

int main() {
    vector<string> files = { "input1.json", "input2.json" };

    for (auto &fname : files) {
        ifstream in(fname);
        if (!in) {
            cerr << "Error: cannot open " << fname << "\n";
            continue;
        }

        json J;
        in >> J;

        int k = J["keys"]["k"];
        vector<ll> xs;
        vector<long double> ys;
        xs.reserve(k);
        ys.reserve(k);

        // collect first k points
        for (auto &it : J.items()) {
            if (it.key() == "keys") continue;
            if ((int)xs.size() == k) break;

            ll x = stoll(it.key());
            int base = it.value()["base"];
            string val = it.value()["value"];

            xs.push_back(x);
            ys.push_back((long double)decode(val, base));
        }

        ll secret = (ll) llround(lagrange_c(xs, ys));
        cout << "Secret from " << fname << ": " << secret << "\n";
    }

    return 0;
}
