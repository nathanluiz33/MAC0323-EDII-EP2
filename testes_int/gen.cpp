#include<bits/stdc++.h>
using namespace std;

// #define int long long
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int mod = 1e5;

signed main(){
    int n; cin >> n;
    cout << n << "\n";
    for(int i = 0; i < n; i++){
        cout << rng() % mod << "\n";
    }
}