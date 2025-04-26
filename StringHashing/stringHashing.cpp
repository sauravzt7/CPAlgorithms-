#include <bits/stdc++.h>
using namespace std;


struct pair_hash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
    }
};

/*
    This Hash struct gives a hash value for a string.
    It uses two different bases and a large prime modulus to reduce collisions.
    The hash value is computed as:
    hash(s) = (s[0] * BASE1^0 + s[1] * BASE1^1 + ... + s[n-1] * BASE1^(n-1)) % MOD
              + (s[0] * BASE2^0 + s[1] * BASE2^1 + ... + s[n-1] * BASE2^(n-1)) % MOD
    where n is the length of the string.
    The hash value is stored in a vector of pairs, where each pair contains the hash values for the two bases.
    The powValue vector stores the powers of the bases modulo MOD.
    

*/



struct Hash {
    const int MOD = 1e9 + 7, BASE1 = 5689, BASE2 = 8861;
    vector<pair<long long, long long>> hashValue, powValue; // Hash values and powers of bases wrt BASE1 and BASE2

    Hash(string s){
        hashValue.assign(s.size() + 1, {0, 0});
        powValue.assign(s.size() + 1, {1, 1});
        for(int i = 0; i < s.size(); i++){
            hashValue[i + 1] = {(1LL * hashValue[i].first * BASE1 + (s[i] - 'a' + 1)) % MOD, (1LL * hashValue[i].second * BASE2 + (s[i] - 'a' + 1)) % MOD };
            powValue[i + 1] = {(1LL * powValue[i].first * BASE1) % MOD, (1LL * powValue[i].second * BASE2) % MOD };
        };
    }

    pair<long long, long long> getRangeHash(int L, int R){
        // L and R are 1-indexed
        L++, R++;
        long long hash1 = (hashValue[R].first - (1LL * hashValue[L - 1].first * powValue[R - L + 1].first) % MOD + MOD) % MOD;
        long long hash2 = (hashValue[R].second - (1LL * hashValue[L - 1].second * powValue[R - L + 1].second) % MOD + MOD) % MOD;
        return {hash1, hash2};
    }
};

void fillPatternHash(string pattern, unordered_set<pair<int, int>, pair_hash>& patternHash){
    // Fill the pattern hash set with all possible hashes of the pattern
    
    unordered_set<string> uniquePatterns;
    int idx = pattern.find('*');
    if(idx == -1){
        uniquePatterns.insert(pattern);
        return;
    }
    int n = pattern.size();
    for(char c = 'a'; c <= 'z'; c++){
        string modifiedPattern = pattern;
        modifiedPattern[idx] = c;
        uniquePatterns.insert(modifiedPattern);
    }

    for(auto& p: uniquePatterns){
        Hash hash(p);
        auto hashValue = hash.getRangeHash(1, p.size());
        patternHash.insert(hashValue);
    }

}

signed main(){
    // Example usage
    string s;
    cin >> s;
    
    int len = s.size();

    if(len == 1){
        cout << s << endl;
        return 0;
    }

    string s2 = s;
    s.pop_back();
    s = s2 + s;
    Hash hash(s);

    vector<int> pos(len);
    iota(pos.begin(), pos.end(), 0);

    auto bestPos = *std::min_element(pos.begin(), pos.end(), [&](const int a, const int b) {
        int low = 0, high = len - 1, ind = len;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (a + mid >= s.size() || b + mid >= s.size()) break;

            auto hash1 = hash.getRangeHash(a, a + mid);
            auto hash2 = hash.getRangeHash(b, b + mid);

            if (hash1 == hash2) {
                low = mid + 1;
            } else {
                ind = mid;
                high = mid - 1;
            }
        }

        if (ind == len || a + ind >= s.size() || b + ind >= s.size()) return false;
        return s[a + ind] < s[b + ind];
    });

    
    
    string result = s.substr(bestPos, len);

    cout << result << "\n";

    
    return 0;
}